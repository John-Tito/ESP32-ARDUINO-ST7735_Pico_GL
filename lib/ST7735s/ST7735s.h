/*
  ST7735s.h - ST7735s driver
*/
#ifndef _ST7735_H_INCLUDED
#define _ST7735_H_INCLUDED

#include "SPI.h"

#define delay_us(us) delayMicroseconds(us)
#define delay_ms(ms) delay(ms)
/**
 * Represents a point on the screen.
 */
typedef struct
{
    uint16_t x;
    uint16_t y;
} lcd_point_t;

/** Represents an area of the screen.*/
typedef struct
{
    uint16_t y1;
    uint16_t x1;
    uint16_t x2;
    uint16_t y2;
} lcd_area_t;

enum ST7735s_DataType_t
{

    LCD_DATA = 1,
    LCD_CMD = 0
};

enum ST7735s_Color_t
{
    RED = 0xf800,
    GREEN = 0x07e0,
    BLUE = 0x001f,
    WHITE = 0xffff,
    BLACK = 0x0000,
    YELLOW = 0xFFE0,
    GRAY0 = 0xEF7D,
    GRAY2 = 0x4208
};
class ST7735sClass
{
private:
    SPIClass *_spi;
    uint8_t _spi_num;
    uint8_t _resetPin;
    uint8_t _backLightPin;
    uint8_t _chipSelectPin;
    uint8_t _dcPin;
    uint16_t X_MAX_PIXEL;
    uint16_t Y_MAX_PIXEL;
    lcd_area_t _currentArea;
    lcd_point_t _currentPoint;
    void drive_type();
    void backLight_on();   //BL=1
    void backLight_off();  //BL=0
    void reset_asssert();  //RST=0
    void reset_deassert(); //RST=1
    void chip_select();    //CS=0
    void chip_diselect();  //CS=1
    void hw_init();
    void Lcd_Reset();
    uint8_t Lcd_Read(uint8_t Addr);
    uint8_t Lcd_Write(uint8_t d_cn, uint8_t Data);
    void LCD_WriteData_16Bit(uint16_t Data);
    uint8_t Lcd_Write_Buffer(const uint16_t *Buffer, uint16_t Data_len);
    uint8_t Lcd_Clear_Buffer(uint16_t Data, uint16_t Data_len);
    void Lcd_WriteReg(uint8_t Index, uint8_t Data);
    void SPI_NSS_Begin();
    void SPI_NSS_Stop();
    void LCD_DC_N(uint8_t d_cn);
    void LCD_BL_Begin();
    void LCD_BL_Stop();
    void LCD_RST_Begin();
    void LCD_RST_Stop();

public:
    ST7735sClass();
    ST7735sClass(uint8_t spi_num, uint8_t bl_pin, uint8_t cs_pin, uint8_t dc_pin, uint8_t rst_pin, uint16_t max_x, uint16_t max_y);
    void Lcd_Init(void);
    void Lcd_Clear(uint16_t Color);
    void Lcd_SetRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);
    void Lcd_SetXY(uint16_t x, uint16_t y);
    void Lcd_DrawPoint(uint16_t x, uint16_t y, uint16_t Data);
    uint16_t Lcd_ReadPoint(uint16_t x, uint16_t y);
    void Lcd_DrawRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, const uint16_t *color, uint16_t color_length);
    void Lcd_ClearRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color, uint16_t color_length);
};

#endif
