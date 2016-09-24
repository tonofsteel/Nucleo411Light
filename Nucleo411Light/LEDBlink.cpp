#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_rcc.h>
//#include <stm32f4xx_hal_rcc_ex.h>

#ifdef __cplusplus
extern "C"
#endif
	
// Move to separate file for the interrupt handlers, putting any function declaration before this causes failure
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void SetSysClockTo100MHz(void);

int main(void)
{
	SetSysClockTo100MHz();
	
	HAL_Init();

	__GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_5;

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	uint32_t ClockFrequency = HAL_RCC_GetSysClockFreq();
	
	
	for (;;)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_Delay(500);
	}
}

// SYSCLK = 100 MHz
// HCLK = 100 MHz
// APB1/PCLK1 = 50 MHz
// APB2/PCLK2 = 100 MHz
// All timers 100 MHz (APB1/APB2)
void SetSysClockTo100MHz(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	
	//HAL_RCC_DeInit();
	
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;  //16 Mhz
	RCC_OscInitStruct.HSEState = RCC_HSE_OFF;	// No External Crystal Installed on Nucleo
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 10;
	RCC_OscInitStruct.PLL.PLLN = 125;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);	// RM0383 manual Section 3.4 Read Interface (min is 3 but 4 for lower voltage range) - this setting depends on frequency configured and voltage

	
}
