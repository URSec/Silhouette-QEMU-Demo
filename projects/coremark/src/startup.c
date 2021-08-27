/*
 * Copyright (C) 2021 University of Rochester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define ALIAS(f) __attribute__ ((weak, alias (#f)))

//*****************************************************************************
// External declaration for the pointer to the stack top from the Linker Script
//*****************************************************************************
extern void * _StackTop;

//*****************************************************************************
// Forward declaration of the core exception handlers.
// When the application defines a handler (with the same name), this will
// automatically take precedence over these weak definitions.
// If your application is a C++ one, then any interrupt handlers defined
// in C++ files within in your main application will need to have C linkage
// rather than C++ linkage. To do this, make sure that you are using extern "C"
// { .... } around the interrupt handler within your main application code.
//*****************************************************************************
void ResetISR(void);
void DefaultHandler(void);
void NMI_Handler(void)        ALIAS(DefaultHandler);
void HardFault_Handler(void)  ALIAS(DefaultHandler);
void MemManage_Handler(void)  ALIAS(DefaultHandler);
void BusFault_Handler(void)   ALIAS(DefaultHandler);
void UsageFault_Handler(void) ALIAS(DefaultHandler);
void SVC_Handler(void)        ALIAS(DefaultHandler);
void DebugMon_Handler(void)   ALIAS(DefaultHandler);
void PendSV_Handler(void)     ALIAS(DefaultHandler);
void SysTick_Handler(void)    ALIAS(DefaultHandler);

//*****************************************************************************
// Forward declaration of the application IRQ handlers. When the application
// defines a handler (with the same name), this will automatically take
// precedence over weak definitions below.
//*****************************************************************************
void GPIOA_IRQHandler(void)     ALIAS(DefaultHandler);
void GPIOB_IRQHandler(void)     ALIAS(DefaultHandler);
void GPIOC_IRQHandler(void)     ALIAS(DefaultHandler);
void GPIOD_IRQHandler(void)     ALIAS(DefaultHandler);
void GPIOE_IRQHandler(void)     ALIAS(DefaultHandler);
void UART0_IRQHandler(void)     ALIAS(DefaultHandler);
void UART1_IRQHandler(void)     ALIAS(DefaultHandler);
void SSI0_IRQHandler(void)      ALIAS(DefaultHandler);
void I2C0_IRQHandler(void)      ALIAS(DefaultHandler);
void PWMFault_IRQHandler(void)  ALIAS(DefaultHandler);
void PWMGen0_IRQHandler(void)   ALIAS(DefaultHandler);
void PWMGen1_IRQHandler(void)   ALIAS(DefaultHandler);
void PWMGen2_IRQHandler(void)   ALIAS(DefaultHandler);
void QEI0_IRQHandler(void)      ALIAS(DefaultHandler);
void ADC0_IRQHandler(void)      ALIAS(DefaultHandler);
void ADC1_IRQHandler(void)      ALIAS(DefaultHandler);
void ADC2_IRQHandler(void)      ALIAS(DefaultHandler);
void ADC3_IRQHandler(void)      ALIAS(DefaultHandler);
void WWDG_IRQHandler(void)      ALIAS(DefaultHandler);
void Timer0A_IRQHandler(void)   ALIAS(DefaultHandler);
void Timer0B_IRQHandler(void)   ALIAS(DefaultHandler);
void Timer1A_IRQHandler(void)   ALIAS(DefaultHandler);
void Timer1B_IRQHandler(void)   ALIAS(DefaultHandler);
void Timer2A_IRQHandler(void)   ALIAS(DefaultHandler);
void Timer2B_IRQHandler(void)   ALIAS(DefaultHandler);
void Comp0_IRQHandler(void)     ALIAS(DefaultHandler);
void Comp1_IRQHandler(void)     ALIAS(DefaultHandler);
void SysCtrl_IRQHandler(void)   ALIAS(DefaultHandler);
void FlashCtrl_IRQHandler(void) ALIAS(DefaultHandler);
void GPIOF_IRQHandler(void)     ALIAS(DefaultHandler);
void GPIOG_IRQHandler(void)     ALIAS(DefaultHandler);
void UART2_IRQHandler(void)     ALIAS(DefaultHandler);
void Timer3A_IRQHandler(void)   ALIAS(DefaultHandler);
void Timer3B_IRQHandler(void)   ALIAS(DefaultHandler);
void I2C1_IRQHandler(void)      ALIAS(DefaultHandler);
void QEI1_IRQHandler(void)      ALIAS(DefaultHandler);
void Ethernet_IRQHandler(void)  ALIAS(DefaultHandler);
void Hibernate_IRQHandler(void) ALIAS(DefaultHandler);

//*****************************************************************************
// The vector table.
// This relies on the linker script to place at correct location in memory.
//*****************************************************************************
__attribute__ ((used, section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
	(void (*)(void))&_StackTop,             // The initial stack pointer

	// Core exceptions
	ResetISR,                               // The reset handler
	NMI_Handler,                            // The NMI handler
	HardFault_Handler,                      // The hard fault handler
	MemManage_Handler,                      // The MPU fault handler
	BusFault_Handler,                       // The bus fault handler
	UsageFault_Handler,                     // The usage fault handler
	0,                                      // Reserved
	0,                                      // Reserved
	0,                                      // Reserved
	0,                                      // Reserved
	SVC_Handler,                            // SVCall handler
	DebugMon_Handler,                       // Debug monitor handler
	0,                                      // Reserved
	PendSV_Handler,                         // The PendSV handler
	SysTick_Handler,                        // The SysTick handler

	// Interrupts
	GPIOA_IRQHandler,                       // GPIO Port A
	GPIOB_IRQHandler,                       // GPIO Port B
	GPIOC_IRQHandler,                       // GPIO Port C
	GPIOD_IRQHandler,                       // GPIO Port D
	GPIOE_IRQHandler,                       // GPIO Port E
	UART0_IRQHandler,                       // UART0 Rx and Tx
	UART1_IRQHandler,                       // UART1 Rx and Tx
	SSI0_IRQHandler,                        // SSI0 Rx and Tx
	I2C0_IRQHandler,                        // I2C0 Master and Slave
	PWMFault_IRQHandler,                    // PWM Fault
	PWMGen0_IRQHandler,                     // PWM Generator 0
	PWMGen1_IRQHandler,                     // PWM Generator 1
	PWMGen2_IRQHandler,                     // PWM Generator 2
	QEI0_IRQHandler,                        // Quadrature Encoder 0
	ADC0_IRQHandler,                        // ADC Sequence 0
	ADC1_IRQHandler,                        // ADC Sequence 1
	ADC2_IRQHandler,                        // ADC Sequence 2
	ADC3_IRQHandler,                        // ADC Sequence 3
	WWDG_IRQHandler,                        // Watchdog timer
	Timer0A_IRQHandler,                     // Timer 0 subtimer A
	Timer0B_IRQHandler,                     // Timer 0 subtimer B
	Timer1A_IRQHandler,                     // Timer 1 subtimer A
	Timer1B_IRQHandler,                     // Timer 1 subtimer B
	Timer2A_IRQHandler,                     // Timer 2 subtimer A
	Timer2B_IRQHandler,                     // Timer 2 subtimer B
	Comp0_IRQHandler,                       // Analog Comparator 0
	Comp1_IRQHandler,                       // Analog Comparator 1
	0,                                      // Reserved
	SysCtrl_IRQHandler,                     // System Control (PLL, OSC, BO)
	FlashCtrl_IRQHandler,                   // FLASH Control
	GPIOF_IRQHandler,                       // GPIO Port F
	GPIOG_IRQHandler,                       // GPIO Port G
	0,                                      // Reserved
	UART2_IRQHandler,                       // UART2 Rx and Tx
	0,                                      // Reserved
	Timer3A_IRQHandler,                     // Timer 3 subtimer A
	Timer3B_IRQHandler,                     // Timer 3 subtimer B
	I2C1_IRQHandler,                        // I2C1 Master and Slave
	QEI1_IRQHandler,                        // Quadrature Encoder 1
	0,                                      // Reserved
	0,                                      // Reserved
	0,                                      // Reserved
	Ethernet_IRQHandler,                    // Ethernet
	Hibernate_IRQHandler,                   // Hibernate
};

//*****************************************************************************
// Declaration of external SystemInit function.
//*****************************************************************************
#ifdef __USE_CMSIS
extern void SystemInit(void);
#endif /* __USE_CMSIS */

//*****************************************************************************
// The entry and exit points for the C library startup.
//*****************************************************************************
extern void __libc_init_array(void);
extern void __libc_fini_array(void);

//*****************************************************************************
// The entry point for the application.
//*****************************************************************************
extern int main(void);

//*****************************************************************************
// Functions to carry out the initialization of RW and BSS data sections. These
// are written as separate functions rather than being inlined within the
// ResetISR() function in order to cope with MCUs with multiple banks of
// memory.
//*****************************************************************************
static void
data_init(unsigned int romstart, unsigned int start, unsigned int len)
{
	unsigned int * dest = (unsigned int *)start;
	unsigned int * src = (unsigned int *)romstart;
	unsigned int loop;
	for (loop = 0; loop < len; loop += 4) {
		*dest++ = *src++;
	}
}

static void
bss_init(unsigned int start, unsigned int len)
{
	unsigned int * dest = (unsigned int *)start;
	unsigned int loop;
	for (loop = 0; loop < len; loop += 4) {
		*dest++ = 0;
	}
}

//*****************************************************************************
// The following symbols are constructs generated by the linker, indicating
// the location of various points in the "Global Section Table".  It contains
// the load address, execution address, and length of each RW data section, and
// the execution and length of each BSS (zero initialized) section.
//*****************************************************************************
extern unsigned int __data_section_table;
extern unsigned int __data_section_table_end;
extern unsigned int __bss_section_table;
extern unsigned int __bss_section_table_end;

//*****************************************************************************
// Reset entry point for your code.
// Sets up a simple runtime environment and initializes the C/C++ library.
//*****************************************************************************
void
ResetISR(void)
{
	// Disable interrupts
	asm volatile ("cpsid i");

    	//
	// Copy the data sections from Flash to RAM.
	//
	unsigned int LoadAddr, ExeAddr, SectionLen;
	unsigned int *SectionTableAddr;

	// Load base address of Global Section Table
	SectionTableAddr = &__data_section_table;

	// Copy the data sections from Flash to RAM.
	while (SectionTableAddr < &__data_section_table_end) {
		LoadAddr = *SectionTableAddr++;
		ExeAddr = *SectionTableAddr++;
		SectionLen = *SectionTableAddr++;
		data_init(LoadAddr, ExeAddr, SectionLen);
	}

	// At this point, SectionTableAddr = &__bss_section_table;
	// zero-fill the bss segment
	while (SectionTableAddr < &__bss_section_table_end) {
		ExeAddr = *SectionTableAddr++;
		SectionLen = *SectionTableAddr++;
		bss_init(ExeAddr, SectionLen);
	}

	//
	// Call C++ library initialisation.
	//
	__libc_init_array();

#ifdef __USE_CMSIS
        // If __USE_CMSIS defined, then call CMSIS SystemInit code
        SystemInit();
#endif /* __USE_CMSIS */

	// Reenable interrupts
	asm volatile ("cpsie i");

	main();

	//
	// Call C++ library finalization.
	//
	__libc_fini_array();

	//
	// main() shouldn't return, but if it does, we'll just enter an
	// infinite loop.
	//
	while (1) {
		asm volatile ("wfi");
	}
}

//*****************************************************************************
// The default exception handler. Override the ones here by defining your own
// handler routines in your application code.
//*****************************************************************************
void
DefaultHandler(void)
{
	while (1) {
	}
}
