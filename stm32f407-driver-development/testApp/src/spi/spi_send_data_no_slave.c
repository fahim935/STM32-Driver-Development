/*******************************************************************************
 * @file           spi_send_data_no_slave.c
 * @brief          Brief description
 * 
 * @author         Fahim Akhtar
 * @date           26-Oct-2025
 * @time           11:45:21 AM
 * @version        1.0
 * @copyright      (c) 2025
 ******************************************************************************/

#include <spi_send_data_no_slave.h>
#if (FEATURE_SELECTED == STM32_SPI2_SEND_DATA_WITH_NO_SLAVE)
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
#include <string.h>

static void SPI2_GPIOInits(void){

    /*
     * PB12 : SPI2_NSS
     * PB13 : SPI2_SCLK
     * PB14 : SPI2_MISO
     * PB15 : SPI2_MOSI
     * ALT Function mode : 5
     *
     * */

    GPIO_Handle_t SPIPins;

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // Configure the NSS
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIO_Init(&SPIPins);

    // Configure the SCLK
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    // As in this application we are not using slave so we can disable below pin

    // Configure the MISO
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    GPIO_Init(&SPIPins);

    // Configure the MOSI
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&SPIPins);

}


static void SPI2_inits(void) {

    SPI_Handle_t SPI2handle;

    SPI2handle.pSPIx = SPI2;
    SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MASTER_MODE;
    //SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
    SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
    SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

    SPI_Init(&SPI2handle);
}

void send_data_over_spi2(void){

    char user_data[] = "Hello SPI Data";

    SPI2_GPIOInits();

    SPI2_inits();

    SPI_SSIConfig(SPI2, ENABLE);

    // enable the SPI2 Peripheral
    SPI_PeripheralControl(SPI2, ENABLE);

    //to send data
    SPI_SendData_Polling(SPI2, (uint8_t *)user_data, strlen(user_data));

    // Let confirm SPI is not busy
    while(SPI_GetFlagStatus(SPI2, SPI_BSY_FLAG));

    // Disable the SPI2 Peripheral
    SPI_PeripheralControl(SPI2, ENABLE);
}

#endif
