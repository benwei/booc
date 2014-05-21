#ifndef _PARSE_H
#define _PARSE_H

enum tokens {
    NUMBER='n',
    TMULT='*',
    TDIV='/',
    TADD='+',
    TSUB='-',
    TLEFTSI='(',
    TRIGHSI=')',
    ASSIGN='=',
};

extern enum tokens token;
void *sum(void);
void process(const void *tree);
enum tokens scan(const char *buf);
void delete (void * tree);

#endif /* _PARSE_H */
