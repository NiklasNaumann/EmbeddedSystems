#include <sys/stat.h>
#include <errno.h>
#undef errno
extern int errno;

char *__env[1] = { 0 };

char **environ = __env;

int _getpid(void) {
    return 1;
}

int _wait(int *status) {
    errno = ECHILD;
    return -1;
}

int _execve(char *name, char **argv, char **env) {
    errno = ENOMEM;
    return -1;
}

int _fork(void) {
    errno = EAGAIN;
    return -1;
}

int _kill(int pid, int sig) {
    errno = EINVAL;
    return -1;
}


int _open(const char *name, int flags, int mode) {
    return -1;
}

int _close(int file) {
    return -1;
}

int _lseek(int file, int ptr, int dir) {
    return 0;
}

int _isatty(int file) {
    return 1;
}

int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _link(char *old, char *new) {
    errno = EMLINK;
    return -1;
}

int _unlink(char *name) {
    errno = ENOENT;
    return -1; 
}

int _write(int file, char *ptr, int len) {
    extern void uart_write(char c);
    for (int i = 0; i < len; i++) {
        uart_write(*ptr++);
    }
    return len;
}

int _read(int file, char *ptr, int len) {
    return 0;
}

void _exit(int code) {
    while(1);
}

void *_sbrk(int incr) {
    register char *stack_ptr asm ("sp");
    extern char _end;
    static char *heap_end = &_end;
    char *prev_heap_end;

    prev_heap_end = heap_end;
    if ((heap_end + incr) > stack_ptr) {
        errno = ENOMEM;
        return (void *) -1;
    }

    heap_end += incr;
    return prev_heap_end;
}
