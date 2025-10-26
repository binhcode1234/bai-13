#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"

void USART1_Init(void);
void sendString(char *str);

void Task1(void *pvParameters);
void Task2(void *pvParameters);

int main(void)
{
    SystemInit();
    USART1_Init();

    xTaskCreate(Task1, (const char *)"Task1", 128, NULL, 1, NULL);
    xTaskCreate(Task2, (const char *)"Task2", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1);
}

void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // PA9 -> TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA10 -> RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void sendString(char *str)
{
    while(*str)
    {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, *str++);
    }
}

void Task1(void *pvParameters)
{
    while(1)
    {
        sendString("Task 1 sending data...\r\n");
        vTaskDelay(200 / portTICK_RATE_MS);
    }
}

void Task2(void *pvParameters)
{
    while(1)
    {
        sendString("Task 2 sending data...\r\n");
        vTaskDelay(200 / portTICK_RATE_MS);
    }
}
