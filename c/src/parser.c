#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include "structs.h"

extern regex_t preg_int;
extern regmatch_t pmatch;

int whitespace(char c) {
    return c == ' ' || c == '\n';
}

int isbool(char* buf) {
    return (strcmp(buf, "#T") == 0 ||
            strcmp(buf, "#F") == 0 ||
            strcmp(buf, "#t") == 0 ||
            strcmp(buf, "#f") == 0);
}

void append(expr list, expr child) {
    list->len++;
    list->child = realloc(list->child, sizeof(expr) * list->len);
    list->child[list->len-1] = child;
}

expr parse_symbol(char* s, char** e) {
    expr ex = malloc(sizeof(s_expr));
    int i;
    char buf[256] = {0};
    
    i = 0;
    while(*s) {
        if (i >= 256) {
            fprintf(stderr, "symbol too long!\n");
            return NULL;
        } else {
            if (whitespace(*s)) {
                *e = s + 1;
                break;
            } else if (*s == ')'){
                *e = s;
                break;
            } else {
                buf[i++] = *s;
                s++;
            }
        }
    }
    if (regexec(&preg_int, buf, 1, &pmatch, 0) == 0) {
        ex->type = S_NUMBER;
        ex->value = atoi(buf);
    } else if (isbool(buf)) {
        ex->type = S_BOOL;
        if (buf[1] == 'T' || buf[1] == 't') {
            ex->value = 1;
        } else {
            ex->value = 0;
        }
    } else {
        ex->type = S_SYMBOL;
        ex->len = strlen(buf);
        ex->symbol = strdup(buf);
    }
    return ex;
}


expr parse_list(char* s, char **e) {
    expr ex = malloc(sizeof(s_expr));
    expr child;

    char *next;
    ex->len = 0;
    ex->child = NULL;

    while(*s) {
        if (whitespace(*s)) {
            s++;
        } else {
            switch(*s) {
                case ')':
                    *e = s + 1;
                    ex->type = S_LIST;
                    return ex;
                case '(':
                    child = parse_list(s+1, &next);
                    if (child == NULL) {
                        free(ex);
                        return NULL;
                    } else {
                        break;
                    }
                default:
                    child = parse_symbol(s, &next);
                    if (child == NULL) {
                        free(ex);
                        return NULL;
                    }
            }
            append(ex, child);
            s = next;
        }
    }
    free(ex);
    return NULL;
}

expr parse(char* s, char** e) {
    for(;*s;++s) {
        if (!whitespace(*s)) {
            switch(*s) {
                case '(':
                    return parse_list(s+1, e);
                default:
                    return parse_symbol(s, e);
            }
        }
    }
    return NULL;
}
