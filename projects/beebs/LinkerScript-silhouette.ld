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

ENTRY(ResetISR)

MEMORY
{
	FLASH (r) : ORIGIN = 0x00000000, LENGTH = 0x20000
	CODE (rx) : ORIGIN = 0x00020000, LENGTH = 0x20000
	RAM  (rw) : ORIGIN = 0x20000000, LENGTH = 0x10000
}

_FLASH_start = ORIGIN(FLASH);
_FLASH_end = ORIGIN(FLASH) + LENGTH(FLASH);
_CODE_start = ORIGIN(CODE);
_CODE_end = ORIGIN(CODE) + LENGTH(CODE);
_RAM_start = ORIGIN(RAM);
_RAM_end = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
	/* Vector Table */
	.isr_vector :
	{
		KEEP(*(.isr_vector))
	} > FLASH

	/* Global Section Table */
	.section_table : ALIGN(4)
	{
		__data_section_table = .;
		LONG(LOADADDR(.data));
		LONG(    ADDR(.data));
		LONG(  SIZEOF(.data));
		__data_section_table_end = .;
		__bss_section_table = .;
		LONG(    ADDR(.bss));
		LONG(  SIZEOF(.bss));
		__bss_section_table_end = .;
	} > FLASH

	/* Read-Only Data */
	.rodata : ALIGN(4)
	{
		*(.rodata .rodata.* .constdata .constdata.*)
	} > FLASH

	.eh_frame : ALIGN(4)
	{
		*(.eh_frame)
	} > FLASH

	/*
	 * For exception handling/unwind - some Newlib functions (in common
	 * with C++ and STDC++) use this.
	 */
	.ARM.extab : ALIGN(4)
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > FLASH

	.ARM.exidx : ALIGN(4)
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > FLASH

	/* Code */
	.text : ALIGN(4)
	{
		*(.text*)
		*(privileged_functions*)
	} > CODE =0x00BF00BF

	/* Data */
	.data : ALIGN(4)
	{
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);

		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);

		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);

		*(.data*)
	} > RAM AT> FLASH

	/* Bss */
	.bss : ALIGN(4)
	{
		*(.bss*)
		. = ALIGN(4);
		PROVIDE(end = .);
	} > RAM

	/* Reserve space for heap */
	_HeapSize = 0x4000;
	.heap (NOLOAD) : ALIGN(4)
	{
		. += _HeapSize;
	} > RAM

	/* Reserve space for stack */
	_StackSize = 0x2000;
	.stack ORIGIN(RAM) + LENGTH(RAM) - 2 * _StackSize (NOLOAD) : ALIGN(4)
	{
		. += _StackSize;
		PROVIDE(_StackTop = .);
	} > RAM

	/* Reserve space for shadow stack */
	.shadowstack ORIGIN(RAM) + LENGTH(RAM) - _StackSize (NOLOAD) : ALIGN(4)
	{
		. += _StackSize;
	} > RAM
}
