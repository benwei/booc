#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include "parse.h"
#include <stdarg.h>
#include "blog.h"

BLOG_INIT_ENV()

static jmp_buf onError;
void error (const char * fmt, ...);

#define BCALC_VERSION "0.1"
#define BCALC_BUILD   "1000"
#define BCALC_NAME    "bcalc"

static inline void show_top_banner()
{
    printf("%s version %s.%s\n", BCALC_NAME, BCALC_VERSION, BCALC_BUILD);
}

static inline void calc_line_by_line_from_stdin(void)
{
    char buf [BUFSIZ] = {0};
    BLOG("Blog is Enabled.\n");
    while (fgets(buf, BUFSIZ, stdin))
    {
        if (scan(buf)) {
            void *e = sum();
            if (token)
                error("trash after sum");
            process(e);
            delete(e);
        }
    }
}

int debug_level = 0;

static inline int parse_args(int argc, char **argv)
{
    int i = 1;
    for (; i < argc ; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'v':
                verbose++;
                break;
                case 'd':
                debug_level++;
                break;
            }
        }
    }
    return 0;
}

int main (int argc, char **argv)
{
    volatile int errors = 0;
    int rc = 0;
    if ((rc = parse_args(argc, argv))<0) {
        return rc;
    }

    if (debug_level > 0)
        show_top_banner();

    /* if there are error raised by error() called,
     * programm will back to next line */
    if (setjmp(onError))
        ++errors;

    calc_line_by_line_from_stdin();

    return errors > 0;
}

void error (const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap), putc('\n', stderr); va_end(ap);
    longjmp(onError, 1);
}
