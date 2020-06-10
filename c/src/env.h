#ifndef ENV_H
#define ENV_H

static void ext_env(char* symbol, lval val, Env *env) {
    Env e = malloc(sizeof(struct Env));
    e->value = val;
    e->symbol = strdup(symbol);
    e->next = *env;
    *env = e;
}

static void print_env(Env env) {
    puts("---start---");
    while(env) {
        printf("%s, ", env->symbol);
        println_lval(env->value);
        env = env->next;
    }
    puts("---end---");
}

#endif
