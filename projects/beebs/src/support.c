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

#include <stdint.h>
#include <assert.h>

//=============================================================================
// Clock
//=============================================================================

/* SysCtrl registers */
#define RIS	*(volatile uint32_t *)0x400fe050
#define RCC	*(volatile uint32_t *)0x400fe060
#define PLLCFG	*(volatile uint32_t *)0x400fe064
#define RCC2	*(volatile uint32_t *)0x400fe070
#define RCGC1	*(volatile uint32_t *)0x400fe104

/* RIS field accessors */
#define RIS_PLLLRIS_SHIFT	(6)
#define RIS_PLLLRIS_MASK	(0x1u << RIS_PLLLRIS_SHIFT)
#define RIS_PLLLRIS_GET(x)	(((x) & RIS_PLLLRIS_MASK) >> RIS_PLLLRIS_SHIFT)
#define RIS_BORRIS_SHIFT	(1)
#define RIS_BORRIS_MASK		(0x1u << RIS_BORRIS_SHIFT)
#define RIS_BORRIS_GET(x)	(((x) & RIS_BORRIS_MASK) >> RIS_BORRIS_SHIFT)

/* RCC field accessors */
#define RCC_ACG_SHIFT		(27)
#define RCC_ACG_MASK		(0x1u << RCC_ACG_SHIFT)
#define RCC_ACG_SET(x)		(((x) << RCC_ACG_SHIFT) & RCC_ACG_MASK)
#define RCC_ACG_GET(x)		(((x) & RCC_ACG_MASK) >> RCC_ACG_SHIFT)
#define RCC_SYSDIV_SHIFT	(23)
#define RCC_SYSDIV_MASK		(0xfu << RCC_SYSDIV_SHIFT)
#define RCC_SYSDIV_SET(x)	(((x) << RCC_SYSDIV_SHIFT) & RCC_SYSDIV_MASK)
#define RCC_SYSDIV_GET(x)	(((x) & RCC_SYSDIV_MASK) >> RCC_SYSDIV_SHIFT)
#define RCC_USESYSDIV_SHIFT	(22)
#define RCC_USESYSDIV_MASK	(0x1u << RCC_USESYSDIV_SHIFT)
#define RCC_USESYSDIV_SET(x)	(((x) << RCC_USESYSDIV_SHIFT) & RCC_USESYSDIV_MASK)
#define RCC_USESYSDIV_GET(x)	(((x) & RCC_USESYSDIV_MASK) >> RCC_USESYSDIV_SHIFT)
#define RCC_USEPWMDIV_SHIFT	(20)
#define RCC_USEPWMDIV_MASK	(0x1u << RCC_USEPWMDIV_SHIFT)
#define RCC_USEPWMDIV_SET(x)	(((x) << RCC_USEPWMDIV_SHIFT) & RCC_USEPWMDIV_MASK)
#define RCC_USEPWMDIV_GET(x)	(((x) & RCC_USEPWMDIV_MASK) >> RCC_USEPWMDIV_SHIFT)
#define RCC_PWMDIV_SHIFT	(17)
#define RCC_PWMDIV_MASK		(0x7u << RCC_PWMDIV_SHIFT)
#define RCC_PWMDIV_SET(x)	(((x) << RCC_PWMDIV_SHIFT) & RCC_PWMDIV_MASK)
#define RCC_PWMDIV_GET(x)	(((x) & RCC_PWMDIV_MASK) >> RCC_PWMDIV_SHIFT)
#define RCC_PWRDN_SHIFT		(13)
#define RCC_PWRDN_MASK		(0x1u << RCC_PWRDN_SHIFT)
#define RCC_PWRDN_SET(x)	(((x) << RCC_PWRDN_SHIFT) & RCC_PWRDN_MASK)
#define RCC_PWRDN_GET(x)	(((x) & RCC_PWRDN_MASK) >> RCC_PWRDN_SHIFT)
#define RCC_BYPASS_SHIFT	(11)
#define RCC_BYPASS_MASK		(0x1u << RCC_BYPASS_SHIFT)
#define RCC_BYPASS_SET(x)	(((x) << RCC_BYPASS_SHIFT) & RCC_BYPASS_MASK)
#define RCC_BYPASS_GET(x)	(((x) & RCC_BYPASS_MASK) >> RCC_BYPASS_SHIFT)
#define RCC_XTAL_SHIFT		(6)
#define RCC_XTAL_MASK		(0xfu << RCC_XTAL_SHIFT)
#define RCC_XTAL_SET(x)		(((x) << RCC_XTAL_SHIFT) & RCC_XTAL_MASK)
#define RCC_XTAL_GET(x)		(((x) & RCC_XTAL_MASK) >> RCC_XTAL_SHIFT)
#define RCC_OSCSRC_SHIFT	(4)
#define RCC_OSCSRC_MASK		(0x3u << RCC_OSCSRC_SHIFT)
#define RCC_OSCSRC_SET(x)	(((x) << RCC_OSCSRC_SHIFT) & RCC_OSCSRC_MASK)
#define RCC_OSCSRC_GET(x)	(((x) & RCC_OSCSRC_MASK) >> RCC_OSCSRC_SHIFT)
#define RCC_IOSCDIS_SHIFT	(1)
#define RCC_IOSCDIS_MASK	(0x1u << RCC_IOSCDIS_SHIFT)
#define RCC_IOSCDIS_SET(x)	(((x) << RCC_IOSCDIS_SHIFT) & RCC_IOSCDIS_MASK)
#define RCC_IOSCDIS_GET(x)	(((x) & RCC_IOSCDIS_MASK) >> RCC_IOSCDIS_SHIFT)
#define RCC_MOSCDIS_SHIFT	(0)
#define RCC_MOSCDIS_MASK	(0x1u << RCC_MOSCDIS_SHIFT)
#define RCC_MOSCDIS_SET(x)	(((x) << RCC_MOSCDIS_SHIFT) & RCC_MOSCDIS_MASK)
#define RCC_MOSCDIS_GET(x)	(((x) & RCC_MOSCDIS_MASK) >> RCC_MOSCDIS_SHIFT)

/* PLLCFG field accessors */
#define PLLCFG_F_SHIFT	(5)
#define PLLCFG_F_MASK	(0x1ffu << PLLCFG_F_SHIFT)
#define PLLCFG_F_SET(x)	(((x) << PLLCFG_F_SHIFT) & PLLCFG_F_MASK)
#define PLLCFG_F_GET(x)	(((x) & PLLCFG_F_MASK) >> PLLCFG_F_SHIFT)
#define PLLCFG_R_SHIFT	(0)
#define PLLCFG_R_MASK	(0x1fu << PLLCFG_R_SHIFT)
#define PLLCFG_R_SET(x)	(((x) << PLLCFG_R_SHIFT) & PLLCFG_R_MASK)
#define PLLCFG_R_GET(x)	(((x) & PLLCFG_R_MASK) >> PLLCFG_R_SHIFT)

/* RCC2 field accessors */
#define RCC2_USERCC2_SHIFT	(31)
#define RCC2_USERCC2_MASK	(0x1u << RCC2_USERCC2_SHIFT)
#define RCC2_USERCC2_SET(x)	(((x) << RCC2_USERCC2_SHIFT) & RCC2_USERCC2_MASK)
#define RCC2_USERCC2_GET(x)	(((x) & RCC2_USERCC2_MASK) >> RCC2_USERCC2_SHIFT)
#define RCC2_SYSDIV2_SHIFT	(23)
#define RCC2_SYSDIV2_MASK	(0x3fu << RCC2_SYSDIV2_SHIFT)
#define RCC2_SYSDIV2_SET(x)	(((x) << RCC2_SYSDIV2_SHIFT) & RCC2_SYSDIV2_MASK)
#define RCC2_SYSDIV2_GET(x)	(((x) & RCC2_SYSDIV2_MASK) >> RCC2_SYSDIV2_SHIFT)
#define RCC2_PWRDN2_SHIFT	(13)
#define RCC2_PWRDN2_MASK	(0x1u << RCC2_PWRDN2_SHIFT)
#define RCC2_PWRDN2_SET(x)	(((x) << RCC2_PWRDN2_SHIFT) & RCC2_PWRDN2_MASK)
#define RCC2_PWRDN2_GET(x)	(((x) & RCC2_PWRDN2_MASK) >> RCC2_PWRDN2_SHIFT)
#define RCC2_BYPASS2_SHIFT	(11)
#define RCC2_BYPASS2_MASK	(0x1u << RCC2_BYPASS2_SHIFT)
#define RCC2_BYPASS2_SET(x)	(((x) << RCC2_BYPASS2_SHIFT) & RCC2_BYPASS2_MASK)
#define RCC2_BYPASS2_GET(x)	(((x) & RCC2_BYPASS2_MASK) >> RCC2_BYPASS2_SHIFT)
#define RCC2_OSCSRC2_SHIFT	(4)
#define RCC2_OSCSRC2_MASK	(0x7u << RCC2_OSCSRC2_SHIFT)
#define RCC2_OSCSRC2_SET(x)	(((x) << RCC2_OSCSRC2_SHIFT) & RCC2_OSCSRC2_MASK)
#define RCC2_OSCSRC2_GET(x)	(((x) & RCC2_OSCSRC2_MASK) >> RCC2_OSCSRC2_SHIFT)

/* RCGC1 field accessors */
#define RCGC1_UART0_SHIFT	(0)
#define RCGC1_UART0_MASK	(0x1u << RCGC1_UART0_SHIFT)
#define RCGC1_UART0_SET(x)	(((x) << RCGC1_UART0_SHIFT) & RCGC1_UART0_MASK)
#define RCGC1_UART0_GET(x)	(((x) & RCGC1_UART0_MASK) >> RCGC1_UART0_SHIFT)

/* Preset oscillator frequencies */
#define IOSC_FREQ	(12000000u)
#define IOSC30K_FREQ	(   30000u)
#define EOSC_FREQ	(   32768u)

/* System clock frequency */
uint32_t SystemCoreClock = 12000000u;

/* Crystal value to main oscillator frequency */
const uint32_t Xtal2MainOsc[] = {
	[0x0] = 1000000u,
	[0x1] = 1843200u,
	[0x2] = 2000000u,
	[0x3] = 2457600u,
	[0x4] = 3579545u,
	[0x5] = 3686400u,
	[0x6] = 4000000u,
	[0x7] = 4096000u,
	[0x8] = 4915200u,
	[0x9] = 5000000u,
	[0xa] = 5120000u,
	[0xb] = 6000000u,
	[0xc] = 6144000u,
	[0xd] = 7372800u,
	[0xe] = 8000000u,
	[0xf] = 8192000u,
};

/*
 * Get the oscillator frequency with given oscillator source and crystal value.
 *
 * @OscSrc: the oscillator source, ranging from 0 to 7
 * @Xtal: the crystal value (only valid when @OscSrc is 0), ranging from 0 to 15
 *
 * return: the oscillator frequency
 */
static uint32_t
GetOscillatorFrequency(uint32_t OscSrc, uint32_t Xtal)
{
	switch (OscSrc) {
	/* Main oscillator */
	case 0x0:
		return Xtal2MainOsc[Xtal & 0xf];

	/* Internal oscillator */
	case 0x1:
		return IOSC_FREQ;

	/* Internal oscillator / 4 */
	case 0x2:
		return IOSC_FREQ / 4;

	/* 30-kHz internal oscillator */
	case 0x3:
		return IOSC30K_FREQ;

	/* 32768-Hz external real-time oscillator */
	case 0x7:
		return EOSC_FREQ;

	/* Reserved or illegal values */
	default:
		assert(0 && "Illegal oscillator source!");
	}
}

/*
 * Update the system clock frequency.
 */
void
SystemCoreClockUpdate(void)
{
	uint32_t rcc = RCC;
	uint32_t rcc2 = RCC2;
	uint32_t pllcfg = PLLCFG;

	uint32_t clock;

	/* Check if we use RCC2 */
	if (RCC2_USERCC2_GET(rcc2)) {
		clock = GetOscillatorFrequency(RCC2_OSCSRC2_GET(rcc2),
					       RCC_XTAL_GET(rcc));
		/* Check if we use PLL */
		if (!RCC2_BYPASS2_GET(rcc2)) {
			assert(!RCC2_PWRDN2_GET(rcc2) &&
			       "PLL down but selected?");
			assert(RCC2_OSCSRC2_GET(rcc2) == 0x0 &&
			       "Illegal clock source for PLL!");
			clock = clock * PLLCFG_F_GET(pllcfg) /
				(PLLCFG_R_GET(pllcfg) + 1) / 2;
		}

		/* Check SYSDIV */
		if (RCC_USESYSDIV_GET(rcc)) {
			clock /= (RCC2_SYSDIV2_GET(rcc2) + 1);
		}
	} else {
		clock = GetOscillatorFrequency(RCC_OSCSRC_GET(rcc),
					       RCC_XTAL_GET(rcc));
		/* Check if we use PLL */
		if (!RCC_BYPASS_GET(rcc)) {
			assert(!RCC_PWRDN_GET(rcc) && "PLL down but selected?");
			assert(RCC_OSCSRC_GET(rcc) == 0x0 &&
			       "Illegal clock source for PLL!");
			clock = clock * PLLCFG_F_GET(pllcfg) /
				(PLLCFG_R_GET(pllcfg) + 1) / 2;
		}

		/* Check SYSDIV */
		if (RCC_USESYSDIV_GET(rcc)) {
			clock /= (RCC_SYSDIV_GET(rcc) + 1);
		}
	}

	SystemCoreClock = clock;
}

/*
 * Initialize the system Clock.
 */
void
Clock_Init(void)
{
	/*
	 * Set RCC and RCC2 to boost the system clock frequency to maximum
	 * (50 MHz).
	 */
	RCC = RCC_ACG_SET(0) |
	      RCC_SYSDIV_SET(0x3u) |
	      RCC_USESYSDIV_SET(1) |
	      RCC_USEPWMDIV_SET(0) |
	      RCC_PWMDIV_SET(0x7u) |
	      RCC_PWRDN_SET(0) |
	      RCC_BYPASS_SET(0) |
	      RCC_XTAL_SET(0xbu) |
	      RCC_OSCSRC_SET(0x0u) |
	      RCC_IOSCDIS_SET(1) |
	      RCC_MOSCDIS_SET(0);
	RCC2 = RCC2_USERCC2_SET(0) |
	       RCC2_SYSDIV2_SET(0x0fu) |
	       RCC2_PWRDN2_SET(0) |
	       RCC2_BYPASS2_SET(0) |
	       RCC2_OSCSRC2_SET(0x0u);

	/* Busy-wait until the system clock source is switched to PLL */
	while (!RIS_PLLLRIS_GET(RIS)) {
	}

	/* Update the system clock frequency */
	SystemCoreClockUpdate();

	/* Clock the UART0 peripheral */
	RCGC1 |= RCGC1_UART0_SET(1);
}

//=============================================================================
// SysTick
//=============================================================================

/* SysTick registers */
#define STCTRL		*(volatile uint32_t *)0xe000e010
#define STRELOAD	*(volatile uint32_t *)0xe000e014
#define STCURRENT	*(volatile uint32_t *)0xe000e018

/* STCTRL field accessors */
#define STCTRL_COUNT_SHIFT	(16)
#define STCTRL_COUNT_MASK	(0x1u << STCTRL_COUNT_SHIFT)
#define STCTRL_COUNT_SET(x)	(((x) << STCTRL_COUNT_SHIFT) & STCTRL_COUNT_MASK)
#define STCTRL_COUNT_GET(x)	(((x) & STCTRL_COUNT_MASK) >> STCTRL_COUNT_SHIFT)
#define STCTRL_CLK_SRC_SHIFT	(2)
#define STCTRL_CLK_SRC_MASK	(0x1u << STCTRL_CLK_SRC_SHIFT)
#define STCTRL_CLK_SRC_SET(x)	(((x) << STCTRL_CLK_SRC_SHIFT) & STCTRL_CLK_SRC_MASK)
#define STCTRL_CLK_SRC_GET(x)	(((x) & STCTRL_CLK_SRC_MASK) >> STCTRL_CLK_SRC_SHIFT)
#define STCTRL_INTEN_SHIFT	(1)
#define STCTRL_INTEN_MASK	(0x1u << STCTRL_INTEN_SHIFT)
#define STCTRL_INTEN_SET(x)	(((x) << STCTRL_INTEN_SHIFT) & STCTRL_INTEN_MASK)
#define STCTRL_INTEN_GET(x)	(((x) & STCTRL_INTEN_MASK) >> STCTRL_INTEN_SHIFT)
#define STCTRL_ENABLE_SHIFT	(0)
#define STCTRL_ENABLE_MASK	(0x1u << STCTRL_ENABLE_SHIFT)
#define STCTRL_ENABLE_SET(x)	(((x) << STCTRL_ENABLE_SHIFT) & STCTRL_ENABLE_MASK)
#define STCTRL_ENABLE_GET(x)	(((x) & STCTRL_ENABLE_MASK) >> STCTRL_ENABLE_SHIFT)

/* STRELOAD field accessors */
#define STRELOAD_RELOAD_SHIFT	(0)
#define STRELOAD_RELOAD_MASK	(0xffffffu << STRELOAD_RELOAD_SHIFT)
#define STRELOAD_RELOAD_SET(x)	(((x) << STRELOAD_RELOAD_SHIFT) & STRELOAD_RELOAD_MASK)
#define STRELOAD_RELOAD_GET(x)	(((x) << STRELOAD_RELOAD_SHIFT) & STRELOAD_RELOAD_MASK)

/* STCURRENT field accessors */
#define STCURRENT_CURRENT_SHIFT		(0)
#define STCURRENT_CURRENT_MASK		(0xffffffu << STCURRENT_CURRENT_SHIFT)
#define STCURRENT_CURRENT_SET(x)	(((x) << STCURRENT_CURRENT_SHIFT) & STCURRENT_CURRENT_MASK)
#define STCURRENT_CURRENT_GET(x)	(((x) & STCURRENT_CURRENT_MASK) >> STCURRENT_CURRENT_SHIFT)

/* Number of ticks */
static uint32_t tick;

/*
 * Initialize SysTick.
 */
void __attribute__((section("privileged_functions"), noinline))
SysTick_Init(void)
{
	tick = 0;

	/* Set SysTick frequency: one tick per millisecond */
	STRELOAD = STRELOAD_RELOAD_SET(SystemCoreClock / 1000 - 1);

	/* Set current value to 0 */
	STCURRENT = STCURRENT_CURRENT_SET(0u);

	/* Enable SysTick with internal clock source and interrupt enabled */
	STCTRL = STCTRL_CLK_SRC_SET(1) |
		 STCTRL_INTEN_SET(1) |
		 STCTRL_ENABLE_SET(1);
}

/*
 * Handler of SysTick interrupt.
 */
void
SysTick_Handler(void)
{
	++tick;
}

/*
 * Get the current tick.
 *
 * return: the current tick
 */
uint32_t
SysTick_GetTick(void)
{
	return tick;
}

//=============================================================================
// UART
//=============================================================================

/* UART registers */
#define UART0DR		*(volatile uint32_t *)0x4000c000
#define UART0FR		*(volatile uint32_t *)0x4000c018
#define UART0IBRD	*(volatile uint32_t *)0x4000c024
#define UART0FBRD	*(volatile uint32_t *)0x4000c028
#define UART0LCRH	*(volatile uint32_t *)0x4000c02c
#define UART0CTL	*(volatile uint32_t *)0x4000c030

/* UART0DR field accessors */
#define UART0DR_DATA_SHIFT	(0)
#define UART0DR_DATA_MASK	(0xffu << UART0DR_DATA_SHIFT)
#define UART0DR_DATA_SET(x)	(((x) << UART0DR_DATA_SHIFT) & UART0DR_DATA_MASK)
#define UART0DR_DATA_GET(x)	(((x) & UART0DR_DATA_MASK) >> UART0DR_DATA_SHIFT)

/* UART0FR field accessors */
#define UART0FR_TXFE_SHIFT	(7)
#define UART0FR_TXFE_MASK	(0x1u << UART0FR_TXFE_SHIFT)
#define UART0FR_TXFE_GET(x)	(((x) & UART0FR_TXFE_MASK) >> UART0FR_TXFE_SHIFT)
#define UART0FR_RXFF_SHIFT	(6)
#define UART0FR_RXFF_MASK	(0x1u << UART0FR_RXFF_SHIFT)
#define UART0FR_RXFF_GET(x)	(((x) & UART0FR_RXFF_MASK) >> UART0FR_RXFF_SHIFT)
#define UART0FR_TXFF_SHIFT	(5)
#define UART0FR_TXFF_MASK	(0x1u << UART0FR_TXFF_SHIFT)
#define UART0FR_TXFF_GET(x)	(((x) & UART0FR_TXFF_MASK) >> UART0FR_TXFF_SHIFT)
#define UART0FR_RXFE_SHIFT	(4)
#define UART0FR_RXFE_MASK	(0x1u << UART0FR_RXFE_SHIFT)
#define UART0FR_RXFE_GET(x)	(((x) & UART0FR_RXFE_MASK) >> UART0FR_RXFE_SHIFT)
#define UART0FR_BUSY_SHIFT	(3)
#define UART0FR_BUSY_MASK	(0x1u << UART0FR_BUSY_SHIFT)
#define UART0FR_BUSY_GET(x)	(((x) & UART0FR_BUSY_MASK) >> UART0FR_BUSY_SHIFT)

/* UART0IBRD field accessors */
#define UART0IBRD_DIVINT_SHIFT	(0)
#define UART0IBRD_DIVINT_MASK	(0xffffu << UART0IBRD_DIVINT_SHIFT)
#define UART0IBRD_DIVINT_SET(x)	(((x) << UART0IBRD_DIVINT_SHIFT) & UART0IBRD_DIVINT_MASK)
#define UART0IBRD_DIVINT_GET(x)	(((x) & UART0IBRD_DIVINT_MASK) >> UART0IBRD_DIVINT_SHIFT)

/* UART0FBRD field accessors */
#define UART0FBRD_DIVFRAC_SHIFT		(0)
#define UART0FBRD_DIVFRAC_MASK		(0x3fu << UART0FBRD_DIVFRAC_SHIFT)
#define UART0FBRD_DIVFRAC_SET(x)	(((x) << UART0FBRD_DIVFRAC_SHIFT) & UART0FBRD_DIVFRAC_MASK)
#define UART0FBRD_DIVFRAC_GET(x)	(((x) & UART0FBRD_DIVFRAC_MASK) >> UART0FBRD_DIVFRAC_SHIFT)

/* UART0LCRH field accessors */
#define UART0LCRH_SPS_SHIFT	(7)
#define UART0LCRH_SPS_MASK	(0x1u << UART0LCRH_SPS_SHIFT)
#define UART0LCRH_SPS_SET(x)	(((x) << UART0LCRH_SPS_SHIFT) & UART0LCRH_SPS_MASK)
#define UART0LCRH_SPS_GET(x)	(((x) & UART0LCRH_SPS_MASK) >> UART0LCRH_SPS_SHIFT)
#define UART0LCRH_WLEN_SHIFT	(5)
#define UART0LCRH_WLEN_MASK	(0x3u << UART0LCRH_WLEN_SHIFT)
#define UART0LCRH_WLEN_SET(x)	(((x) << UART0LCRH_WLEN_SHIFT) & UART0LCRH_WLEN_MASK)
#define UART0LCRH_WLEN_GET(x)	(((x) & UART0LCRH_WLEN_MASK) >> UART0LCRH_WLEN_SHIFT)
#define UART0LCRH_FEN_SHIFT	(4)
#define UART0LCRH_FEN_MASK	(0x1u << UART0LCRH_FEN_SHIFT)
#define UART0LCRH_FEN_SET(x)	(((x) << UART0LCRH_FEN_SHIFT) & UART0LCRH_FEN_MASK)
#define UART0LCRH_FEN_GET(x)	(((x) & UART0LCRH_FEN_MASK) >> UART0LCRH_FEN_SHIFT)
#define UART0LCRH_STP2_SHIFT	(3)
#define UART0LCRH_STP2_MASK	(0x1u << UART0LCRH_STP2_SHIFT)
#define UART0LCRH_STP2_SET(x)	(((x) << UART0LCRH_STP2_SHIFT) & UART0LCRH_STP2_MASK)
#define UART0LCRH_STP2_GET(x)	(((x) & UART0LCRH_STP2_MASK) >> UART0LCRH_STP2_SHIFT)
#define UART0LCRH_EPS_SHIFT	(2)
#define UART0LCRH_EPS_MASK	(0x1u << UART0LCRH_EPS_SHIFT)
#define UART0LCRH_EPS_SET(x)	(((x) << UART0LCRH_EPS_SHIFT) & UART0LCRH_EPS_MASK)
#define UART0LCRH_EPS_GET(x)	(((x) & UART0LCRH_EPS_MASK) >> UART0LCRH_EPS_SHIFT)
#define UART0LCRH_PEN_SHIFT	(1)
#define UART0LCRH_PEN_MASK	(0x1u << UART0LCRH_PEN_SHIFT)
#define UART0LCRH_PEN_SET(x)	(((x) << UART0LCRH_PEN_SHIFT) & UART0LCRH_PEN_MASK)
#define UART0LCRH_PEN_GET(x)	(((x) & UART0LCRH_PEN_MASK) >> UART0LCRH_PEN_SHIFT)
#define UART0LCRH_BRK_SHIFT	(0)
#define UART0LCRH_BRK_MASK	(0x1u << UART0LCRH_BRK_SHIFT)
#define UART0LCRH_BRK_SET(x)	(((x) << UART0LCRH_BRK_SHIFT) & UART0LCRH_BRK_MASK)
#define UART0LCRH_BRK_GET(x)	(((x) & UART0LCRH_BRK_MASK) >> UART0LCRH_BRK_SHIFT)

/* UART0CTL field accessors */
#define UART0CTL_RXE_SHIFT	(9)
#define UART0CTL_RXE_MASK	(0x1u << UART0CTL_RXE_SHIFT)
#define UART0CTL_RXE_SET(x)	(((x) << UART0CTL_RXE_SHIFT) & UART0CTL_RXE_MASK)
#define UART0CTL_RXE_GET(x)	(((x) & UART0CTL_RXE_MASK) >> UART0CTL_RXE_SHIFT)
#define UART0CTL_TXE_SHIFT	(8)
#define UART0CTL_TXE_MASK	(0x1u << UART0CTL_TXE_SHIFT)
#define UART0CTL_TXE_SET(x)	(((x) << UART0CTL_TXE_SHIFT) & UART0CTL_TXE_MASK)
#define UART0CTL_TXE_GET(x)	(((x) & UART0CTL_TXE_MASK) >> UART0CTL_TXE_SHIFT)
#define UART0CTL_LBE_SHIFT	(7)
#define UART0CTL_LBE_MASK	(0x1u << UART0CTL_LBE_SHIFT)
#define UART0CTL_LBE_SET(x)	(((x) << UART0CTL_LBE_SHIFT) & UART0CTL_LBE_MASK)
#define UART0CTL_LBE_GET(x)	(((x) & UART0CTL_LBE_MASK) >> UART0CTL_LBE_SHIFT)
#define UART0CTL_SIRLP_SHIFT	(2)
#define UART0CTL_SIRLP_MASK	(0x1u << UART0CTL_SIRLP_SHIFT)
#define UART0CTL_SIRLP_SET(x)	(((x) << UART0CTL_SIRLP_SHIFT) & UART0CTL_SIRLP_MASK)
#define UART0CTL_SIRLP_GET(x)	(((x) & UART0CTL_SIRLP_MASK) >> UART0CTL_SIRLP_SHIFT)
#define UART0CTL_SIREN_SHIFT	(1)
#define UART0CTL_SIREN_MASK	(0x1u << UART0CTL_SIREN_SHIFT)
#define UART0CTL_SIREN_SET(x)	(((x) << UART0CTL_SIREN_SHIFT) & UART0CTL_SIREN_MASK)
#define UART0CTL_SIREN_GET(x)	(((x) & UART0CTL_SIREN_MASK) >> UART0CTL_SIREN_SHIFT)
#define UART0CTL_UARTEN_SHIFT	(0)
#define UART0CTL_UARTEN_MASK	(0x1u << UART0CTL_UARTEN_SHIFT)
#define UART0CTL_UARTEN_SET(x)	(((x) << UART0CTL_UARTEN_SHIFT) & UART0CTL_UARTEN_MASK)
#define UART0CTL_UARTEN_GET(x)	(((x) & UART0CTL_UARTEN_MASK) >> UART0CTL_UARTEN_SHIFT)

/*
 * Initialize UART0.
 */
void __attribute__((section("privileged_functions"), noinline))
UART0_Init(void)
{
	float brd = (float)SystemCoreClock / (16 * 115200);
	uint32_t ibrd = (uint32_t)brd;
	uint32_t fbrd = (uint32_t)((brd - ibrd) * 64 + 0.5);

	/* Disable UART0 */
	UART0CTL &= ~UART0CTL_UARTEN_SET(1);

	/* Write IBRD */
	UART0IBRD = UART0IBRD_DIVINT_SET(ibrd);

	/* Write FBRD */
	UART0FBRD = UART0FBRD_DIVFRAC_SET(fbrd);

	/* Write line control */
	UART0LCRH = UART0LCRH_SPS_SET(0) |
		    UART0LCRH_WLEN_SET(0x3u) |
		    UART0LCRH_FEN_SET(1) |
		    UART0LCRH_STP2_SET(0) |
		    UART0LCRH_EPS_SET(0) |
		    UART0LCRH_PEN_SET(0) |
		    UART0LCRH_BRK_SET(0);

	/* Enable UART0 with TX and RX */
	UART0CTL |= UART0CTL_RXE_SET(1) |
		    UART0CTL_TXE_SET(1) |
		    UART0CTL_UARTEN_SET(1);
}

/*
 * Low-level I/O routine called by _write() to output a character to an I/O
 * device.
 *
 * @ch: the character to output
 *
 * return: the character
 */
int
__io_putchar(int ch)
{
	while (UART0FR_TXFF_GET(UART0FR)) {
	}

	UART0DR = UART0DR_DATA_SET(ch);
	return ch;
}

/*
 * Low-level I/O routine called by _read() to read a character from an
 * I/O device.
 *
 * return: the character read
 */
int
__io_getchar(void)
{
	char ch;

	while (UART0FR_RXFE_GET(UART0FR)) {
	}
	ch = UART0DR_DATA_GET(UART0DR);

	/* Echo it back */
	__io_putchar(ch);

	return ch;
}

//=============================================================================
// MPU
//=============================================================================

/* MPU registers */
#define MPUTYPE		*(volatile uint32_t *)0xe000ed90
#define MPUCTRL		*(volatile uint32_t *)0xe000ed94
#define MPUNUMBER	*(volatile uint32_t *)0xe000ed98
#define MPUBASE		*(volatile uint32_t *)0xe000ed9c
#define MPUATTR		*(volatile uint32_t *)0xe000eda0

/* SCB register */
#define SYSHNDCTRL	*(volatile uint32_t *)0xe000ed24

/* MPUTYPE field accessors */
#define MPUTYPE_DREGION_SHIFT	(8)
#define MPUTYPE_DREGION_MASK	(0xffu << MPUTYPE_DREGION_SHIFT)
#define MPUTYPE_DREGION_SET(x)	(((x) << MPUTYPE_DREGION_SHIFT) & MPUTYPE_DREGION_MASK)
#define MPUTYPE_DREGION_GET(x)	(((x) & MPUTYPE_DREGION_MASK) >> MPUTYPE_DREGION_SHIFT)

/* MPUCTRL field accessors */
#define MPUCTRL_PRIVDEFENA_SHIFT	(2)
#define MPUCTRL_PRIVDEFENA_MASK		(0x1u << MPUCTRL_PRIVDEFENA_SHIFT)
#define MPUCTRL_PRIVDEFENA_SET(x)	(((x) << MPUCTRL_PRIVDEFENA_SHIFT) & MPUCTRL_PRIVDEFENA_MASK)
#define MPUCTRL_PRIVDEFENA_GET(x)	(((x) & MPUCTRL_PRIVDEFENA_MASK) >> MPUCTRL_PRIVDEFENA_SHIFT)
#define MPUCTRL_HFNMIENA_SHIFT		(1)
#define MPUCTRL_HFNMIENA_MASK		(0x1u << MPUCTRL_HFNMIENA_SHIFT)
#define MPUCTRL_HFNMIENA_SET(x)		(((x) << MPUCTRL_HFNMIENA_SHIFT) & MPUCTRL_HFNMIENA_MASK)
#define MPUCTRL_HFNMIENA_GET(x)		(((x) & MPUCTRL_HFNMIENA_MASK) >> MPUCTRL_HFNMIENA_SHIFT)
#define MPUCTRL_ENABLE_SHIFT		(0)
#define MPUCTRL_ENABLE_MASK		(0x1u << MPUCTRL_ENABLE_SHIFT)
#define MPUCTRL_ENABLE_SET(x)		(((x) << MPUCTRL_ENABLE_SHIFT) & MPUCTRL_ENABLE_MASK)
#define MPUCTRL_ENABLE_GET(x)		(((x) & MPUCTRL_ENABLE_MASK) >> MPUCTRL_ENABLE_SHIFT)

/* MPUNUMBER field accessors */
#define MPUNUMBER_NUMBER_SHIFT	(0)
#define MPUNUMBER_NUMBER_MASK	(0x7u << MPUNUMBER_NUMBER_SHIFT)
#define MPUNUMBER_NUMBER_SET(x)	(((x) << MPUNUMBER_NUMBER_SHIFT) & MPUNUMBER_NUMBER_MASK)
#define MPUNUMBER_NUMBER_GET(x)	(((x) & MPUNUMBER_NUMBER_MASK) >> MPUNUMBER_NUMBER_SHIFT)

/* MPUBASE field accessors */
#define MPUBASE_ADDR_SHIFT	(5)
#define MPUBASE_ADDR_MASK	(0x7ffffffu << MPUBASE_ADDR_SHIFT)
#define MPUBASE_ADDR_SET(x)	(((x) << MPUBASE_ADDR_SHIFT) & MPUBASE_ADDR_MASK)
#define MPUBASE_ADDR_GET(x)	(((x) & MPUBASE_ADDR_MASK) >> MPUBASE_ADDR_SHIFT)
#define MPUBASE_VALID_SHIFT	(4)
#define MPUBASE_VALID_MASK	(0x1u << MPUBASE_VALID_SHIFT)
#define MPUBASE_VALID_SET(x)	(((x) << MPUBASE_VALID_SHIFT) & MPUBASE_VALID_MASK)
#define MPUBASE_VALID_GET(x)	(((x) & MPUBASE_VALID_MASK) >> MPUBASE_VALID_SHIFT)
#define MPUBASE_REGION_SHIFT	(0)
#define MPUBASE_REGION_MASK	(0x7u << MPUBASE_REGION_SHIFT)
#define MPUBASE_REGION_SET(x)	(((x) << MPUBASE_REGION_SHIFT) & MPUBASE_REGION_MASK)
#define MPUBASE_REGION_GET(x)	(((x) & MPUBASE_REGION_MASK) >> MPUBASE_REGION_SHIFT)

/* MPUATTR field accessors */
#define MPUATTR_XN_SHIFT	(28)
#define MPUATTR_XN_MASK		(0x1u << MPUATTR_XN_SHIFT)
#define MPUATTR_XN_SET(x)	(((x) << MPUATTR_XN_SHIFT) & MPUATTR_XN_MASK)
#define MPUATTR_XN_GET(x)	(((x) & MPUATTR_XN_MASK) >> MPUATTR_XN_SHIFT)
#define MPUATTR_AP_SHIFT	(24)
#define MPUATTR_AP_MASK		(0x7u << MPUATTR_AP_SHIFT)
#define MPUATTR_AP_SET(x)	(((x) << MPUATTR_AP_SHIFT) & MPUATTR_AP_MASK)
#define MPUATTR_AP_GET(x)	(((x) & MPUATTR_AP_MASK) >> MPUATTR_AP_SHIFT)
#define MPUATTR_TEX_SHIFT	(19)
#define MPUATTR_TEX_MASK	(0x7u << MPUATTR_TEX_SHIFT)
#define MPUATTR_TEX_SET(x)	(((x) << MPUATTR_TEX_SHIFT) & MPUATTR_TEX_MASK)
#define MPUATTR_TEX_GET(x)	(((x) & MPUATTR_TEX_MASK) >> MPUATTR_TEX_SHIFT)
#define MPUATTR_S_SHIFT		(18)
#define MPUATTR_S_MASK		(0x1u << MPUATTR_S_SHIFT)
#define MPUATTR_S_SET(x)	(((x) << MPUATTR_S_SHIFT) & MPUATTR_S_MASK)
#define MPUATTR_S_GET(x)	(((x) & MPUATTR_S_MASK) >> MPUATTR_S_SHIFT)
#define MPUATTR_C_SHIFT		(17)
#define MPUATTR_C_MASK		(0x1u << MPUATTR_C_SHIFT)
#define MPUATTR_C_SET(x)	(((x) << MPUATTR_C_SHIFT) & MPUATTR_C_MASK)
#define MPUATTR_C_GET(x)	(((x) & MPUATTR_C_MASK) >> MPUATTR_C_SHIFT)
#define MPUATTR_B_SHIFT		(16)
#define MPUATTR_B_MASK		(0x1u << MPUATTR_B_SHIFT)
#define MPUATTR_B_SET(x)	(((x) << MPUATTR_B_SHIFT) & MPUATTR_B_MASK)
#define MPUATTR_B_GET(x)	(((x) & MPUATTR_B_MASK) >> MPUATTR_B_SHIFT)
#define MPUATTR_SRD_SHIFT	(8)
#define MPUATTR_SRD_MASK	(0xffu << MPUATTR_SRD_SHIFT)
#define MPUATTR_SRD_SET(x)	(((x) << MPUATTR_SRD_SHIFT) & MPUATTR_SRD_MASK)
#define MPUATTR_SRD_GET(x)	(((x) & MPUATTR_SRD_MASK) >> MPUATTR_SRD_SHIFT)
#define MPUATTR_SIZE_SHIFT	(1)
#define MPUATTR_SIZE_MASK	(0x1fu << MPUATTR_SIZE_SHIFT)
#define MPUATTR_SIZE_SET(x)	(((x) << MPUATTR_SIZE_SHIFT) & MPUATTR_SIZE_MASK)
#define MPUATTR_SIZE_GET(x)	(((x) & MPUATTR_SIZE_MASK) >> MPUATTR_SIZE_SHIFT)
#define MPUATTR_ENABLE_SHIFT	(0)
#define MPUATTR_ENABLE_MASK	(0x1u << MPUATTR_ENABLE_SHIFT)
#define MPUATTR_ENABLE_SET(x)	(((x) << MPUATTR_ENABLE_SHIFT) & MPUATTR_ENABLE_MASK)
#define MPUATTR_ENABLE_GET(x)	(((x) & MPUATTR_ENABLE_MASK) >> MPUATTR_ENABLE_SHIFT)

/* SYSHNDCTRL field accessors */
#define SYSHNDCTRL_USAGE_SHIFT		(18)
#define SYSHNDCTRL_USAGE_MASK		(0x1u << SYSHNDCTRL_USAGE_SHIFT)
#define SYSHNDCTRL_USAGE_SET(x)		(((x) << SYSHNDCTRL_USAGE_SHIFT) & SYSHNDCTRL_USAGE_MASK)
#define SYSHNDCTRL_USAGE_GET(x)		(((x) & SYSHNDCTRL_USAGE_MASK) >> SYSHNDCTRL_USAGE_SHIFT)
#define SYSHNDCTRL_BUS_SHIFT		(17)
#define SYSHNDCTRL_BUS_MASK		(0x1u << SYSHNDCTRL_BUS_SHIFT)
#define SYSHNDCTRL_BUS_SET(x)		(((x) << SYSHNDCTRL_BUS_SHIFT) & SYSHNDCTRL_BUS_MASK)
#define SYSHNDCTRL_BUS_GET(x)		(((x) & SYSHNDCTRL_BUS_MASK) >> SYSHNDCTRL_BUS_SHIFT)
#define SYSHNDCTRL_MEM_SHIFT		(16)
#define SYSHNDCTRL_MEM_MASK		(0x1u << SYSHNDCTRL_MEM_SHIFT)
#define SYSHNDCTRL_MEM_SET(x)		(((x) << SYSHNDCTRL_MEM_SHIFT) & SYSHNDCTRL_MEM_MASK)
#define SYSHNDCTRL_MEM_GET(x)		(((x) & SYSHNDCTRL_MEM_MASK) >> SYSHNDCTRL_MEM_SHIFT)
#define SYSHNDCTRL_SVC_SHIFT		(15)
#define SYSHNDCTRL_SVC_MASK		(0x1u << SYSHNDCTRL_SVC_SHIFT)
#define SYSHNDCTRL_SVC_SET(x)		(((x) << SYSHNDCTRL_SVC_SHIFT) & SYSHNDCTRL_SVC_MASK)
#define SYSHNDCTRL_SVC_GET(x)		(((x) & SYSHNDCTRL_SVC_MASK) >> SYSHNDCTRL_SVC_SHIFT)
#define SYSHNDCTRL_BUSP_SHIFT		(14)
#define SYSHNDCTRL_BUSP_MASK		(0x1u << SYSHNDCTRL_BUSP_SHIFT)
#define SYSHNDCTRL_BUSP_SET(x)		(((x) << SYSHNDCTRL_BUSP_SHIFT) & SYSHNDCTRL_BUSP_MASK)
#define SYSHNDCTRL_BUSP_GET(x)		(((x) & SYSHNDCTRL_BUSP_MASK) >> SYSHNDCTRL_BUSP_SHIFT)
#define SYSHNDCTRL_MEMP_SHIFT		(13)
#define SYSHNDCTRL_MEMP_MASK		(0x1u << SYSHNDCTRL_MEMP_SHIFT)
#define SYSHNDCTRL_MEMP_SET(x)		(((x) << SYSHNDCTRL_MEMP_SHIFT) & SYSHNDCTRL_MEMP_MASK)
#define SYSHNDCTRL_MEMP_GET(x)		(((x) & SYSHNDCTRL_MEMP_MASK) >> SYSHNDCTRL_MEMP_SHIFT)
#define SYSHNDCTRL_USAGEP_SHIFT		(12)
#define SYSHNDCTRL_USAGEP_MASK		(0x1u << SYSHNDCTRL_USAGEP_SHIFT)
#define SYSHNDCTRL_USAGEP_SET(x)	(((x) << SYSHNDCTRL_USAGEP_SHIFT) & SYSHNDCTRL_USAGEP_MASK)
#define SYSHNDCTRL_USAGEP_GET(x)	(((x) & SYSHNDCTRL_USAGEP_MASK) >> SYSHNDCTRL_USAGEP_SHIFT)
#define SYSHNDCTRL_TICK_SHIFT		(11)
#define SYSHNDCTRL_TICK_MASK		(0x1u << SYSHNDCTRL_TICK_SHIFT)
#define SYSHNDCTRL_TICK_SET(x)		(((x) << SYSHNDCTRL_TICK_SHIFT) & SYSHNDCTRL_TICK_MASK)
#define SYSHNDCTRL_TICK_GET(x)		(((x) & SYSHNDCTRL_TICK_MASK) >> SYSHNDCTRL_TICK_SHIFT)
#define SYSHNDCTRL_PNDSV_SHIFT		(10)
#define SYSHNDCTRL_PNDSV_MASK		(0x1u << SYSHNDCTRL_PNDSV_SHIFT)
#define SYSHNDCTRL_PNDSV_SET(x)		(((x) << SYSHNDCTRL_PNDSV_SHIFT) & SYSHNDCTRL_PNDSV_MASK)
#define SYSHNDCTRL_PNDSV_GET(x)		(((x) & SYSHNDCTRL_PNDSV_MASK) >> SYSHNDCTRL_PNDSV_SHIFT)
#define SYSHNDCTRL_MON_SHIFT		(8)
#define SYSHNDCTRL_MON_MASK		(0x1u << SYSHNDCTRL_MON_SHIFT)
#define SYSHNDCTRL_MON_SET(x)		(((x) << SYSHNDCTRL_MON_SHIFT) & SYSHNDCTRL_MON_MASK)
#define SYSHNDCTRL_MON_GET(x)		(((x) & SYSHNDCTRL_MON_MASK) >> SYSHNDCTRL_MON_SHIFT)
#define SYSHNDCTRL_SVCA_SHIFT		(7)
#define SYSHNDCTRL_SVCA_MASK		(0x1u << SYSHNDCTRL_SVCA_SHIFT)
#define SYSHNDCTRL_SVCA_SET(x)		(((x) << SYSHNDCTRL_SVCA_SHIFT) & SYSHNDCTRL_SVCA_MASK)
#define SYSHNDCTRL_SVCA_GET(x)		(((x) & SYSHNDCTRL_SVCA_MASK) >> SYSHNDCTRL_SVCA_SHIFT)
#define SYSHNDCTRL_USGA_SHIFT		(3)
#define SYSHNDCTRL_USGA_MASK		(0x1u << SYSHNDCTRL_USGA_SHIFT)
#define SYSHNDCTRL_USGA_SET(x)		(((x) << SYSHNDCTRL_USGA_SHIFT) & SYSHNDCTRL_USGA_MASK)
#define SYSHNDCTRL_USGA_GET(x)		(((x) & SYSHNDCTRL_USGA_MASK) >> SYSHNDCTRL_USGA_SHIFT)
#define SYSHNDCTRL_BUSA_SHIFT		(1)
#define SYSHNDCTRL_BUSA_MASK		(0x1u << SYSHNDCTRL_BUSA_SHIFT)
#define SYSHNDCTRL_BUSA_SET(x)		(((x) << SYSHNDCTRL_BUSA_SHIFT) & SYSHNDCTRL_BUSA_MASK)
#define SYSHNDCTRL_BUSA_GET(x)		(((x) & SYSHNDCTRL_BUSA_MASK) >> SYSHNDCTRL_BUSA_SHIFT)
#define SYSHNDCTRL_MEMA_SHIFT		(0)
#define SYSHNDCTRL_MEMA_MASK		(0x1u << SYSHNDCTRL_MEMA_SHIFT)
#define SYSHNDCTRL_MEMA_SET(x)		(((x) << SYSHNDCTRL_MEMA_SHIFT) & SYSHNDCTRL_MEMA_MASK)
#define SYSHNDCTRL_MEMA_GET(x)		(((x) & SYSHNDCTRL_MEMA_MASK) >> SYSHNDCTRL_MEMA_SHIFT)

/* Enumeration of MPUATTR's AP field values */
enum {
	AP_NO_ACCESS = 0,
	AP_PRW = 1,
	AP_PRW_URO = 2,
	AP_PRW_URW = 3,
	AP_PRO = 5,
	AP_PRO_URO = 6,
};

/* Region boundaries defined by the linker script */
extern uint8_t * _FLASH_start;
extern uint8_t * _FLASH_end;
extern uint8_t * _CODE_start;
extern uint8_t * _CODE_end;
extern uint8_t * _RAM_start;
extern uint8_t * _RAM_end;

/* Our MPU region numbers */
enum {
	RN_FLASH = 0,	/* Flash:  U - RO, P - RO, XN */
 	RN_RAM,		/* RAM:    U - RO, P - RW, XN */
 	RN_DATA,	/* Data:   U - RW, P - RW, XN */
 	RN_PERIPH,	/* Periph: U - RW, P - RW, XN */
 	RN_CODE,	/* Code:   U - RO, P - RO, X */

	NUM_REGIONS,
};

/*
 * Determine if a number is a power of two.
 *
 * @n: the number
 *
 * return: 1 if true, 0 otherwise
 */
static uint32_t
IsPowerOf2(uint32_t n)
{
	return n != 0 && (n & (n - 1)) == 0;
}

/*
 * Encode an MPU region size to MPUATTR's encoding.
 *
 * @size: the MPU region size
 *
 * return: the MPU region size in MPUATTR's encoding
 */
static uint32_t
EncodeMPURegionSize(uint32_t size)
{
	uint32_t i = 4;

	assert(size >= 32 && IsPowerOf2(size) && "Invalid MPU region size");
	while (size > 32) {
		size >>= 1;
		++i;
	}

	return i;
}

/*
 * Initialize the MPU.
 */
void __attribute__((section("privileged_functions"), noinline))
MPU_Init(void)
{
	uint32_t _periph_start = 0x40000000u;
	uint32_t _periph_end = 0x44000000u;

	uint32_t size;

	/* Make sure the MPU supports enough regions for us */
	assert(MPUTYPE_DREGION_GET(MPUTYPE) >= NUM_REGIONS &&
	       "No enough MPU regions!");

	/* Setup the MPU region for FLASH */
	size = (uint32_t)&_FLASH_end - (uint32_t)&_FLASH_start;
	assert(((uint32_t)&_FLASH_start & (size - 1)) == 0 &&
	       "Unaligned start address!");
	MPUBASE = MPUBASE_ADDR_SET((uint32_t)&_FLASH_start >> 5) |
		  MPUBASE_VALID_SET(1) |
		  MPUBASE_REGION_SET(RN_FLASH);
	MPUATTR = MPUATTR_XN_SET(1) |
		  MPUATTR_AP_SET(AP_PRO_URO) |
		  MPUATTR_TEX_SET(0x0) |
		  MPUATTR_S_SET(1) |
		  MPUATTR_C_SET(1) |
		  MPUATTR_B_SET(1) |
		  MPUATTR_SRD_SET(0x0) |
		  MPUATTR_SIZE_SET(EncodeMPURegionSize(size)) |
		  MPUATTR_ENABLE_SET(1);

	/* Setup the MPU region for RAM */
	size = (uint32_t)&_RAM_end - (uint32_t)&_RAM_start;
	assert(((uint32_t)&_RAM_start & (size - 1)) == 0 &&
	       "Unaligned start address!");
	MPUBASE = MPUBASE_ADDR_SET((uint32_t)&_RAM_start >> 5) |
		  MPUBASE_VALID_SET(1) |
		  MPUBASE_REGION_SET(RN_RAM);
	MPUATTR = MPUATTR_XN_SET(1) |
		  MPUATTR_AP_SET(AP_PRW_URO) |
		  MPUATTR_TEX_SET(0x0) |
		  MPUATTR_S_SET(1) |
		  MPUATTR_C_SET(1) |
		  MPUATTR_B_SET(1) |
		  MPUATTR_SRD_SET(0x0) |
		  MPUATTR_SIZE_SET(EncodeMPURegionSize(size)) |
		  MPUATTR_ENABLE_SET(1);

	/* Setup the MPU region for R/W data */
	MPUBASE = MPUBASE_ADDR_SET((uint32_t)&_RAM_start >> 5) |
		  MPUBASE_VALID_SET(1) |
		  MPUBASE_REGION_SET(RN_DATA);
	MPUATTR = MPUATTR_XN_SET(1) |
		  MPUATTR_AP_SET(AP_PRW_URW) |
		  MPUATTR_TEX_SET(0x0) |
	       	  MPUATTR_S_SET(1) |
		  MPUATTR_C_SET(1) |
		  MPUATTR_B_SET(1) |
		  MPUATTR_SRD_SET(0x80) | /* Disable last subregion */
		  MPUATTR_SIZE_SET(EncodeMPURegionSize(size)) |
		  MPUATTR_ENABLE_SET(1);

	/* Setup the MPU region for peripherals */
	size = _periph_end - _periph_start;
	assert((_periph_start & (size - 1)) == 0 &&
	       "Unaligned start address!");
	MPUBASE = MPUBASE_ADDR_SET(_periph_start >> 5) |
		  MPUBASE_VALID_SET(1) |
		  MPUBASE_REGION_SET(RN_PERIPH);
	MPUATTR = MPUATTR_XN_SET(1) |
		  MPUATTR_AP_SET(AP_PRW_URW) |
		  MPUATTR_TEX_SET(0x0) |
		  MPUATTR_S_SET(0) |
		  MPUATTR_C_SET(0) |
		  MPUATTR_B_SET(1) |
		  MPUATTR_SRD_SET(0x0) |
		  MPUATTR_SIZE_SET(EncodeMPURegionSize(size)) |
		  MPUATTR_ENABLE_SET(1);

	/* Setup the MPU region for code */
	size = (uint32_t)&_CODE_end - (uint32_t)&_CODE_start;
	assert(((uint32_t)&_CODE_start & (size - 1)) == 0 &&
	       "Unaligned start address!");
	MPUBASE = MPUBASE_ADDR_SET((uint32_t)&_CODE_start >> 5) |
		  MPUBASE_VALID_SET(1) |
		  MPUBASE_REGION_SET(RN_CODE);
	MPUATTR = MPUATTR_XN_SET(0) |
		  MPUATTR_AP_SET(AP_PRO_URO) |
		  MPUATTR_TEX_SET(0x0) |
		  MPUATTR_S_SET(1) |
		  MPUATTR_C_SET(1) |
		  MPUATTR_B_SET(1) |
		  MPUATTR_SRD_SET(0x0) |
		  MPUATTR_SIZE_SET(EncodeMPURegionSize(size)) |
		  MPUATTR_ENABLE_SET(1);

	/*
	 * Enable MemManage fault.
	 */
	SYSHNDCTRL |= SYSHNDCTRL_MEM_SET(1);

	/*
	 * Now enable the MPU with:
	 *
	 * (1) Default memory map as a background region for privileged-only
	 *     access;
	 *
	 * (2) Using MPU for memory accesses by HardFault and NMI handlers.
	 */
	MPUCTRL = MPUCTRL_PRIVDEFENA_SET(1) |
		  MPUCTRL_HFNMIENA_SET(1) |
		  MPUCTRL_ENABLE_SET(1);
}

//=============================================================================
// Initialization
//=============================================================================

/*
 * Initialization routine called by __libc_init_array().
 */
void __attribute__((constructor))
Init(void)
{
	/* Initialize the system clock */
	Clock_Init();

	/* Initialize SysTick */
	SysTick_Init();

	/* Initialize UART0 */
	UART0_Init();

#ifdef SILHOUETTE
	/* Initialize MPU */
	MPU_Init();
#endif
}
