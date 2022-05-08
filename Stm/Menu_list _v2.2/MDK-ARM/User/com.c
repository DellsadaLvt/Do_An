#include "com.h"

uint8_t u8g_RxBuff = 0u;

HAL_StatusTypeDef COM_TestCmdToEsp( void ){
	static uint8_t u8l_Count = 0u;
	static uint8_t u8l_Count_1 = 0u;
	static uint8_t u8l_Count_2 = 0u;
	
		if( u8l_Count % 2u ){
			uint8_t u8l_s_Cmd[4U];
			sprintf((char*)u8l_s_Cmd, "%dt", u8l_Count_2);
			HAL_UART_Transmit(&huart1, u8l_s_Cmd, strlen((char*)u8l_s_Cmd), 100u);
			u8l_Count_2 += 1u;
			if ( u8l_Count_2 >= 9u){
				u8l_Count_2 = 0u;
			}
		}
		else{
			uint8_t u8l_s_Cmd[4U];
			sprintf((char*)u8l_s_Cmd, "%dh", u8l_Count_1);
			HAL_UART_Transmit(&huart1, u8l_s_Cmd, strlen((char*)u8l_s_Cmd), 100u);
			u8l_Count_1 += 5u;
		}
		
		u8l_Count += 1u;
		HAL_Delay(2000u);
		
		return HAL_OK;
}

HAL_StatusTypeDef COM_TestReceiveFromEsp( void ){
	HAL_UART_Receive_IT(&huart1, &u8g_RxBuff, 1u);
	
	return HAL_OK;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  
	if( huart->Instance == USART1 ){
		HAL_UART_Receive_IT(&huart1, &u8g_RxBuff, 1u);
	}
}

