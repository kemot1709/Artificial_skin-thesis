#include <tim.h>
#include "usart.h"


UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

uint8_t uart_input_time_flag;
uint8_t uart_input_flag;
static uint8_t input_buff[UART_IN_BUFF_SIZE];
uint8_t input_buff_length;
uint8_t input_char;


/* USART1 init function */
void MX_USART1_UART_Init() {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
}

/* USART3 init function */
void MX_USART3_UART_Init(void) {
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK) {
        Error_Handler();
    }

    HAL_UART_Receive_IT(&huart3, &input_char, 1);
    uart_input_flag = 0;
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (uartHandle->Instance == USART1) {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* USART1 clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    } else if (uartHandle->Instance == USART3) {
        /* USER CODE BEGIN USART3_MspInit 0 */

        /* USER CODE END USART3_MspInit 0 */
        /* USART3 clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**USART3 GPIO Configuration
        PC10     ------> USART3_TX
        PC11     ------> USART3_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        __HAL_AFIO_REMAP_USART3_PARTIAL();

        /* USART3 interrupt Init */
        HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
        /* USER CODE BEGIN USART3_MspInit 1 */

        /* USER CODE END USART3_MspInit 1 */
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle) {
    if (uartHandle->Instance == USART1) {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

        /* USART1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    } else if (uartHandle->Instance == USART3) {
        /* USER CODE BEGIN USART3_MspDeInit 0 */

        /* USER CODE END USART3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART3 GPIO Configuration
        PC10     ------> USART3_TX
        PC11     ------> USART3_RX
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10 | GPIO_PIN_11);

        /* USART3 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART3_IRQn);
        /* USER CODE BEGIN USART3_MspDeInit 1 */

        /* USER CODE END USART3_MspDeInit 1 */
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    UNUSED(huart);
    HAL_GPIO_WritePin(LED_ERROR_Port, LED_ERROR_Pin, GPIO_PIN_SET);

    /** Determine state of buffer and interprete got sign */
    if (input_char == ASCII_STX && (uart_input_flag == 0 || uart_input_time_flag > 1)) {
        /** Start bit - begin getting message
         * Start when last message was proper or we had long waiting
         * */
        uart_input_flag = 1;
        input_buff_length = 0;
        uart_input_time_flag = 0;
    } else if (input_buff_length == UART_MESAGE_LEN && input_char == ASCII_ETX &&
               uart_input_flag == 1) {
        /** End message bit - interprete message */
        message_interpret(input_buff, input_buff_length);
        uart_input_flag = 0;
    } else if (uart_input_flag == 1) {
        input_buff[input_buff_length] = input_char;
        input_buff_length++;
    }

    HAL_UART_Receive_IT(&huart3, &input_char, 1);
    HAL_GPIO_WritePin(LED_ERROR_Port, LED_ERROR_Pin, GPIO_PIN_RESET);
}

uint8_t message_interpret(const uint8_t *buffer, uint8_t length) {
    UNUSED(length);
    switch (buffer[0]) {
        /** Save to memory incoming tactile config */
        case FIELD_TYPE_TACTILE:
            if (!first_tactile_ptr) {
                first_tactile_ptr = field_config_fulfilment;
            }
            write8_add(buffer);
            break;
        case FIELD_TYPE_TEMPERATURE:
            if (!first_temperature_ptr) {
                first_temperature_ptr = field_config_fulfilment;
            }
            write8_add(buffer);
            break;
        case FIELD_TYPE_ERASE_PAGES:
            field_config_flag = 0;
            field_config_fulfilment = EEPROM_FIELD_CONFIG_START;
            erase_FLASH(EEPROM_FIELD_CONFIG_START, EEPROM_FIELD_PAGES);
            break;
        case FIELD_TYPE_CONFIG_READY:
            field_config_flag = 1;
            break;
        default:
            break;
    }
    return 0;
}

uint8_t write8_add(uint8_t *buffer) {
    // Check if we can have place in memory to save config
    if (field_config_fulfilment >=
        EEPROM_FIELD_CONFIG_START + (EEPROM_PAGE_SIZE * EEPROM_FIELD_PAGES)) {
        return 1;
    }

    // Write to memory
    uint64_t *ptr = (uint64_t *) buffer;
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, field_config_fulfilment, *ptr);
    field_config_fulfilment += EEPROM_FIELD_SIZE;
    return 0;
}
