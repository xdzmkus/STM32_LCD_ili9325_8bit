
/*
 * lcd_io.c
 *
 */

#include "lcd_io.h"
#include "main.h"

static inline void LCD_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  if(PinState != GPIO_PIN_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    GPIOx->BSRR = (uint32_t)GPIO_Pin << 16U;
  }
}

static inline GPIO_PinState LCD_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  if((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
  {
    return GPIO_PIN_SET;
  }
  return GPIO_PIN_RESET;
}

static inline void LCD_DB_WRITE(uint8_t Data)
{
	LCD_GPIO_WritePin(LCD_DB0_GPIO_Port, LCD_DB0_Pin, (Data & 1));
	LCD_GPIO_WritePin(LCD_DB1_GPIO_Port, LCD_DB1_Pin, (Data >> 1) & 1);
	LCD_GPIO_WritePin(LCD_DB2_GPIO_Port, LCD_DB2_Pin, (Data >> 2) & 1);
	LCD_GPIO_WritePin(LCD_DB3_GPIO_Port, LCD_DB3_Pin, (Data >> 3) & 1);
	LCD_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, (Data >> 4) & 1);
	LCD_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, (Data >> 5) & 1);
	LCD_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, (Data >> 6) & 1);
	LCD_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, (Data >> 7) & 1);
}

static inline uint8_t LCD_DB_READ()
{
	struct DATA_BYTE {
	    unsigned int bit0 : 1;
	    unsigned int bit1 : 1;
	    unsigned int bit2 : 1;
	    unsigned int bit3 : 1;
	    unsigned int bit4 : 1;
	    unsigned int bit5 : 1;
	    unsigned int bit6 : 1;
	    unsigned int bit7 : 1;
	} Data;

	Data.bit0 = LCD_GPIO_ReadPin(LCD_DB0_GPIO_Port, LCD_DB0_Pin);
	Data.bit1 = LCD_GPIO_ReadPin(LCD_DB1_GPIO_Port, LCD_DB1_Pin);
	Data.bit2 = LCD_GPIO_ReadPin(LCD_DB2_GPIO_Port, LCD_DB2_Pin);
	Data.bit3 = LCD_GPIO_ReadPin(LCD_DB3_GPIO_Port, LCD_DB3_Pin);
	Data.bit4 = LCD_GPIO_ReadPin(LCD_DB4_GPIO_Port, LCD_DB4_Pin);
	Data.bit5 = LCD_GPIO_ReadPin(LCD_DB5_GPIO_Port, LCD_DB5_Pin);
	Data.bit6 = LCD_GPIO_ReadPin(LCD_DB6_GPIO_Port, LCD_DB6_Pin);
	Data.bit7 = LCD_GPIO_ReadPin(LCD_DB7_GPIO_Port, LCD_DB7_Pin);

	return *(uint8_t*)&Data;
}

static inline void LCD_DB_Config_Input()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin = LCD_DB0_Pin;
	HAL_GPIO_Init(LCD_DB0_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB1_Pin;
	HAL_GPIO_Init(LCD_DB1_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB2_Pin;
	HAL_GPIO_Init(LCD_DB2_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB3_Pin;
	HAL_GPIO_Init(LCD_DB3_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB4_Pin;
	HAL_GPIO_Init(LCD_DB4_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB5_Pin;
	HAL_GPIO_Init(LCD_DB5_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB6_Pin;
	HAL_GPIO_Init(LCD_DB6_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB7_Pin;
	HAL_GPIO_Init(LCD_DB7_GPIO_Port, &GPIO_InitStruct);
}

static inline void LCD_DB_Config_Output()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin = LCD_DB0_Pin;
	HAL_GPIO_Init(LCD_DB0_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB1_Pin;
	HAL_GPIO_Init(LCD_DB1_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB2_Pin;
	HAL_GPIO_Init(LCD_DB2_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB3_Pin;
	HAL_GPIO_Init(LCD_DB3_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB4_Pin;
	HAL_GPIO_Init(LCD_DB4_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB5_Pin;
	HAL_GPIO_Init(LCD_DB5_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB6_Pin;
	HAL_GPIO_Init(LCD_DB6_GPIO_Port, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LCD_DB7_Pin;
	HAL_GPIO_Init(LCD_DB7_GPIO_Port, &GPIO_InitStruct);
}

#define LCD_CS_0 LCD_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_1 LCD_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)
#define LCD_RS_0 LCD_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET)
#define LCD_RS_1 LCD_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET)
#define LCD_WR_0 LCD_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET)
#define LCD_WR_1 LCD_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET)
#define LCD_RD_0 LCD_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_RESET)
#define LCD_RD_1 LCD_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_SET)

/**
  * @brief  Configures the LCD_SPI interface.
  */
void LCD_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	/*Configure GPIO pins : LCD_CS_Pin LCD_RS_Pin LCD_WR_Pin LCD_RD_Pin */

	GPIO_InitStruct.Pin = LCD_CS_Pin;
	HAL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_RS_Pin;
	HAL_GPIO_Init(LCD_RS_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_WR_Pin;
	HAL_GPIO_Init(LCD_WR_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_RD_Pin;
	HAL_GPIO_Init(LCD_RD_GPIO_Port, &GPIO_InitStruct);

  	LCD_DB_Config_Output();
}

/**
  * @brief  Reset the LCD_SPI interface.
  */
void LCD_IO_Reset(void)
{
	LCD_CS_1;
	LCD_RS_1;
	LCD_RD_1;
	LCD_WR_1;

	LCD_CS_0;
	LCD_RS_0;

	for (uint8_t i = 0; i < 4; i++)
	{
		LCD_DB_WRITE(0x00);
		LCD_WR_0;
		LCD_WR_1;
	}

	LCD_RS_1;
	LCD_CS_1;
}

//========================================================================
// **                                                                    **
// ** nCS       ----\__________________________________________/-------  **
// ** RS        ------\____________/-----------------------------------  **
// ** nRD       -------------------------\_____/-----------------------  **
// ** nWR       --------\_______/--------------------------------------  **
// ** DB[0:8]   ---------[index]----------[data]-----------------------  **
// **                                                                    **
//========================================================================
uint16_t LCD_IO_ReadReg(uint8_t Reg)
{
	uint32_t readvalue = 0;

	LCD_CS_0;

	// write register index
	LCD_RS_0;
	LCD_RD_1;
	LCD_DB_WRITE(0x00); // write 0x00
	LCD_WR_0;
	LCD_WR_1;
	LCD_DB_WRITE(Reg); // write index
	LCD_WR_0;
	LCD_WR_1;
	LCD_RS_1;

	// read data
	LCD_DB_Config_Input();

	LCD_RD_0;
	readvalue = LCD_DB_READ();
	LCD_RD_1;

	readvalue <<= 8;

	LCD_RD_0;
	readvalue |= LCD_DB_READ();
	LCD_RD_1;

	LCD_CS_1;

	LCD_DB_Config_Output();

	return readvalue;
}

/************************************************************************
 **                                                                    **
 ** nCS       ----\__________________________________________/-------  **
 ** RS        ------\____________/-----------------------------------  **
 ** nRD       -------------------------------------------------------  **
 ** nWR       --------\_______/--------\_____/-----------------------  **
 ** DB[0:8]   ---------[index]----------[data]-----------------------  **
 **                                                                    **
 ************************************************************************/
void LCD_IO_WriteReg(uint8_t Reg, uint16_t Data)
{
	LCD_CS_0;

	// write register index
	LCD_RS_0;
	LCD_RD_1;
	LCD_DB_WRITE(0x00); // write 0x00
	LCD_WR_0;
	LCD_WR_1;
	LCD_DB_WRITE(Reg); // write index
	LCD_WR_0;
	LCD_WR_1;
	LCD_RS_1;

	// write register data
	LCD_DB_WRITE(Data >> 8); // write high byte data
	LCD_WR_0;
	LCD_WR_1;
	LCD_DB_WRITE(Data & 0x00ff); // write low byte data
	LCD_WR_0;
	LCD_WR_1;

	LCD_CS_1;
}

void LCD_IO_WriteRegN(uint8_t Reg, uint16_t Data, uint32_t Count)
{
	LCD_CS_0;

	// write register index
	LCD_RS_0;
	LCD_RD_1;
	LCD_DB_WRITE(0x00); // write 0x00
	LCD_WR_0;
	LCD_WR_1;
	LCD_DB_WRITE(Reg); // write index
	LCD_WR_0;
	LCD_WR_1;
	LCD_RS_1;

	// write register data
	for(uint32_t index=0; index < Count; index++)
	{
		LCD_DB_WRITE(Data >> 8); // write high byte data
		LCD_WR_0;
		LCD_WR_1;
		LCD_DB_WRITE(Data & 0x00ff); // write low byte data
		LCD_WR_0;
		LCD_WR_1;
	}

	LCD_CS_1;
}

void LCD_IO_WriteRegArray(uint8_t Reg, uint16_t *pData, uint32_t Size)
{
	LCD_CS_0;

	// write register index
	LCD_RS_0;
	LCD_RD_1;
	LCD_DB_WRITE(0x00); // write 0x00
	LCD_WR_0;
	LCD_WR_1;
	LCD_DB_WRITE(Reg); // write index
	LCD_WR_0;
	LCD_WR_1;
	LCD_RS_1;

	// write register data
	for(uint32_t index=0; index < Size; index++)
	{
		LCD_DB_WRITE(pData[index] >> 8); // write high byte data
		LCD_WR_0;
		LCD_WR_1;
		LCD_DB_WRITE(pData[index] & 0x00ff); // write low byte data
		LCD_WR_0;
		LCD_WR_1;
	}

	LCD_CS_1;
}

