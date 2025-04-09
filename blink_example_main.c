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

void bitmap_or(unsigned char* dst, const unsigned char* src, size_t size) {
    for (size_t i = 0; i < size; i++) {
        dst[i] |= src[i];
    }
}

void app_main(void)
{
    HUB75E_Init();
    HUB75E_setDisplayBrightness(BrightnessLevel1); //1-5 (4 is highest that works)
    //HUB75E_setDisplayColor(Red); //White, Red, Blue, Cyan, Yellow

    gpio_reset_pin(13);
    gpio_set_direction(13, GPIO_MODE_OUTPUT);
    gpio_set_level(13, 1);

    gpio_reset_pin(22);
    gpio_set_direction(22, GPIO_MODE_OUTPUT);
    gpio_set_level(22, 1);

    int count = 0;
    int num = 0;
    int max = 1200;
    int size = 1024;
    int IN_GAME = 0;
    int MODE = 0;

    
    //unsigned char bitmap_scoreboard[1024];
    unsigned char* bitmap_m_scoreboard = (unsigned char*)malloc(size * sizeof(unsigned char)); 
    memset(bitmap_m_scoreboard, 0, size * sizeof(unsigned char));

    bitmap_or(bitmap_m_scoreboard, bitmap_standard, size);

    while (1) {

        //if(count == 0) IN_GAME = 0;
        //else if(count == 160) IN_GAME = 1;
        //MODE = count / 40;
        MODE = 4;
        IN_GAME = 0;
        
        num = (count - 160) / 40;

        memset(bitmap_m_scoreboard, 0, size * sizeof(unsigned char));
            
        if(IN_GAME == 1) {

            //LEFT BIG LEFT
            if(num >= 10) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_LX, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_LY, size);
            }

            //LEFT BIG RIGHT
            if(num % 10 == 0) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RG, size);
            }
            else if(num % 10 == 1) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RF, size);
            }
            else if(num % 10 == 2) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RG, size);
            }
            else if(num % 10 == 3) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RG, size);
            }
            else if(num % 10 == 4) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RF, size);
            }
            else if(num % 10 == 5) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RG, size);
            }
            else if(num % 10 == 6) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RG, size);
            }
            else if(num % 10 == 7) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RF, size);
            }
            else if(num % 10 == 8) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RG, size);
            }
            else if(num % 10 == 9) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LB_RF, size);
            }

    
            //LEFT UPPER LEFT
            //Nothing on 0
            if((num / 10) % 10 == 1) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LF, size);
            }
            else if((num / 10) % 10 == 2) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LG, size);
            }
            else if((num / 10) % 10 == 3) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LG, size);
            }
            else if((num / 10) % 10 == 4) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LF, size);
            }
            else if((num / 10) % 10 == 5) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LG, size);
            }
            else if((num / 10) % 10 == 6) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LG, size);
            }
            else if((num / 10) % 10 == 7) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LF, size);
            }
            else if((num / 10) % 10 == 8) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LG, size);
            }
            else if((num / 10) % 10 == 9) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_LF, size);
            }

            //LEFT UPPER RIGHT
            if(num % 10 == 0) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RG, size);
            }
            else if(num % 10 == 1) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RF, size);
            }
            else if(num % 10 == 2) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RG, size);
            }
            else if(num % 10 == 3) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RG, size);
            }
            else if(num % 10 == 4) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RF, size);
            }
            else if(num % 10 == 5) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RG, size);
            }
            else if(num % 10 == 6) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RG, size);
            }
            else if(num % 10 == 7) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RF, size);
            }
            else if(num % 10 == 8) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RG, size);
            }
            else if(num % 10 == 9) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LU_RF, size);
            }

            //LEFT LOWER LEFT
            //Nothing on 0
            if((num / 10) % 10 == 1) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LF, size);
            }
            else if((num / 10) % 10 == 2) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LG, size);
            }
            else if((num / 10) % 10 == 3) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LG, size);
            }
            else if((num / 10) % 10 == 4) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LF, size);
            }
            else if((num / 10) % 10 == 5) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LG, size);
            }
            else if((num / 10) % 10 == 6) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LG, size);
            }
            else if((num / 10) % 10 == 7) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LF, size);
            }
            else if((num / 10) % 10 == 8) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LG, size);
            }
            else if((num / 10) % 10 == 9) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_LF, size);
            }

            //LEFT LOWER RIGHT
            if(num % 10 == 0) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RG, size);
            }
            else if(num % 10 == 1) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RF, size);
            }
            else if(num % 10 == 2) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RG, size);
            }
            else if(num % 10 == 3) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RG, size);
            }
            else if(num % 10 == 4) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RF, size);
            }
            else if(num % 10 == 5) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RG, size);
            }
            else if(num % 10 == 6) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RG, size);
            }
            else if(num % 10 == 7) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RF, size);
            }
            else if(num % 10 == 8) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RG, size);
            }
            else if(num % 10 == 9) {
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_LL_RF, size);
            }
                    
            //RIGHT BIG LEFT
            if(num >= 10) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_LX, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_LY, size);
            }

            //RIGHT BIG RIGHT
            if(num % 10 == 0) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RG, size);
            }
            else if(num % 10 == 1) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RF, size);
            }
            else if(num % 10 == 2) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RG, size);
            }
            else if(num % 10 == 3) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RG, size);
            }
            else if(num % 10 == 4) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RF, size);
            }
            else if(num % 10 == 5) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RG, size);
            }
            else if(num % 10 == 6) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RG, size);
            }
            else if(num % 10 == 7) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RF, size);
            }
            else if(num % 10 == 8) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RG, size);
            }
            else if(num % 10 == 9) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RB_RF, size);
            }

            //RIGHT UPPER LEFT
            //Nothing on 0
            if((num / 10) % 10 == 1) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LF, size);
            }
            else if((num / 10) % 10 == 2) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LG, size);
            }
            else if((num / 10) % 10 == 3) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LG, size);
            }
            else if((num / 10) % 10 == 4) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LF, size);
            }
            else if((num / 10) % 10 == 5) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LG, size);
            }
            else if((num / 10) % 10 == 6) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LG, size);
            }
            else if((num / 10) % 10 == 7) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LF, size);
            }
            else if((num / 10) % 10 == 8) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LG, size);
            }
            else if((num / 10) % 10 == 9) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_LF, size);
            }

            //RIGHT UPPER RIGHT
            if(num % 10 == 0) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RG, size);
            }
            else if(num % 10 == 1) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RF, size);
            }
            else if(num % 10 == 2) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RG, size);
            }
            else if(num % 10 == 3) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RG, size);
            }
            else if(num % 10 == 4) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RF, size);
            }
            else if(num % 10 == 5) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RG, size);
            }
            else if(num % 10 == 6) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RG, size);
            }
            else if(num % 10 == 7) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RF, size);
            }
            else if(num % 10 == 8) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RG, size);
            }
            else if(num % 10 == 9) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RU_RF, size);
            }

            //RIGHT LOWER LEFT
            //Nothing on 0
            if((num / 10) % 10 == 1) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LF, size);
            }
            else if((num / 10) % 10 == 2) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LG, size);
            }
            else if((num / 10) % 10 == 3) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LG, size);
            }
            else if((num / 10) % 10 == 4) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LF, size);
            }
            else if((num / 10) % 10 == 5) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LG, size);
            }
            else if((num / 10) % 10 == 6) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LG, size);
            }
            else if((num / 10) % 10 == 7) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LF, size);
            }
            else if((num / 10) % 10 == 8) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LG, size);
            }
            else if((num / 10) % 10 == 9) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_LF, size);
            }

            //RIGHT LOWER RIGHT
            if(num % 10 == 0) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RG, size);
            }
            else if(num % 10 == 1) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RF, size);
            }
            else if(num % 10 == 2) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RG, size);
            }
            else if(num % 10 == 3) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RG, size);
            }
            else if(num % 10 == 4) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RF, size);
            }
            else if(num % 10 == 5) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RG, size);
            }
            else if(num % 10 == 6) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RG, size);
            }
            else if(num % 10 == 7) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RF, size);
            }
            else if(num % 10 == 8) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RE, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RF, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RG, size);
            }
            else if(num % 10 == 9) {
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RA, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RB, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RC, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RD, size);
                bitmap_or(bitmap_m_scoreboard, bitmap_RL_RF, size);
            }
      
        }
        else {
            if(MODE == 0) {
                memcpy(bitmap_m_scoreboard, bitmap_standard, 1024);
            }
            else if(MODE == 1) {
                memcpy(bitmap_m_scoreboard, bitmap_21_exact, 1024);
            }
            else if(MODE == 2) {
                memcpy(bitmap_m_scoreboard, bitmap_hole_only, 1024);
            }
            else if(MODE == 3) {
                memcpy(bitmap_m_scoreboard, bitmap_board_only, 1024);
            }
            else if(MODE == 4) {
                memcpy(bitmap_m_scoreboard, bitmap_Game_Over, 1024);
                    
                //GAME OVER RED LEFT
                if((num / 10) % 10 == 0) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_G, size);
                }
                else if((num / 10) % 10 == 1) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_F, size);
                }
                else if((num / 10) % 10 == 2) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_G, size);
                }
                else if((num / 10) % 10 == 3) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_G, size);
                }
                else if((num / 10) % 10 == 4) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_F, size);
                }
                else if((num / 10) % 10 == 5) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_G, size);
                }
                else if((num / 10) % 10 == 6) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_G, size);
                }
                else if((num / 10) % 10 == 7) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_F, size);
                }
                else if((num / 10) % 10 == 8) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_G, size);
                }
                else if((num / 10) % 10 == 9) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RL_F, size);
                }

                //GAME OVER RED RIGHT
                if(num % 10 == 0) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_G, size);
                }
                else if(num % 10 == 1) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_F, size);
                }
                else if(num % 10 == 2) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_G, size);
                }
                else if(num % 10 == 3) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_G, size);
                }
                else if(num % 10 == 4) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_F, size);
                }
                else if(num % 10 == 5) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_G, size);
                }
                else if(num % 10 == 6) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_G, size);
                }
                else if(num % 10 == 7) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_F, size);
                }
                else if(num % 10 == 8) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_G, size);
                }
                else if(num % 10 == 9) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_RR_F, size);
                }

                //GAME OVER BLUE LEFT
                if((num / 10) % 10 == 0) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_G, size);
                }
                else if((num / 10) % 10 == 1) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_F, size);
                }
                else if((num / 10) % 10 == 2) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_G, size);
                }
                else if((num / 10) % 10 == 3) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_G, size);
                }
                else if((num / 10) % 10 == 4) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_F, size);
                }
                else if((num / 10) % 10 == 5) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_G, size);
                }
                else if((num / 10) % 10 == 6) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_G, size);
                }
                else if((num / 10) % 10 == 7) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_F, size);
                }
                else if((num / 10) % 10 == 8) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_G, size);
                }
                else if((num / 10) % 10 == 9) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BL_F, size);
                }

                //GAME OVER BLUE RIGHT
                if(num % 10 == 0) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_G, size);
                }
                else if(num % 10 == 1) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_F, size);
                }
                else if(num % 10 == 2) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_G, size);
                }
                else if(num % 10 == 3) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_G, size);
                }
                else if(num % 10 == 4) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_F, size);
                }
                else if(num % 10 == 5) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_G, size);
                }
                else if(num % 10 == 6) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_G, size);
                }
                else if(num % 10 == 7) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_F, size);
                }
                else if(num % 10 == 8) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_E, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_F, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_G, size);
                }
                else if(num % 10 == 9) {
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_A, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_B, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_C, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_D, size);
                    bitmap_or(bitmap_m_scoreboard, bitmap_BR_F, size);
                }
            
            }
        }

        HUB75E_setDisplayBuffer(bitmap_m_scoreboard);
        HUB75E_displayBufferPixels(IN_GAME, MODE); //must call every loop

        count += 1;
        if(count > max) count = 0;
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    free(bitmap_m_scoreboard);



}
