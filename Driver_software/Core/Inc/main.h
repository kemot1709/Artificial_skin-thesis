#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"


enum {
    err_OK = 0x00,
    err_other = 0x01,
    err_wtf = 0x02,
    err_no_tactile_config = 0x10,
    err_no_config = 0xA0,
    err_try_to_get_outside_of_memory =0xA1,
};


#define SYS_CLK 16000000U
#define ADC_CLK 8000000U

#define EEPROM_START 0x08000000
#define EEPROM_END 0x08020000
#define EEPROM_FIELD_CONFIG_START 0x08018000
#define EEPROM_PAGE_SIZE 0x00000400
#define EEPROM_FIELD_PAGES 2 // 16x16x8 = 2048 (rows x columns x bytes)
#define EEPROM_FIELD_SIZE 8
#define EEPROM_VARIABLE_SIZE 4
#define EEPROM_ADDRESS(start, number, size) ((start) + ((number) * (size))


#define LED_WORK_Pin GPIO_PIN_3
#define LED_WORK_Port GPIOB
#define LED_ERROR_Pin GPIO_PIN_4
#define LED_ERROR_Port GPIOB

#define KEY_A0_Pin GPIO_PIN_5
#define KEY_A0_Port GPIOB
#define KEY_A1_Pin GPIO_PIN_6
#define KEY_A1_Port GPIOB
#define KEY_A2_Pin GPIO_PIN_7
#define KEY_A2_Port GPIOB
#define KEY_A3_Pin GPIO_PIN_8
#define KEY_A3_Port GPIOB
#define KEY_A4_Pin GPIO_PIN_9
#define KEY_A4_Port GPIOB
#define KEY_A5_Pin GPIO_PIN_13
#define KEY_A5_Port GPIOC
#define KEY_A6_Pin GPIO_PIN_14
#define KEY_A6_Port GPIOC
#define KEY_A7_Pin GPIO_PIN_15
#define KEY_A7_Port GPIOC

#define KEY_B0_Pin GPIO_PIN_12
#define KEY_B0_Port GPIOB
#define KEY_B1_Pin GPIO_PIN_13
#define KEY_B1_Port GPIOB
#define KEY_B2_Pin GPIO_PIN_14
#define KEY_B2_Port GPIOB
#define KEY_B3_Pin GPIO_PIN_15
#define KEY_B3_Port GPIOB
#define KEY_B4_Pin GPIO_PIN_6
#define KEY_B4_Port GPIOC
#define KEY_B5_Pin GPIO_PIN_7
#define KEY_B5_Port GPIOC
#define KEY_B6_Pin GPIO_PIN_8
#define KEY_B6_Port GPIOC
#define KEY_B7_Pin GPIO_PIN_9
#define KEY_B7_Port GPIOC

#define UART_IN_BUFF_SIZE 40
#define ASCII_STX 0x02
#define ASCII_ETX 0x03
#define UART_MESAGE_LEN 8

#define USB_UART_Port huart3

#define BT_UART_Port huart1
#define BT_STATE_Pin GPIO_PIN_8
#define BT_STATE_Port GPIOA
#define BT_ENABLE_Pin GPIO_PIN_11
#define BT_ENABLE_Port GPIOA

#define UTIL_TIM_Handler htim1
#define UTIL_TIM_FREQ 10


#define FIELD_TYPE_ERASE_PAGES 0x01
#define FIELD_TYPE_CONFIG_READY 0x02
#define FIELD_TYPE_TACTILE 0x10
#define FIELD_TYPE_TEMPERATURE 0x20


#define TACT_HYPERBOLA_MEASURE_PAR1 600.0f // y shift
#define TACT_HYPERBOLA_MEASURE_PAR2 2500000.0f // hyperbolic parameter
#define TACT_HYPERBOLA_MEASURE_PAR3 -750.0f // x shift

#define TACT_FORCE_DIVIDER 100.0f
#define TACT_IGNORANCE_LIMIT 3800 // 31.25g

#define TACT_MEASURE(raw_analog) (uint16_t) (TACT_HYPERBOLA_MEASURE_PAR3 + \
                    (TACT_HYPERBOLA_MEASURE_PAR2 / ((raw_analog) - TACT_HYPERBOLA_MEASURE_PAR1)))


typedef struct {
    uint8_t type;
    uint8_t remained;
    uint8_t key;
    uint8_t channel;
    uint16_t angle;
    uint16_t distance;
} TactileField;


void Error_Handler();

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
