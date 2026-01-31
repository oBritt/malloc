
#include "malloc.h"

static int ft_strcmp(char *str1, char *str2) {
    int ptr = 0;
    while (1)
    {
        if (str1[ptr] != str2[ptr]) {
            return str1[ptr] - str2[ptr];
        }
        if (str1[ptr] == 0) {
            break;
        }
        ptr++;
    }
    return 0;
}


static int env_true(const char *name)
{
    char *v = getenv(name);
    return v && (ft_strcmp(v, "1") == 0 || ft_strcmp(v, "true") == 0);
}


debug_config_t *get_debug(void)
{
    static int initialized = 0;
    static debug_config_t cfg;

    if (!initialized) {
        cfg.debug       = env_true("MALLOC_DEBUG");
        cfg.random_fail = env_true("MALLOC_RANDOM_FAIL");
        cfg.zero_alloc  = env_true("MALLOC_ZERO_ALLOC");

        initialized = 1;
        srand((unsigned int)time(NULL));
    }

    return &cfg;
}