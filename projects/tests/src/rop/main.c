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

#include <stdio.h>
#include <string.h>

/* Attack payload and its size */
extern char payload[];
extern size_t payload_size;

char *
get_input(char * buf)
{
	/* A buffer overflow vulnerability */
	return memcpy(buf, payload, payload_size);
}

uintptr_t
foo(unsigned n)
{
	if (n > 0) {
		/*
		 * Recurse to make enough used stack space; the XORs here
		 * ensure the recurisve call doesn't get tail-call optimized.
		 */
		return foo(n - 1) ^ (uintptr_t)&foo ^ (uintptr_t)&foo;
	} else {
		char buf[16];

		return (uintptr_t)get_input(buf);
	}
}

int main(void)
{
	setbuf(stdout, NULL);

	printf("ROP example\n");

	/* The following printf() won't be called if the ROP attack succeeds */
	printf("Input: %s\n", (char *)foo(100));

	return 0;
}
