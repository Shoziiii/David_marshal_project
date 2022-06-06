/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}
#endif

#if defined(spi_default) && defined(PICO_DEFAULT_SPI_CSN_PIN)
static void write_register(uint8_t reg, uint8_t data) {
    uint8_t buf[2];
    buf[0] = reg & 0x7f;  // remove read bit as this is a write
    buf[1] = data;
    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();
    sleep_ms(10);
}

#endif
int32_t num=0x17;
int main() {
    stdio_init_all();
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
#warning spi/bme280_spi example requires a board with SPI pins
    puts("Default SPI pins were not defined");
#else

   

    // This example will use SPI0 at 0.5MHz.
    spi_init(spi_default, 500 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS"));
/***************************************************************************************/
/*************************************MAX 7219 settings*********************************/
        
        write_register(0x0c,1); // normal mode
         sleep_ms(1000);
        write_register(0x0b ,0x01); // SCan 1 digits , We can use 0x00 to 0x07 depends on the number of numbers we want to use.
        sleep_ms(1000);
        write_register(0x09 ,0xFF); // Deode mode all
        sleep_ms(1000);
        write_register(0x0f,1);  // test display ON
        sleep_ms(1000);
        write_register(0x0f,0);  // test display OFF
        sleep_ms(1000);
        write_register(0x0a ,0x05); // brightness level 1
        sleep_ms(1000);
/***************************************************************************************/
    while (1) {
/*************************************For loop to dipslay from 9 to 0 ******************      
    for (int abc=9;abc>0;abc--)
    {
         write_register(0x01 ,abc); // 
        sleep_ms(1000);
    }
/***************************************************************************************/    

        write_register(0x01 ,0x00); // 
        write_register(0x02 ,0x00); // 
        sleep_ms(1000);
       write_register(0x01 ,0x01); // 
        sleep_ms(1000);
         write_register(0x01 ,0x02); // 
        sleep_ms(1000);
         write_register(0x01 ,0x03); // 
        sleep_ms(1000);
         write_register(0x01 ,0x04); // 
        sleep_ms(1000);
        write_register(0x01 ,0x05); // 
        sleep_ms(1000);
        write_register(0x01 ,0x06); // 
        sleep_ms(1000);
        write_register(0x01 ,0x07); // 
        sleep_ms(1000);
        write_register(0x01 ,0x08); // 
        sleep_ms(1000);
        write_register(0x01 ,0x09); // 
        sleep_ms(1000);
        write_register(0x02 ,0x01); //
        write_register(0x01 ,0x00); //  
        sleep_ms(1000);
        
    }

    return 0;
#endif
}
