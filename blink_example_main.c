/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "bitmaps.h"
/*
#include "bitmap_hole_only.h"
#include "bitmap_board_only.h"
#include "bitmap_standard.h"
#include "bitmap_21_exact.h"
*/

#include "hub75e/HUB75ELib.h"

void app_main(void)
{
    HUB75E_Init();
    HUB75E_setDisplayBrightness(BrightnessLevel1); //1-5 (4 is highest that works)
    HUB75E_setDisplayColor(Red); //White, Red, Blue, Cyan, Yellow

    gpio_reset_pin(13);
    gpio_set_direction(13, GPIO_MODE_OUTPUT);
    gpio_set_level(13, 1);

    gpio_reset_pin(22);
    gpio_set_direction(22, GPIO_MODE_OUTPUT);
    gpio_set_level(22, 1);

    int count = 0;
    int max = 200;
    int size = 1024;
    unsigned char bitmap_scoreboard[1024];

    /*
    unsigned char* bitmap_m_standard = (unsigned char*)malloc(size * sizeof(unsigned char)); 
    memcpy(bitmap_m_standard, bitmap_standard, size);

    unsigned char* bitmap_m_21_exact = (unsigned char*)malloc(size * sizeof(unsigned char)); 
    memcpy(bitmap_m_21_exact, bitmap_21_exact, size);

    unsigned char* bitmap_m_hole_only = (unsigned char*)malloc(size * sizeof(unsigned char)); 
    memcpy(bitmap_m_hole_only, bitmap_hole_only, size);

    unsigned char* bitmap_m_board_only = (unsigned char*)malloc(size * sizeof(unsigned char)); 
    memcpy(bitmap_m_board_only, bitmap_board_only, size);
    */

    //combine header files with hardcoded images
    //don't use malloc for hardcoded stuff, read directly from header

    
    memcpy(bitmap_scoreboard, bitmap_21_exact, 1024);
    HUB75E_setDisplayBuffer(bitmap_scoreboard);

    while (1) {
        //count > max / 2 ? HUB75E_setDisplayColor(Red) : HUB75E_setDisplayColor(Green); //White, Red, Blue, Cyan, Yellow
        
        if(count == 0) {
            //memcpy(bitmap_final, bitmap_standard, 1024);
            HUB75E_setDisplayBuffer(bitmap_standard);
        }
        else if (count == 50) {
            //memcpy(bitmap_final, bitmap_21_exact, 1024);
            HUB75E_setDisplayBuffer(bitmap_21_exact);
        }
        else if (count == 100) {
            //memcpy(bitmap_final, bitmap_hole_only, 1024);
            HUB75E_setDisplayBuffer(bitmap_hole_only);

        }
        else if (count == 150) {
            //memcpy(bitmap_final, bitmap_board_only, 1024);
            HUB75E_setDisplayBuffer(bitmap_board_only);
        }
        
        HUB75E_displayBufferPixels(); //must call every loop

        count += 1;
        if(count > max) count = 0;
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    /*
    free(bitmap_m_21_exact);
    free(bitmap_m_standard);
    free(bitmap_m_hole_only);
    free(bitmap_m_board_only);
    */



}
