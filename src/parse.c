/* Modifed ooc calculater with ooc parser
 * reference:
 *  - http://www.cs.rit.edu/~ats/books/ooc.pdf
 *  - http://www.cs.rit.edu/~ats/oop-2001-2/code/13/parse.c
 **/

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include "parse.h"

typedef void * any;

enum tokens token;
static double number;

void error (const char * fmt, ...);

struct Type {
    const char *name;
    void *(*new) (va_list ap);
    double (*exec) (const void *tree);
    void (*delete) (void *tree);
};

double exec(const void *tree)
{
    assert(tree && * (struct Type **) tree
            && (* (struct Type **) tree) -> exec);
    return (* (struct Type **) tree) -> exec(tree);
}

void process(const void *tree)
{
    double r = exec(tree);
    printf("process result = %g\n", r);
}

void *new(const void *type, ...)
{
    va_list ap;
    void *result;
    assert(type && ((struct Type *) type)->new);

    va_start(ap, type);

    result = ((struct Type *) type)->new(ap);

    *(const struct Type **) result = type;
    va_end(ap);
    return result;
}


void delete (void * tree)
{
    assert(tree && * (struct Type **) tree
       && (* (struct Type **) tree) -> delete);
    (* (struct Type **) tree)->delete(tree);
}

struct Bin {
    const void *type;
    void *left, *right;
};

static void * mkBin (va_list ap)
{
    struct Bin * node = malloc(sizeof(struct Bin));
    assert(node);
    node->left = va_arg(ap, any);
    node->right = va_arg(ap, any);
    return node;
}

static double doAdd (const void * tree)
{
    double r = exec(((struct Bin *) tree) -> left) +
        exec(((struct Bin *) tree) -> right);
    printf("doAdd => %g\n", r);
    return r;
}

static double doSub (const void * tree)
{
    return exec(((struct Bin *) tree) -> left) -
        exec(((struct Bin *) tree) -> right);
}

static double doMult (const void * tree)
{
    double r = exec(((struct Bin *) tree) -> left) *
        exec(((struct Bin *) tree) -> right);
    printf("doMult => %g\n", r);
    return r;
}

static double doDiv (const void * tree)
{
    double left = exec(((struct Bin *) tree) -> left);
    double right = exec(((struct Bin *) tree) -> right);

    if (right == 0.0)
        error("division by zero");
    return left / right;
}

static void freeBin (void * tree)
{
    delete(((struct Bin *) tree) -> left);
    delete(((struct Bin *) tree) -> right);
    free(tree);
}

static struct Type _Add = { "+", mkBin, doAdd, freeBin };
static struct Type _Sub = { "-", mkBin, doSub, freeBin };
static struct Type _Mult = { "*", mkBin, doMult, freeBin };
static struct Type _Div = { "/", mkBin, doDiv, freeBin };

const struct Type *Add = &_Add;
const struct Type *Sub = &_Sub;
const struct Type *Mult= &_Mult;
const struct Type *Div= &_Div;

struct Val {
    const void *type;
    double value;
};

static void *mkVal(va_list ap)
{
    struct Val *node = malloc(sizeof(struct Val));
    assert(node);
    node->value = va_arg(ap, double);
    return node;
}

static double doVal(const void *tree)
{
    return ((struct Val *) tree) -> value;
}

static struct Type _Value = {"v", mkVal, doVal, free };

const struct Type *Value = &_Value;

struct Un {
    const void * type;
    void *arg;
};

static void *mkUn (va_list ap)
{
    struct Un * node = malloc(sizeof(struct Un));
    assert(node);
    node->arg = va_arg(ap, any);
    return node;
}

static double doMinus(const void *tree)
{
    return -exec(((struct Un *) tree) -> arg);
}

static void freeUn(void *tree)
{
    delete(((struct Un *) tree)->arg);
    free(tree);
}

static struct Type _Minus = {"m", mkUn, doMinus, freeUn };

static double doAssign(const void *tree)
{
    double r = exec(((struct Un *) tree) -> arg);
    printf("=%g\n", r);
    return r;
}

static struct Type _Assign = {"=", mkUn, doAssign, freeUn };

const struct Type *Minus = &_Minus;
const struct Type *Assign = &_Assign;


static void *factor (void);

/*
 * product : factor { * | / factor } ...
 */
void *product(void)
{
    void *result = factor();
    const void * type;
    for (;;)
    {
        switch (token) {
            case '*':
                type = Mult;
                break;
            case '/':
                type = Div;
                break;
            default:
                return result;
        }
        scan(0);
        result = new(type, result, factor());
    }
}

enum tokens scan(const char *buf)
{
    static const char *bp;

    if (buf)
        bp = buf; /* new input line */
    printf("*bp=%c\n", *bp);

    while (isspace(*bp)) ++bp;
    if (isdigit(*bp & 0xff) || *bp == '.') {
        errno = 0;
        token = NUMBER;
        number = strtod(bp, (char **) & bp);
        if (errno == ERANGE)
            error("bad value: %s", strerror(errno));
    } else
        token = *bp ? *bp++ : 0;

    return token;
}

/*
 *  factor : + factor
 *           - factor
 *           NUMBER
 *           ( sum )
 */
static void *factor (void)
{
    void *result = NULL;
    printf("f-> %c\n", token);
    switch (token) {
        case '+':
            scan(0);
            return factor();
        case '-':
            scan(0);
            return new(Minus, factor());
        default:
            error("bad factor: '%c' 0x%x",
                    token, token);
        case NUMBER:
            printf("factor(number=%g)\n", number);
            result = new(Value, number);
            break;
        case '(':
            scan(0);
            result = sum();
            if (token != ')')
                error("expecting )");
            break;
        case '=':
            scan(0);
            result = new(Assign, number);
            break;
    }
    scan(0);
    return result;
}

/* sum : product { + | - product } ... */

void *sum(void)
{
    void *result = product();
    const void * type;
    for (;;)
    {
        switch (token) {
            case '+':
                type = Add;
                break;
            case '-':
                type = Sub;
                break;
            default:
                return result;
        }
        scan(0);
        result = new(type, result, product());
    }
}

