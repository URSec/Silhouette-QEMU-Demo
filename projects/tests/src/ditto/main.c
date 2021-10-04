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

extern int __io_getchar(void);

char *
do_get_input(char * buf, size_t size)
{
	char * ptr = buf;

	while (ptr < buf + size - 1) {
		char ch = __io_getchar();
		*ptr++ = ch;

		if (ch == '\r') {
			break;
		}
	}

	*ptr++ = '\0';

	printf("\r\n");

	return buf;
}

char *
get_input_from_stdio(unsigned n)
{
	if (n > 0) {
		/*
		 * Recurse to make enough used stack space; the XORs here
		 * ensure the recurisve call doesn't get tail-call optimized.
		 */
		return (char *)((uintptr_t)get_input_from_stdio(n - 1) ^
		                (uintptr_t)&get_input_from_stdio ^
				(uintptr_t)&get_input_from_stdio);
	} else {
		char buf[16];

		return do_get_input(buf, sizeof(buf));
	}
}

int main(void)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	while (1) {
		printf("Please type a string:\r\n");
		printf("You typed: %s\r\n", get_input_from_stdio(100));
	}

	return 0;
}
