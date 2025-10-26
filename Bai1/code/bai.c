#include "stm32f10x.h"

// H�m kh?i t?o LED
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // Clock cho GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // Clock cho GPIOB

    // LED1 - PA0 (Blink)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // LED2 - PB12 (�i?u khi?n b?ng n�t nh?n)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// H�m kh?i t?o n�t nh?n d�ng ng?t ngo�i
void Button_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // PB13 l�m input (Pull-up)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // K?t n?i EXTI Line13 v?i PB13
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

    // C?u h�nh EXTI cho PB13
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // C? 2 c?nh
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // C?u h�nh NVIC cho EXTI15_10_IRQn
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// Task 1: Blink LED PA0
void Task_Blink(void)
{
    GPIOA->ODR ^= GPIO_Pin_0; // �?o tr?ng th�i LED
    for (volatile int i = 0; i < 500000; i++); // Delay don gi?n
}

// Ng?t ngo�i t? PB13
void EXTI15_10_IRQHandler(void)
{
    // Ki?m tra c� ng?t t? line 13 kh�ng
    if (EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        // �?c tr?ng th�i n�t
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == Bit_RESET)
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_12); // B?t LED
        }
        else
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_12); // T?t LED
        }

        
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
}

int main(void)
{
    SystemInit();
    LED_Init();
    Button_Init();
	Task_Blink(); 

    while (1)
    {
        Task_Blink(); 
        
    }
}
