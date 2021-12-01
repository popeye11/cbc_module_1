/* *
 * @file spi.cpp
 * @brief customized low level SPI start and transmit function.
 * @copyright (c) 2021 TOPTICA Photonics AG
 * @author Xiaodong Zhang <xiaodong.zhang@toptica.com>
 * @date 2019-09-14
 */
#include "spi.hpp"
#include "peripherals.hpp"
#include <stm32h743xx.h>
#include <cstdint>
#include <iostream>
SPI::SPI(SPI_TypeDef* spi) : _spi(spi){};
void SPI::init()
{
    _spi->CR1 |= SPI_CR1_SPE_Msk;    // enable SPI
    _spi->CR1 |= SPI_CR1_CSTART_Msk; // master transfer start
}
void SPI::transmit(uint16_t data, uint8_t channel)
{
    __IO uint16_t* ptxdr5_16bits = (uint16_t*)(&_spi->TXDR); // for 16-bit-write
    const uint32_t dac80004Cmd   = 0                         // mode bits, here don't care
        | ((uint32_t)data << 4)                              // DAC Data
        | ((uint32_t)channel << 20)                          // Channel address 0
        | ((uint32_t)0x3 << 24)                              // Cmd: Write to buffer and update DAC
        ;
    SIGNAL_OUTPUT_PORT->BSRR = SIGNAL_OUTPUT_PIN << 16; // Reset
    *ptxdr5_16bits           = dac80004Cmd >> 16;
    while (!(_spi->SR & SPI_SR_TXC_Msk)) {
    } // check if FiFo transmission complete
    *ptxdr5_16bits = dac80004Cmd & 0xffff;
    while (!(_spi->SR & SPI_SR_TXC_Msk)) {
        // check if FiFo transmission complete
    };
    SIGNAL_OUTPUT_PORT->BSRR = SIGNAL_OUTPUT_PIN; // Set
}
void SPI::transmit(uint16_t data)
{
    __IO uint16_t* _ptxdr_16bits = (uint16_t*)(&_spi->TXDR); // for 16-bit-write
    *_ptxdr_16bits               = data;
    while (!(_spi->SR & SPI_SR_TXC_Msk)) {
        // check if FiFo transmission complete
    }
}