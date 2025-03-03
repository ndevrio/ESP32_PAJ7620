#include "apds.h"

APDS9500::APDS9500() {
}

uint8_t APDS9500::init() {
	// Write, then read
	_buffer[0] = APDS9500_PartID_L;
	_buffer[1] = 0;

  Serial.println("Trying I2C");

  // Request, read, and confirm chip ID
  Wire.begin();
  Wire.beginTransmission(APDS9500_I2CADDR_DEFAULT);
  Wire.write(_buffer, 1);
  Wire.endTransmission();

  Wire.requestFrom(APDS9500_I2CADDR_DEFAULT, 2);
  while(Wire.available()) {
      for(int i = 0; i < 2; i++) {
          _buffer[i] = Wire.read();
      }
  }

  Serial.println((_buffer[1] << 8 | _buffer[0]));

	if ((_buffer[1] << 8 | _buffer[0]) != APDS9500_CHIP_ID) {
		return 1;
	}

  return 0;
}

uint8_t APDS9500::user_init() {
  writeReg(APDS9500_R_RegBankSet, 0x00); 			// select bank 0

	writeReg(0x32, 0x29);
	writeReg(0x37, 0x09);
	writeReg(0x38, 0x15);
	writeReg(0x39, 0x0A);
	writeReg(0x3A, 0x12);
	writeReg(0x41, 0x00);
	writeReg(0x42, 0x00);
	writeReg(0x46, 0x60);
	writeReg(0x47, 0x20);
	writeReg(0x48, 0x80); // R_AE_Exposure_UB [7:0]
	writeReg(0x49, 0x04); // R_AE_Exposure_UB [15:8]
	writeReg(0x4A, 0x40); // R_AE_Exposure_LB [7:0]
	writeReg(0x4B, 0x02); // R_AE_Exposure_LB [15:8]
	writeReg(0x4C, 0x20); // R_AE_Gain_UB
	writeReg(0x51, 0x13); // R_AE_EnH
	writeReg(0x54, 0x20);
	writeReg(0x5A, 0x00); // AE_Normal_Factor
	writeReg(0x5B, 0x02);
	writeReg(0x5C, 0x06);
	writeReg(0x5E, 0x10);
	writeReg(0x5F, 0xBF);
	writeReg(0x80, 0x42);
	writeReg(0x81, 0x44);
	writeReg(0x82, 0x0B);
	writeReg(0x83, 0x20);
	writeReg(0x8B, 0x01);
	writeReg(0x90, 0x0C);
	writeReg(0x95, 0x0C);
	writeReg(0x96, 0x0A);
	writeReg(0x97, 0x04);
	writeReg(0x9A, 0x0A);
	writeReg(0x9C, 0x3F);
	writeReg(0x9F, 0xF9);
	writeReg(0xA3, 0x02);
	writeReg(0xA4, 0x00);
	writeReg(0xA5, 0x19);
	writeReg(0xCC, 0x1A);
	writeReg(0xCD, 0x0D);
	writeReg(0xCE, 0x03);
	writeReg(0xCF, 0x63);
	writeReg(0xD0, 0x22);
	writeReg(0xD2, 0x02);

	// Change to Bank 1
	writeReg(APDS9500_R_RegBankSet, 0x01); 			// select bank 1

	writeReg(0x00, 0x3C);
	writeReg(0x01, 0x3C);
	writeReg(0x02, 0x00);
	writeReg(0x03, 0x00);
	writeReg(0x04, 0x02);
	writeReg(0x1E, 0x02);
	writeReg(0x25, 0x00);
	writeReg(0x26, 0x40);
	writeReg(0x27, 0x39);
	writeReg(0x28, 0x7F);
	writeReg(0x29, 0x09);
	writeReg(0x32, 0x02); // R_LED1_DAC_UB
	writeReg(0x33, 0x00); // R_LED2_DAC_UB
	writeReg(0x3E, 0xC8);
	writeReg(0x41, 0x48);
	writeReg(0x4A, 0x3C);
	writeReg(0x4B, 0x3C);
	writeReg(0x5E, 0x59);
	writeReg(0x64, 0x06);
	writeReg(0x65, 0x96); // R_IDLE_TIME[7:0]
	writeReg(0x66, 0x00); // R_IDLE_TIME[15:8]
	writeReg(0x67, 0x97);
	writeReg(0x68, 0x01);
	writeReg(0x69, 0xCD);
	writeReg(0x6A, 0x01);
	writeReg(0x6B, 0xB0);
	writeReg(0x6C, 0x04);
	writeReg(0x6D, 0x2C);
	writeReg(0x6E, 0x01);
	writeReg(0x72, 0x01);
	writeReg(0x73, 0x00);
	writeReg(0x74, 0x01);
	writeReg(0x77, 0x00);
	writeReg(0x7C, 0x10);
	writeReg(0x7D, 0x0E);
	writeReg(0x7E, 0x01);

	// Change back to bank 0 for data read
	writeReg(APDS9500_R_RegBankSet, 0x00); 			// select bank 0

	return 0;
}


void APDS9500::selectRegBank(uint8_t reg) {
	_buffer[0] = APDS9500_R_RegBankSet;
	_buffer[1] = reg;
  Wire.beginTransmission(APDS9500_I2CADDR_DEFAULT);
  Wire.write(_buffer, 2);
  Wire.endTransmission();
}

void APDS9500::readReg(uint8_t addr, uint8_t *val) {
  Wire.beginTransmission(APDS9500_I2CADDR_DEFAULT);
  Wire.write(addr);
  Wire.endTransmission();

  Wire.requestFrom(APDS9500_I2CADDR_DEFAULT, 1);
  while(Wire.available()) {
      *val = Wire.read();
  }
}

void APDS9500::writeReg(uint8_t addr, uint8_t val) {
	_buffer[0] = addr;
	_buffer[1] = val;
  Wire.beginTransmission(APDS9500_I2CADDR_DEFAULT);
  Wire.write(_buffer, 2);
  Wire.endTransmission();
}
