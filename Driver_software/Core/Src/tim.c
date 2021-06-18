/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <adc.h>
#include "usart.h"
#include "tim.h"

#include <stdio.h>
#include <math.h>


//TODO wypierdolić gdzieś do jakiegoś utils
#define ROWS 8
#define COLUMNS 8
#define AVERAGE_BUFFER 2

uint16_t digitalPin[16] = {KEY_A0_Pin, KEY_A1_Pin, KEY_A2_Pin, KEY_A3_Pin, KEY_A4_Pin, KEY_A5_Pin,
                           KEY_A6_Pin, KEY_A7_Pin, KEY_B0_Pin, KEY_B1_Pin, KEY_B2_Pin, KEY_B3_Pin,
                           KEY_B4_Pin, KEY_B5_Pin, KEY_B6_Pin, KEY_B7_Pin};
GPIO_TypeDef *digitalPort[16] = {KEY_A0_Port, KEY_A1_Port, KEY_A2_Port, KEY_A3_Port, KEY_A4_Port,
                                 KEY_A5_Port, KEY_A6_Port, KEY_A7_Port, KEY_B0_Port, KEY_B1_Port,
                                 KEY_B2_Port, KEY_B3_Port, KEY_B4_Port, KEY_B5_Port, KEY_B6_Port,
                                 KEY_B7_Port};

uint8_t analog_data_ready_flag = 0;
uint16_t analogData[COLUMNS];
uint16_t analog_buffer[ROWS * COLUMNS][AVERAGE_BUFFER];
uint8_t analog_buffer_circular_ptr = 0;


TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

uint32_t first_tactile_ptr = 0;
uint32_t first_temperature_ptr = 0;
uint32_t field_config_fulfilment = EEPROM_FIELD_CONFIG_START;
uint8_t field_config_flag = 0;

float tactile_angle;
float tactile_force;


/* TIM1 init function */
void MX_TIM1_Init(void) {
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 15999;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = (1000 / UTIL_TIM_FREQ) - 1;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_OC_Init(&htim1) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_TIMING;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK) {
        Error_Handler();
    }

}

/* TIM2 init function */
void MX_TIM2_Init(void) {
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_OC_Init(&htim2) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_TIMING;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }

}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *tim_ocHandle) {

    if (tim_ocHandle->Instance == TIM1) {
        /* USER CODE BEGIN TIM1_MspInit 0 */

        /* USER CODE END TIM1_MspInit 0 */
        /* TIM1 clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();

        /* TIM1 interrupt Init */
        HAL_NVIC_SetPriority(TIM1_BRK_IRQn, 15, 0);
        HAL_NVIC_EnableIRQ(TIM1_BRK_IRQn);
        HAL_NVIC_SetPriority(TIM1_UP_IRQn, 15, 0);
        HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
        HAL_NVIC_SetPriority(TIM1_TRG_COM_IRQn, 15, 0);
        HAL_NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);
        HAL_NVIC_SetPriority(TIM1_CC_IRQn, 15, 0);
        HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
        /* USER CODE BEGIN TIM1_MspInit 1 */

        /* USER CODE END TIM1_MspInit 1 */
    } else if (tim_ocHandle->Instance == TIM2) {
        /* USER CODE BEGIN TIM2_MspInit 0 */

        /* USER CODE END TIM2_MspInit 0 */
        /* TIM2 clock enable */
        __HAL_RCC_TIM2_CLK_ENABLE();

        /* TIM2 interrupt Init */
        HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
        /* USER CODE BEGIN TIM2_MspInit 1 */

        /* USER CODE END TIM2_MspInit 1 */
    }
}

void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef *tim_ocHandle) {

    if (tim_ocHandle->Instance == TIM1) {
        /* USER CODE BEGIN TIM1_MspDeInit 0 */

        /* USER CODE END TIM1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM1_CLK_DISABLE();

        /* TIM1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM1_BRK_IRQn);
        HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);
        HAL_NVIC_DisableIRQ(TIM1_TRG_COM_IRQn);
        HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
        /* USER CODE BEGIN TIM1_MspDeInit 1 */

        /* USER CODE END TIM1_MspDeInit 1 */
    } else if (tim_ocHandle->Instance == TIM2) {
        /* USER CODE BEGIN TIM2_MspDeInit 0 */

        /* USER CODE END TIM2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM2_CLK_DISABLE();

        /* TIM2 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM2_IRQn);
        /* USER CODE BEGIN TIM2_MspDeInit 1 */

        /* USER CODE END TIM2_MspDeInit 1 */
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &UTIL_TIM_Handler) {
        take_Measurements();
        if (!check_Config_Presence()) {
            calculate_Tactile_Output();

            static uint8_t send_data[12];
            // Making sure that force wont get out of range
            if (tactile_force > 9999) {
                tactile_force = 9999;
            }
            sprintf((char *) &send_data[0], "%04d,%04d;\r\n", radian2degree(tactile_angle),
                    (uint16_t) tactile_force);

            // DEBUG tutaj jest kod wysyłający czyste pomiary do użytkownika
//            static uint8_t send_data[24 * 5 + 2];
//            for (int i = 0; i < 8; i++) { // klucze
//                for (int j = 0; j < 3; j++) { // channels
//                    sprintf((char *) &send_data[5 * (i * 3 + j)], "%04d,",
//                            analog_buffer[i * 8 + j][analog_buffer_circular_ptr]);
//                }
//                sprintf((char *) &send_data[5 * ((i + 1) * 3) - 1], "|");
//            }
//            sprintf((char *) &send_data[120], "\n\r");

            HAL_UART_Transmit_IT(&USB_UART_Port, send_data, sizeof(send_data));
        }
        HAL_GPIO_TogglePin(LED_WORK_Port, LED_WORK_Pin);
    }
}

void take_Measurements() {
    for (int i = 0; i < ROWS; i++) {
        HAL_GPIO_WritePin(digitalPort[i], digitalPin[i], GPIO_PIN_SET);
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *) analogData, COLUMNS);
        while (!analog_data_ready_flag) {
            __NOP();
        }
        analog_data_ready_flag = 0;
        // TODO czy on zdąża???
        // Kurwa musi xd
        // Tak, ale tylko ~11 pomiarów potem to nawet DMA nie daje rady
        // czekanie nie zmianę dma state nie pomaga
        for (int j = 0; j < COLUMNS; j++) {
            uint16_t value = analogData[j];
            analog_buffer[i * COLUMNS + j][analog_buffer_circular_ptr] = value;
        }
        // Reset of keys at the end of the loop
        reset_All_Keys();
    }
    analog_buffer_circular_ptr = (analog_buffer_circular_ptr + 1) % AVERAGE_BUFFER;
}

/**
 * @brief Optimazed version to reset keys. Do not use on different design!!!
 */
void reset_All_Keys() {
    HAL_GPIO_WritePin(GPIOB,
                      GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_12 |
                      GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_13 |
                             GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
}

uint8_t calculate_Tactile_Output() {
    // TODO Obliczasnie kierunku i mocy wynikowej przykłądanej siły
    // Check if we got config of tactile
    if (first_tactile_ptr == 0) {
        return err_no_tactile_config;
    }

    // Reset tactile sensor values
    uint32_t actual_memory_ptr = first_tactile_ptr;
    tactile_force = 0;
    tactile_angle = 0;
    while (1) {
        // Check if tested field is tactile
        if ((*(uint8_t *) actual_memory_ptr) == FIELD_TYPE_TACTILE) {
            uint8_t skip_flag = 0;
            uint8_t key = (*(uint8_t *) (actual_memory_ptr + 2));
            uint8_t channel = (*(uint8_t *) (actual_memory_ptr + 3));
            float pom_angle = degree2radian(*(uint16_t *) (actual_memory_ptr + 4));
            float pom_dist = (float) (*(uint16_t *) (actual_memory_ptr + 6));

            // If distance is zero skip field
            if (pom_dist == 0.0f) {
                skip_flag = 1;
            }
            uint16_t pom_average = calculate_average_analog(key * COLUMNS + channel);

            // TODO Do przemyślenia jak to rozwiązać
            // If we got so little signal that we can interpret it as noise
            // skip that measure and go to next one
            if (pom_average > TACT_IGNORANCE_LIMIT) {
                skip_flag = 1;
            }

            if (!skip_flag) {
                float pom_force = calculate_Force(pom_average) / (pom_dist / TACT_FORCE_DIVIDER);

                // Calculate new force vector
                float force_x = pom_force * cosf(pom_angle) + tactile_force * cosf(tactile_angle);
                float force_y = pom_force * sinf(pom_angle) + tactile_force * sinf(tactile_angle);
                tactile_force = sqrtf(force_x * force_x + force_y * force_y);
                tactile_angle = atan2f(force_y, force_x);
            }

            // It was last tactile field - end loop
            if ((*(uint8_t *) (actual_memory_ptr + 1)) == 0) {
                break;
            }
        }

        // Go to next field and check if we are still in range
        actual_memory_ptr += EEPROM_FIELD_SIZE;
        if (actual_memory_ptr >= field_config_fulfilment) {
            return err_try_to_get_outside_of_memory;
        }
    }
    return err_OK;
}

uint8_t check_Config_Presence() {
    if (!field_config_flag) {
        return err_no_config;
    }
    return err_OK;
}

float degree2radian(uint16_t degree) {
    return (float) degree * PI / 180;
}

uint16_t radian2degree(float radian) {
    // Make sure I have positive angle, otherwise was returning bullshit
    while (radian < 0) {
        radian += 2 * PI;
    }
    uint16_t degree = (uint16_t) (radian * 180.0f / PI);
    return degree % 360;
}

float calculate_Force(uint16_t raw_analog) {
    return TACT_MEASURE(raw_analog);
}

uint16_t calculate_average_analog(uint8_t cell) {
    uint16_t sum = 0;
    for (int i = 0; i < AVERAGE_BUFFER; i++) {
        sum += analog_buffer[cell][i];
    }
    return sum / AVERAGE_BUFFER;
}

void erase_FLASH(uint32_t start, uint8_t pages) {
    // Erase EEPROM
    uint32_t page_error = 0;
    FLASH_EraseInitTypeDef s_eraseinit;

    s_eraseinit.TypeErase = FLASH_TYPEERASE_PAGES;
    s_eraseinit.PageAddress = start;
    s_eraseinit.NbPages = pages; // Erase with place for laser configuration

    HAL_FLASHEx_Erase(&s_eraseinit, &page_error);
}

