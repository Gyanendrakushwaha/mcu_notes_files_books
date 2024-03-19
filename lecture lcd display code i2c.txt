#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"

#define LCD_RS_PIN    GPIO_PIN_0  // RS pin connected to GPIO pin 0
#define LCD_RW_PIN    GPIO_PIN_1  // RW pin connected to GPIO pin 1
#define LCD_EN_PIN    GPIO_PIN_2  // EN pin connected to GPIO pin 2
#define LCD_DATA_PIN4 GPIO_PIN_4  // D4 pin connected to GPIO pin 4
#define LCD_DATA_PIN5 GPIO_PIN_5  // D5 pin connected to GPIO pin 5
#define LCD_DATA_PIN6 GPIO_PIN_6  // D6 pin connected to GPIO pin 6
#define LCD_DATA_PIN7 GPIO_PIN_7  // D7 pin connected to GPIO pin 7

#define LCD_GPIO_PORT GPIOA  // GPIO port for LCD

void LCD_Init(void);
void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);
void LCD_SendString(const char *str);

int main(void)
{
    HAL_Init();
    LCD_Init();

    while (1)
    {
        LCD_SendString("Hello, STM32F0!");
        HAL_Delay(1000);
        LCD_Command(0x01);  // Clear display
        HAL_Delay(1000);
    }
}

void LCD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure GPIO pins as output
    GPIO_InitStruct.Pin = LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN | LCD_DATA_PIN4 | LCD_DATA_PIN5 | LCD_DATA_PIN6 | LCD_DATA_PIN7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_GPIO_PORT, &GPIO_InitStruct);

    // Initialization sequence
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_RW_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_DATA_PIN4 | LCD_DATA_PIN5 | LCD_DATA_PIN6 | LCD_DATA_PIN7, GPIO_PIN_RESET);
    HAL_Delay(40);

    LCD_Command(0x03);
    HAL_Delay(5);

    LCD_Command(0x03);
    HAL_Delay(5);

    LCD_Command(0x03);
    HAL_Delay(5);

    LCD_Command(0x02);

    LCD_Command(0x28);  // 4-bit data, 2-line display, 5x8 font
    LCD_Command(0x08);  // Display off
    LCD_Command(0x01);  // Clear display
    LCD_Command(0x06);  // Entry mode set: increment cursor, no shift
    LCD_Command(0x0C);  // Display on, cursor off, blink off
}

void LCD_Command(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_RW_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_DATA_PIN4 | LCD_DATA_PIN5 | LCD_DATA_PIN6 | LCD_DATA_PIN7, (cmd >> 4) & 0x0F);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_DATA_PIN4 | LCD_DATA_PIN5 | LCD_DATA_PIN6 | LCD_DATA_PIN7, cmd & 0x0F);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_RESET);

    HAL_Delay(1);  // Delay for command execution
}

void LCD_Data(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_RS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_RW_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_DATA_PIN4 | LCD_DATA_PIN5 | LCD_DATA_PIN6 | LCD_DATA_PIN7, (data >> 4) & 0x0F);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_DATA_PIN4 | LCD_DATA_PIN5 | LCD_DATA_PIN6 | LCD_DATA_PIN7, data & 0x0F);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_EN_PIN, GPIO_PIN_RESET);

    HAL_Delay(1);  // Delay for data write
}

void LCD_SendString(const char *str)
{
    while (*str)
    {
        LCD_Data(*str++);
    }
}
