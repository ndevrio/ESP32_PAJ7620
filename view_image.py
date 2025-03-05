import serial
import base64
import numpy as np
import matplotlib.pyplot as plt
import time
import socket
import pickle


ser = serial.Serial('COM3', 10000000, parity=serial.PARITY_NONE)


# matplotlib setup
plt.ion()

x = 60
frame = np.zeros((x, x))
fig = plt.figure(figsize=(5, 5))
im = plt.imshow(frame, cmap='gray', vmin=0, vmax=255)
im.set_data(frame)


def bytes_to_9bit_array(data):
    """Converts a byte array to an array of 9-bit integers.

    Args:
        data: A byte array (bytes or bytearray).

    Returns:
        A list of integers, where each integer represents a 9-bit value.
    """
    result = []
    temp = 0
    bit_count_L = 8
    bit_count_H = 1
    i = 0
    for byte in data:
        if(i != 0):
            temp |= byte >> bit_count_L
            result.append((temp & 0x1FF) >> 1)  # Extract 9 bits
            temp = 0

        if(i != 8):    
            temp |= byte << bit_count_H

        bit_count_L -= 1
        if(bit_count_L < 0):
            bit_count_L = 8
        bit_count_H += 1
        if(bit_count_H > 8):
            bit_count_H = 0

        i += 1
        if(i > 8):
            i = 0

    return result


def read_until(ser, target, timeout=5):
    start_time = time.time()
    in_waiting = ser.in_waiting
    data = bytearray()
    while time.time() - start_time < timeout:
        if ser.in_waiting > in_waiting:
            new_data = ser.read(ser.in_waiting - in_waiting)
            data.extend(new_data)
            if target in data.decode('utf-8', errors='ignore'):
                return data.decode('utf-8', errors='ignore')
            in_waiting = ser.in_waiting
    return data#.decode('utf-8', errors='ignore')


def main():
    ser.flush()

    last_time = time.time()
    cnt = 0
    while True:
        ser.flush()

        # Request a new frame over serial and read it in


        ser.write(b'i')


        time.sleep(0.025)
        l = ser.read(ser.in_waiting)

        # l = ser.read(4057)

        # while(ser.inWaiting() < 4057):
        #     continue
        # l = ser.read(4052+5)
        # # l = ser.readline()
        # # l = read_until(ser, "\n\n\n\n\n")
        print(len(l))
        if(len(l) != 4057):
            continue
        l = l[:-7]

        f_in = bytes_to_9bit_array(l)

        frame = np.array(f_in[:x*x]).reshape((x, x)).astype(int)

        frame = np.rot90(frame)
        #frame = np.fliplr(frame)
        # frame = np.rot90(frame)
        # frame = np.rot90(frame)

        # frame = np.roll(frame, -20, axis=0)
        # frame[-20:] = np.roll(frame[-20:], 1, axis=1)

        # time.sleep(0.01)

        # Display the new frame
        im.set_data(frame)
        fig.canvas.flush_events()




if __name__ == '__main__':
    main()
