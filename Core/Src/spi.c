#include "spi.h"

SPI_HandleTypeDef hspi2;

void MX_SPI_Init(void)
{
    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(hspi->Instance == SPI2)
    {
        __HAL_RCC_SPI2_CLK_ENABLE();
        __HAL_RCC_GPIOI_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**SPI2 GPIO Configuration
        PI1     ------> SPI2_SCK
        PB14     ------> SPI2_MISO
        PB15     ------> SPI2_MOSI
        PI0     ------> SPI2_NSS
        */

        GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;

        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);

        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_Delay(10);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if(hspi->Instance == SPI2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
        PI1     ------> SPI2_SCK
        PB14     ------> SPI2_MISO
        PB15     ------> SPI2_MOSI
        PI0     ------> SPI2_NSS
        */
    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_0);

    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_1);
    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_1);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_15);

  }
}

uint8_t rxTemp[2]= {0};
uint8_t testLoRa()
{
    uint8_t reg[2] = {0x42, 0x42};
    rxTemp[0] = 0;
    rxTemp[1] = 0;
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET);
    // if(HAL_SPI_TransmitReceive(&hspi2, reg, rxTemp, 2, 100) != HAL_OK)
    // {
    //     asm("bkpt #1");
    // }
    // if(HAL_SPI_Receive(&hspi2, &rxTemp, 1, 1000) != HAL_OK)
    // {
    //     asm("bkpt #1");
    // }
    if(HAL_SPI_Transmit(&hspi2, reg, 1, 1000) != HAL_OK)
    {
        asm("bkpt #1");
    }
    if(HAL_SPI_Receive(&hspi2, rxTemp, 1, 1000) != HAL_OK)
    {
        asm("bkpt #1");
    }
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);
    return rxTemp[1];
}
