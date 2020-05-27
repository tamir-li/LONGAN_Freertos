/*
 * bsp.c
 *
 *  Created on: 2020Äê5ÔÂ26ÈÕ
 *      Author: li
 */

#include "bsp.h"
#include "systick.h"

void bsp_Init(void)
{
	nano_board_rgb_led_init();
	//nano_board_led_on(LED_G);

    Lcd_Init();			// init OLED
    LCD_Clear(YELLOW);
    set_Background_color(YELLOW);

    LCD_ShowString(36, 16, (u8 *)("Hello World!"), BLUE);
    LCD_ShowString(36, 32, (u8 *)("OS start"), BLUE);
    LCD_ShowNum_int(36,48,666,3,BLUE);
    delay_1ms(1000);

    LCD_ShowString(36, 32, (u8 *)("OS start."), BLUE);
    delay_1ms(1000);
    LCD_ShowString(36, 32, (u8 *)("OS start.."), BLUE);
    delay_1ms(1000);
    LCD_ShowString(36, 32, (u8 *)("OS start..."), BLUE);
    delay_1ms(1000);
    LCD_ShowString(36, 32, (u8 *)("OS start...."), BLUE);
    delay_1ms(1000);

    set_Background_color(WHITE);
    LCD_Clear(WHITE);
}





