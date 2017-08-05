/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define TARGET_BOARD_IDENTIFIER "CUPD" // CUPDRONE
#define USE_HARDWARE_REVISION_DETECTION
#define TARGET_CONFIG


#define LED0_PIN                PB4
#define LED1_PIN                PB5


#undef BEEPER
#undef USE_PWM
#undef USE_PPM
#undef LED_STRIP
#undef TELEMETRY
#undef TELEMETRY_FRSKY
#undef TELEMETRY_HOTT
#undef TELEMETRY_SMARTPORT
#undef USE_RESOURCE_MGMT
#undef USE_SERVOS
#undef SERIAL_RX
#undef USE_SERIALRX_CRSF       // Team Black Sheep Crossfire protocol
#undef USE_SERIALRX_IBUS       // FlySky and Turnigy receivers
#undef USE_SERIALRX_SBUS       // Frsky and Futaba receivers
#undef USE_SERIALRX_SPEKTRUM   // SRXL, DSM2 and DSMX protocol
#undef USE_SERIALRX_SUMD       // Graupner Hott protocol
#undef USE_SERIALRX_SUMH       // Graupner legacy protocol
#undef USE_SERIALRX_XBUS       // JR

#define GYRO
#define USE_GYRO_MPU6050

#define ACC
#define USE_ACC_MPU6050

//#define MAG
//#define USE_MAG_HMC5883

#define USE_EXTI
#define MPU_INT_EXTI            PC14
#define USE_MPU_DATA_READY_SIGNAL


#define BRUSHED_MOTORS

#define USE_VCP
#define USE_UART1
#define USE_UART2

#define SERIAL_PORT_COUNT       3

#define USE_I2C
#define USE_I2C_DEVICE_2
#define I2C_DEVICE              (I2CDEV_2)



#define DEFAULT_RX_FEATURE      FEATURE_RX_MSP
#define USE_RX_MSP




#define BRUSHED_MOTORS
#define DEFAULT_FEATURES        FEATURE_MOTOR_STOP
//#define SKIP_SERIAL_PASSTHROUGH
//#undef USE_CLI

// Since the CJMCU PCB has holes for 4 motors in each corner we can save same flash space by disabling support for other mixers.
#define USE_QUAD_MIXER_ONLY



//#undef BLACKBOX



// IO - assuming all IOs on 48pin package TODO
#define TARGET_IO_PORTA         0xffff
#define TARGET_IO_PORTB         0xffff
#define TARGET_IO_PORTC         (BIT(13)|BIT(14)|BIT(15))

#define USABLE_TIMER_CHANNEL_COUNT 4
//#define USED_TIMERS             	(TIM_N(1) | TIM_N(2) | TIM_N(3) | TIM_N(4))
#define USED_TIMERS               (TIM_N(3) | TIM_N(4))
