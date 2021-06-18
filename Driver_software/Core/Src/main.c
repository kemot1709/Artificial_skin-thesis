#include "main.h"
#include "adc.h"
#include "dma.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


void SystemClock_Config();

/**
  * @brief  The application entry point.
  * @retval int
  */
int main() {
    HAL_Init();
    SystemClock_Config();
    HAL_FLASH_Unlock();

    MX_GPIO_Init();

    /* Check if the system has resumed from IWDG reset */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
        HAL_GPIO_TogglePin(LED_ERROR_Port, LED_ERROR_Pin);
        __HAL_RCC_CLEAR_RESET_FLAGS();
    } else {
    }

    // Check if config is ready or it's clear download
    if ((*(uint8_t *) EEPROM_FIELD_CONFIG_START) != (uint8_t) 0xFF) {
        field_config_flag = 1;
        // Research all available memory

        for (uint32_t i = EEPROM_FIELD_CONFIG_START; i < EEPROM_FIELD_CONFIG_START +
                                                         (EEPROM_PAGE_SIZE *
                                                          EEPROM_FIELD_PAGES); i += EEPROM_PAGE_SIZE) {
            if (first_tactile_ptr == 0 && (*(uint8_t *) i == FIELD_TYPE_TACTILE)) {
                first_tactile_ptr = i;
            }

            if (*(uint8_t *) i == 0xFF) {
                field_config_fulfilment = i - EEPROM_FIELD_SIZE;
                break;
            }
        }
    }

    MX_ADC1_Init(); // ADC
    MX_TIM1_Init(); // ADC and USB control
//    MX_TIM2_Init(); // RES
//    MX_USART1_UART_Init(); // BT
    MX_USART3_UART_Init(); // USB

    // Watchdog initialization
//    MX_IWDG_Init();

    // Timer odliczający czas do dokładnego wykonywania pomiarów analogowych
    HAL_TIM_Base_Start_IT(&UTIL_TIM_Handler);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1) {
        HAL_Delay(50);
//        HAL_IWDG_Refresh(&hiwdg);
        uart_input_time_flag++;
    }
#pragma clang diagnostic pop
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config() {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType =
            RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler() {

}


