#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

static char program[1024];

void readline(char* buf, FILE* fp) {
    char raw[255], *start;
    if (fgets(raw, 255, fp) == NULL) {
        exit(0);
    }
    for(start=raw;*start == ' ';++start) {}
    buf[0] = '\0';
    strcat(buf, start);
}

void count_bracket(char* buf, int* bracket) {
    int i;
    for(i=0;buf[i] != '\n';++i) {
        if (buf[i] == '(') {
            *bracket += 1;
        } else if (buf[i] == ')') {
            *bracket -= 1;
        }
    }
    buf[i] = ' ';
    strcat(program, buf);
}

void interp(FILE* fp) {
    char buf[255], *start, *next;
    int bracket, repl;
    Env env0 = NULL;
    expr s;
    lval val;
    while(!feof(fp)) {
        if (fp == stdin) {
            printf("LISP> ");
        }
        readline(buf, fp);
        bracket = 0;
        program[0] = '\0';

        if (strcmp(buf, "exit\n") == 0) {
            break;
        } else if (buf[0] != '\n') {
            count_bracket(buf, &bracket);
            while(bracket != 0) {
                readline(buf, fp);
                count_bracket(buf, &bracket);
            }
            start = program;
            s = parse(start, &next);
            val = NULL;
            while(s) {
                free(val);
                val = eval(s, &env0);
                start = next;
                s = parse(start, &next);
            }
            if (val != NULL) {
                if (fp == stdin) {
                    printf("=> ");
                    println_lval(val);
                }
                free(val);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    init_regex();
    init_builtin();
    if (argc == 1) {
        interp(stdin);
    } else {
        int i;
        FILE* fp;
        for(i=1;i<argc;++i) {
            fp = fopen(argv[i], "r");
            if (fp == NULL) {
                printf("Error: no such file %s\n", argv[i]);
            } else {
                interp(fp);
                fclose(fp);
            }
        }
    }
    return 0;
}

