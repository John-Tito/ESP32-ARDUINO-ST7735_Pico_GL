#include "ST7735s.h"
#include "Arduino.h"
#include "SPI.h"

ST7735sClass::ST7735sClass()
    : _spi(NULL), _spi_num(VSPI), _resetPin(0), _backLightPin(0), _chipSelectPin(0), _dcPin(0), X_MAX_PIXEL(0), Y_MAX_PIXEL(0), _currentArea({0, 0, 0, 0}), _currentPoint({0, 0})
{
}

ST7735sClass::ST7735sClass(uint8_t spi_num, uint8_t bl_pin, uint8_t cs_pin, uint8_t dc_pin, uint8_t rst_pin, uint16_t max_x, uint16_t max_y)
    : _spi(NULL), _spi_num(spi_num), _resetPin(rst_pin), _backLightPin(bl_pin), _chipSelectPin(cs_pin), _dcPin(dc_pin), X_MAX_PIXEL(max_x), Y_MAX_PIXEL(max_y), _currentArea({0, 0, max_x, max_x}), _currentPoint({0, 0})
{
}

inline void ST7735sClass::SPI_NSS_Begin()
{
    digitalWrite(_chipSelectPin, LOW);
}
inline void ST7735sClass::SPI_NSS_Stop()
{
    digitalWrite(_chipSelectPin, HIGH);
}
inline void ST7735sClass::LCD_DC_N(uint8_t d_cn)
{
    digitalWrite(_dcPin, d_cn);
}
inline void ST7735sClass::LCD_BL_Begin()
{
    digitalWrite(_backLightPin, HIGH);
}
inline void ST7735sClass::LCD_BL_Stop()
{
    digitalWrite(_backLightPin, LOW);
}
inline void ST7735sClass::LCD_RST_Begin()
{
    digitalWrite(_resetPin, LOW);
}
inline void ST7735sClass::LCD_RST_Stop()
{
    digitalWrite(_resetPin, HIGH);
}

//init the gpio and spi
void ST7735sClass::hw_init(void)
{
    if (-1 == _spi_num)
    {
        _spi_num = VSPI;
    }

    if (NULL == _spi)
    {
        _spi = new SPIClass(_spi_num);
    }
    _spi->begin();
    SPISettings settings = SPISettings(20000000, SPI_MSBFIRST, SPI_MODE3);
    _spi->beginTransaction(settings);

    pinMode(_dcPin, OUTPUT);
    pinMode(_chipSelectPin, OUTPUT); //chip select pin
    pinMode(_resetPin, OUTPUT);      //reset control pin
    pinMode(_backLightPin, OUTPUT);  //back light control pin
}

//reset the led
void ST7735sClass::Lcd_Reset(void)
{
    SPI_NSS_Stop();
    LCD_RST_Stop();
    delay_us(10);
    LCD_RST_Begin();
    delay_us(20);
    LCD_RST_Stop();
    delay_ms(120);
}

//read a byte data form lcd @unused
uint8_t ST7735sClass::Lcd_Read(uint8_t Addr)
{
    return 0;
}

//write a byte data or command to lcd
uint8_t ST7735sClass::Lcd_Write(uint8_t d_cn, uint8_t Data)
{
    SPI_NSS_Begin();
    LCD_DC_N(d_cn);
    _spi->write(Data);
    SPI_NSS_Stop();
    return 0;
}

//write two bytes data to lcd
void ST7735sClass::LCD_WriteData_16Bit(uint16_t Data)
{
    SPI_NSS_Begin();
    LCD_DC_N(LCD_DATA);
    _spi->write16(Data);
    SPI_NSS_Stop();
}

//write pixels value to lcd in two bytes
uint8_t ST7735sClass::Lcd_Write_Buffer(const uint16_t *Buffer, uint16_t Data_len)
{
    uint16_t i;
    SPI_NSS_Begin();
    LCD_DC_N(LCD_DATA);
    for (i = 0; i < Data_len; i++)
    {
        _spi->write16(Buffer[i]);
    }
    SPI_NSS_Stop();
    return 0;
}

uint8_t ST7735sClass::Lcd_Clear_Buffer(uint16_t Data, uint16_t Data_len)
{
    uint16_t i;
    SPI_NSS_Begin();
    LCD_DC_N(LCD_DATA);
    for (i = 0; i < Data_len; i++)
    {
        _spi->write16(Data);
    }
    SPI_NSS_Stop();
    return 0;
}

//write one byte command followed by one byte data to lcd @unused
void ST7735sClass::Lcd_WriteReg(uint8_t Index, uint8_t Data)
{
    Lcd_Write(LCD_CMD, Index);
    Lcd_Write(LCD_DATA, Data);
}

//LCD Init For 1.44Inch LCD Panel with ST7735R.
void ST7735sClass::Lcd_Init(void)
{
    hw_init();

    Lcd_Reset(); //Reset before LCD Init.

    LCD_BL_Stop();
    delay_ms(1000);

    //LCD Init For 1.44Inch LCD Panel with ST7735R.
    Lcd_Write(LCD_CMD, 0x11); //Sleep exit
    delay_ms(50);
    //Frame Ctrl Settings
    Lcd_Write(LCD_CMD, 0xB1);
    Lcd_Write(LCD_DATA, 0x05);
    Lcd_Write(LCD_DATA, 0x3A);
    Lcd_Write(LCD_DATA, 0x3A);

    Lcd_Write(LCD_CMD, 0xB2);
    Lcd_Write(LCD_DATA, 0x05);
    Lcd_Write(LCD_DATA, 0x3A);
    Lcd_Write(LCD_DATA, 0x3A);

    Lcd_Write(LCD_CMD, 0xB3);
    Lcd_Write(LCD_DATA, 0x05);
    Lcd_Write(LCD_DATA, 0x3A);
    Lcd_Write(LCD_DATA, 0x3A);
    Lcd_Write(LCD_DATA, 0x05);
    Lcd_Write(LCD_DATA, 0x3A);
    Lcd_Write(LCD_DATA, 0x3A);

    //Display inversion control
    // Lcd_Write(LCD_CMD, 0xB4);
    // Lcd_Write(LCD_DATA, 0x03);

    Lcd_Write(LCD_CMD, 0xC0);
    Lcd_Write(LCD_DATA, 0x62);
    Lcd_Write(LCD_DATA, 0x02);
    Lcd_Write(LCD_DATA, 0x04);

    Lcd_Write(LCD_CMD, 0xC1);
    Lcd_Write(LCD_DATA, 0xC0);

    Lcd_Write(LCD_CMD, 0xC2);
    Lcd_Write(LCD_DATA, 0x0D);
    Lcd_Write(LCD_DATA, 0x00);

    Lcd_Write(LCD_CMD, 0xC3);
    Lcd_Write(LCD_DATA, 0x8D);
    Lcd_Write(LCD_DATA, 0x6A);

    Lcd_Write(LCD_CMD, 0xC4);
    Lcd_Write(LCD_DATA, 0x8D);
    Lcd_Write(LCD_DATA, 0xEE);

    Lcd_Write(LCD_CMD, 0xC5); /*VCOM*/
    Lcd_Write(LCD_DATA, 0x0E);

    Lcd_Write(LCD_CMD, 0xE0);
    Lcd_Write(LCD_DATA, 0x10);
    Lcd_Write(LCD_DATA, 0x0E);
    Lcd_Write(LCD_DATA, 0x02);
    Lcd_Write(LCD_DATA, 0x03);
    Lcd_Write(LCD_DATA, 0x0E);
    Lcd_Write(LCD_DATA, 0x07);
    Lcd_Write(LCD_DATA, 0x02);
    Lcd_Write(LCD_DATA, 0x07);
    Lcd_Write(LCD_DATA, 0x0A);
    Lcd_Write(LCD_DATA, 0x12);
    Lcd_Write(LCD_DATA, 0x27);
    Lcd_Write(LCD_DATA, 0x37);
    Lcd_Write(LCD_DATA, 0x00);
    Lcd_Write(LCD_DATA, 0x0D);
    Lcd_Write(LCD_DATA, 0x0E);
    Lcd_Write(LCD_DATA, 0x10);

    Lcd_Write(LCD_CMD, 0xE1);
    Lcd_Write(LCD_DATA, 0x10);
    Lcd_Write(LCD_DATA, 0x0E);
    Lcd_Write(LCD_DATA, 0x03);
    Lcd_Write(LCD_DATA, 0x03);
    Lcd_Write(LCD_DATA, 0x0F);
    Lcd_Write(LCD_DATA, 0x06);
    Lcd_Write(LCD_DATA, 0x02);
    Lcd_Write(LCD_DATA, 0x08);
    Lcd_Write(LCD_DATA, 0x0A);
    Lcd_Write(LCD_DATA, 0x13);
    Lcd_Write(LCD_DATA, 0x26);
    Lcd_Write(LCD_DATA, 0x36);
    Lcd_Write(LCD_DATA, 0x00);
    Lcd_Write(LCD_DATA, 0x0D);
    Lcd_Write(LCD_DATA, 0x0E);
    Lcd_Write(LCD_DATA, 0x10);

    Lcd_Write(LCD_CMD, 0x3A);
    Lcd_Write(LCD_DATA, 0x05);

    Lcd_Write(LCD_CMD, 0x36);
    Lcd_Write(LCD_DATA, 0xA8);

    Lcd_Write(LCD_CMD, 0x29);

    LCD_BL_Begin();
}

//clear lcd with given color
void ST7735sClass::Lcd_Clear(uint16_t Color)
{
    Lcd_ClearRegion(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1, Color, X_MAX_PIXEL * Y_MAX_PIXEL);
}

//set display region
void ST7735sClass::Lcd_SetRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    Lcd_Write(LCD_CMD, 0x2a);
    LCD_WriteData_16Bit(x_start + 1);
    LCD_WriteData_16Bit(x_end + 1);

    Lcd_Write(LCD_CMD, 0x2b);
    LCD_WriteData_16Bit(y_start + 1);
    LCD_WriteData_16Bit(y_end + 1);

    Lcd_Write(LCD_CMD, 0x2c);
}

//draw the area using the given data
void ST7735sClass::Lcd_DrawRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, const uint16_t *color, uint16_t color_length)
{
    Lcd_SetRegion(x_start, y_start, x_end, y_end);
    Lcd_Write_Buffer(color, color_length);
}

//clear the area using the given data
void ST7735sClass::Lcd_ClearRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color, uint16_t color_length)
{
    Lcd_SetRegion(x_start, y_start, x_end, y_end);
    Lcd_Clear_Buffer(color, color_length);
}

//set the coordinate that next data will be write to
void ST7735sClass::Lcd_SetXY(uint16_t x, uint16_t y)
{
    Lcd_SetRegion(x, y, x, y);
}

//draw a point at the coordinate using the given data
void ST7735sClass::Lcd_DrawPoint(uint16_t x, uint16_t y, uint16_t Data)
{
    Lcd_SetRegion(x, y, x + 1, y + 1);
    LCD_WriteData_16Bit(Data);
}

//get pixel data of the coordinate
uint16_t ST7735sClass::Lcd_ReadPoint(uint16_t x, uint16_t y)
{
    return 0;
}
