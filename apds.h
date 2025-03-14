/*
 *  apds9500.h - Library for interfacing with the APDS9500 image sensor
 *  Created on: May 30, 2023
 *      Author: Nathan DeVrio (ndevrio)
 */
#include <Arduino.h>
#include <Wire.h>

#ifndef INC_APDS9500_H_
#define INC_APDS9500_H_


#define APDS9500_I2CADDR_DEFAULT 0x73 ///< APDS9500 default i2c address
#define APDS9500_CHIP_ID 0x7620 ///< APDS9500 default device id from WHOAMI

#define APDS9500_WHOAMI 0x00 ///< Chip ID register

#define APDS9500_R_RegBankSet 0xEF ///< 0x00 register bank 0, 0x01 register bank 1

/* Bank 0*/
#define APDS9500_PartID_L 0x00 ///< Low  byte of Part ID

/* Cursor Mode Controls */
#define APDS9500_R_CursorClampLeft 0x37     ///< Cursor Mode Control parameter
#define APDS9500_R_CursorClampRight 0x38    ///< Cursor Mode Control parameter
#define APDS9500_R_CursorClampUp 0x39       ///<  Cursor Mode Control parameter
#define APDS9500_R_Cursor_ObjectSizeTh 0x8B ///< Cursor Mode Control parameter

// /* Interrupt Controls */
#define APDS9500_R_Int2_En 0x42  ///< Interrupt Control parameter
#define APDS9500_Int_Flag_1 0x43 ///< Interrupt Control parameter
#define APDS9500_Int_Flag_2 0x44 ///< Interrupt Control parameter

// /* AE/AG Controls */
#define APDS9500_R_AELedOff_UB 0x46      ///< AE/AG Control parameter
#define APDS9500_R_AELedOff_LB 0x47      ///< AE/AG Control parameter
#define APDS9500_R_AE_Exposure_UB_L 0x48 ///< AE/AG Control parameter
#define APDS9500_R_AE_Exposure_UB_H 0x49 ///< AE/AG Control parameter
#define APDS9500_R_AE_Exposure_LB_L 0x4A ///< AE/AG Control parameter
#define APDS9500_R_AE_Gain_LB 0x4D       ///< AE/AG Control parameter
#define APDS9500_R_Manual 0x51           ///< AE/AG Control parameter

// /* GPIO Setting*/
#define APDS9500_InputMode_GPIO_0_1 0x80 ///< GPIO parameter
#define APDS9500_InputMode_GPIO_2_3 0x81 ///< GPIO parameter
#define APDS9500_InputMode_INT 0x82      ///< GPIO parameter

// /* Gesture Mode Controls */

#define APDS9500_R_NoMotionCountThd 0x90   ///< Gesture Mode Control parameter
#define APDS9500_R_ZDirectionThd 0x95      ///< Gesture Mode Control parameter
#define APDS9500_R_ZDirectionXYThd 0x96    ///< Gesture Mode Control parameter
#define APDS9500_R_ZDirectionAngleThd 0x97 ///< Gesture Mode Control parameter
#define APDS9500_R_RotateXYThd 0x9A        ///< Gesture Mode Control parameter
#define APDS9500_R_Filter 0x9C             ///< Gesture Mode Control parameter
#define APDS9500_R_FilterImage 0xA5        ///< Gesture Mode Control parameter
#define APDS9500_R_YtoZSum 0xCC            ///< Gesture Mode Control parameter
#define APDS9500_R_YtoZFactor 0xCD         ///< Gesture Mode Control parameter
#define APDS9500_R_FilterLength 0xCE       ///< Gesture Mode Control parameter
#define APDS9500_R_WaveThd 0xCF            ///< Gesture Mode Control parameter
#define APDS9500_R_AbortCountThd 0xD0      ///< Gesture Mode Control parameter

// /* Bank 1 */

/* Image size settings */
#define APDS9500_R_ImageHeight 0xAA
#define APDS9500_R_ImageWidth 0xAB
#define APDS9500_Cmd_HSize 0x00
#define APDS9500_Cmd_VSize 0x01
#define APDS9500_Cmd_HStart 0x02 ///< Image size parameter
#define APDS9500_Cmd_VStart 0x03 ///< Image size parameter
#define APDS9500_Cmd_HV 0x04     ///< Image size parameter

/* Image control settings */
#define APDS9500_R_SPIOUT_EnH 0x7E
#define APDS9500_R_SPIOUT_PXDNUM_L 0x7C
#define APDS9500_R_SPIOUT_PXDNUM_H 0x7D

// /* Lens Shading */
#define APDS9500_R_LensShadingComp_EnH 0x25 ///< Lens Shading Parameter
#define APDS9500_R_Offest_Y 0x27            ///< Lens Shading Parameter
#define APDS9500_R_LSC 0x28                 ///< Lens Shading Parameter
#define APDS9500_R_LSFT 0x29                ///< Lens Shading Parameter

// /* Sleep Mode Parameters */
#define APDS9500_R_IDLE_TIME_L 0x65         ///< Sleep Mode Parameter
#define APDS9500_R_IDLE_TIME_SLEEP_1_L 0x67 ///< Sleep Mode Parameter
#define APDS9500_R_IDLE_TIME_SLEEP_2_L 0x69 ///< Sleep Mode Parameter
#define APDS9500_R_IDLE_TIME_SLEEP_2_H 0x6A ///< Sleep Mode Parameter
#define APDS9500_R_Object_TIME_2_L 0x6D     ///< Sleep Mode Parameter
#define APDS9500_R_Object_TIME_2_H 0x6E     ///< Sleep Mode Parameter
#define APDS9500_R_TG_EnH 0x72              ///< Sleep Mode Parameter
#define APDS9500_R_Auto_SLEEP_Mode 0x73     ///< Sleep Mode Parameter
#define APDS9500_R_Wake_Up_Sig_Sel 0x74     ///< Sleep Mode Parameter

// /* Image Controls */
#define APDS9500_R_SRAM_Read_EnH 0x77 ///< Image control parameter
#define APDS9500_R_SPIOUT_CSN_MODE 0x7E


class APDS9500 {
    public:
        uint8_t init();
        uint8_t user_init();
        void selectRegBank(uint8_t reg);
        void readReg(uint8_t reg, uint8_t *data);
        void writeReg(uint8_t reg, uint8_t data);
        APDS9500();
    private:
        uint8_t _buffer[2];
};


#endif /* INC_APDS9500_H_ */
