#include <dma.h>
#include "adc.h"


ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;


/* ADC1 init function */
void MX_ADC1_Init(void) {
    MX_DMA_Init();

    ADC_ChannelConfTypeDef sConfig = {0};

    /** Common config
    */
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 16;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        Error_Handler();
    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_10;
//    sConfig.Rank = ADC_REGULAR_RANK_1;
//    sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_11;
//    sConfig.Rank = ADC_REGULAR_RANK_2;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_12;
//    sConfig.Rank = ADC_REGULAR_RANK_3;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_13;
//    sConfig.Rank = ADC_REGULAR_RANK_4;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_0;
//    sConfig.Rank = ADC_REGULAR_RANK_5;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_1;
//    sConfig.Rank = ADC_REGULAR_RANK_6;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_2;
//    sConfig.Rank = ADC_REGULAR_RANK_7;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_3;
//    sConfig.Rank = ADC_REGULAR_RANK_8;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_4;
//    sConfig.Rank = ADC_REGULAR_RANK_9;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_5;
//    sConfig.Rank = ADC_REGULAR_RANK_10;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_6;
//    sConfig.Rank = ADC_REGULAR_RANK_11;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_7;
//    sConfig.Rank = ADC_REGULAR_RANK_12;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_14;
//    sConfig.Rank = ADC_REGULAR_RANK_13;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_15;
//    sConfig.Rank = ADC_REGULAR_RANK_14;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_8;
//    sConfig.Rank = ADC_REGULAR_RANK_15;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_9;
//    sConfig.Rank = ADC_REGULAR_RANK_16;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        Error_Handler();
//    }

    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_15;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_9;
    sConfig.Rank = ADC_REGULAR_RANK_3;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_10;
    sConfig.Rank = ADC_REGULAR_RANK_4;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_11;
    sConfig.Rank = ADC_REGULAR_RANK_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_12;
    sConfig.Rank = ADC_REGULAR_RANK_6;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_13;
    sConfig.Rank = ADC_REGULAR_RANK_7;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_REGULAR_RANK_8;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_9;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_2;
    sConfig.Rank = ADC_REGULAR_RANK_10;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = ADC_REGULAR_RANK_11;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = ADC_REGULAR_RANK_12;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_5;
    sConfig.Rank = ADC_REGULAR_RANK_13;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_6;
    sConfig.Rank = ADC_REGULAR_RANK_14;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_7;
    sConfig.Rank = ADC_REGULAR_RANK_15;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_14;
    sConfig.Rank = ADC_REGULAR_RANK_16;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (adcHandle->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* ADC1 clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC1 GPIO Configuration
        PC0     ------> ADC1_IN10
        PC1     ------> ADC1_IN11
        PC2     ------> ADC1_IN12
        PC3     ------> ADC1_IN13
        PA0-WKUP     ------> ADC1_IN0
        PA1     ------> ADC1_IN1
        PA2     ------> ADC1_IN2
        PA3     ------> ADC1_IN3
        PA4     ------> ADC1_IN4
        PA5     ------> ADC1_IN5
        PA6     ------> ADC1_IN6
        PA7     ------> ADC1_IN7
        PC4     ------> ADC1_IN14
        PC5     ------> ADC1_IN15
        PB0     ------> ADC1_IN8
        PB1     ------> ADC1_IN9
        */
        GPIO_InitStruct.Pin =
                GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin =
                GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
                GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* ADC1 DMA Init */
        /* ADC1 Init */
        hdma_adc1.Instance = DMA1_Channel1;
        hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
        hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_adc1.Init.Mode = DMA_NORMAL;
        hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_adc1) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc1);

        /* ADC1 interrupt Init */
        HAL_NVIC_SetPriority(ADC1_2_IRQn, 10, 0);
        HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle) {

    if (adcHandle->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();

        /**ADC1 GPIO Configuration
        PC0     ------> ADC1_IN10
        PC1     ------> ADC1_IN11
        PC2     ------> ADC1_IN12
        PC3     ------> ADC1_IN13
        PA0-WKUP     ------> ADC1_IN0
        PA1     ------> ADC1_IN1
        PA2     ------> ADC1_IN2
        PA3     ------> ADC1_IN3
        PA4     ------> ADC1_IN4
        PA5     ------> ADC1_IN5
        PA6     ------> ADC1_IN6
        PA7     ------> ADC1_IN7
        PC4     ------> ADC1_IN14
        PC5     ------> ADC1_IN15
        PB0     ------> ADC1_IN8
        PB1     ------> ADC1_IN9
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |
                               GPIO_PIN_5);

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |
                               GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_1);

        /* ADC1 DMA DeInit */
        HAL_DMA_DeInit(adcHandle->DMA_Handle);

        /* ADC1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
}
