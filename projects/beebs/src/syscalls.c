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

/* Includes */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

/* Macro definitions */
#define USED(x) (x) = (x)

/* Forward prototypes */
int	_system		(const char *);
int	_rename		(const char *, const char *);
int	_isatty		(int);
clock_t _times		(struct tms *);
int	_gettimeofday	(struct timeval *, void *);
void	_raise		(void);
int	_unlink		(const char *);
int	_link		(const char *, const char *);
int	_stat		(const char *, struct stat *);
int	_fstat		(int, struct stat *);
void *	_sbrk		(intptr_t);
pid_t	_getpid		(void);
int	_kill		(pid_t, int);
void	_exit		(int);
int	_close		(int);
int	_open		(const char *, int, ...);
ssize_t	_write		(int, const void *, size_t);
off_t	_lseek		(int, off_t, int);
ssize_t	_read		(int, void *, size_t);
int	_wait		(int *);
pid_t	_fork		(void);
int	_execve		(const char *, char *const *, char *const *);
void	initialise_monitor_handles (void);

/* Variables */
extern int errno;

register char * stack_ptr asm ("sp");

char *__env[1] = { 0 };
char **environ = __env;

/* Functions */
int __attribute__((weak))
_system(const char *s)
{
	if (s == NULL)
		return 0;

	errno = ENOSYS;
	return -1;
}

int __attribute__((weak))
_rename(const char *oldpath, const char *newpath)
{
	USED(oldpath);
	USED(newpath);

	errno = ENOENT;
	return -1;
}

int __attribute__((weak))
_isatty(int file)
{
	if (file >= 0 && file <= 2) {
		return 1;
	}

	errno = EBADF;
 	return 0;
}

clock_t __attribute__((weak))
_times(struct tms *buf)
{
	USED(buf);

	errno = ENOSYS;
	return -1;
}

int __attribute__((weak))
_gettimeofday(struct timeval *tp, void *tzvp)
{
	USED(tp);
	USED(tzvp);

	errno = ENOSYS;
	return -1;
}

void __attribute__((weak))
_raise(void)
{
}

int __attribute__((weak))
_unlink(const char *name)
{
	USED(name);

	errno = ENOENT;
	return -1;
}

int __attribute__((weak))
_link(const char *old, const char *new)
{
	USED(old);
	USED(new);

	errno = ENOENT;
	return -1;
}

int __attribute__((weak))
_stat(const char *file, struct stat *st)
{
	USED(file);
	USED(st);

	errno = ENOENT;
	return -1;
}

int __attribute__((weak))
_fstat(int file, struct stat *st)
{
	if (file >= 0 && file <= 2) {
		st->st_mode = S_IFCHR;
		return 0;
	}

	errno = EBADF;
	return -1;
}

void * __attribute__((weak))
_sbrk(intptr_t incr)
{
	extern char end asm ("end"); /* Defined by the linker */
	static char * heap_end;
	char *        prev_heap_end;

	if (heap_end == NULL) {
		heap_end = &end;
	}

	prev_heap_end = heap_end;

	if (heap_end + incr > stack_ptr) {
		/*
		 * Some of the libstdc++-v3 tests rely upon detecting
		 * out of memory errors, so do not abort here.
		 */
#if 0
		extern void abort(void);

		_write(1, "_sbrk: Heap and stack collision\n", 32);

		abort();
#else
		errno = ENOMEM;
		return (void *)-1;
#endif
	}

	heap_end += incr;

	return (void *)prev_heap_end;
}

pid_t __attribute__((weak))
_getpid(void)
{
	return (pid_t)1;
}

int __attribute__((weak))
_kill(pid_t pid, int sig)
{
	USED(pid);
	USED(sig);

	errno = EPERM;
	return -1;
}

void __attribute__((weak))
_exit(int status)
{
	USED(status);

	_kill((pid_t)0, SIGABRT);
	while (1) {}	/* Make sure we hang here */
}

int __attribute__((weak))
_close(int file)
{
	USED(file);

	errno = EBADF;
	return -1;
}

int __attribute__((weak))
_open(const char *path, int flags, ...)
{
	USED(path);
	USED(flags);

	errno = ENOENT;
	return -1;
}

extern int __io_putchar(int ch) __attribute__((weak));

ssize_t __attribute__((weak))
_write(int file, const void *data, size_t len)
{
	const char *ptr = (const char *)data;
	size_t DataIdx;

	if (file < 1 || file > 2) {
		errno = EBADF;
		return -1;
	}

	for (DataIdx = 0; DataIdx < len; DataIdx++) {
		__io_putchar(*ptr++);
	}
	return len;
}

off_t __attribute__((weak))
_lseek(int file, off_t offset, int whence)
{
	USED(offset);
	USED(whence);

	if (file >= 0 && file <= 2) {
		errno = EINVAL;
	} else {
		errno = EBADF;
	}
	return (off_t)-1;
}

extern int __io_getchar(void) __attribute__((weak));

ssize_t __attribute__((weak))
_read(int file, void *buf, size_t len)
{
	char *ptr = (char *)buf;
	size_t DataIdx;

	if (file != 0) {
		errno = EBADF;
		return -1;
	}

	for (DataIdx = 0; DataIdx < len; DataIdx++) {
		*ptr++ = __io_getchar();
	}

	return len;
}

int __attribute__((weak))
_wait(int *status)
{
	USED(status);

	errno = ECHILD;
	return -1;
}

pid_t __attribute__((weak))
_fork(void)
{
	errno = ENOSYS;
	return -1;
}

int __attribute__((weak))
_execve(const char *name, char *const *argv, char *const *env)
{
	USED(name);
	USED(argv);
	USED(env);

	errno = ENOENT;
	return -1;
}

void __attribute__((weak))
initialise_monitor_handles(void)
{
}
