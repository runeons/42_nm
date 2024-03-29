#include <nm_functions.h>

static char         get_type(const void *symbol)
{
    t_sym   *sym;
    
    sym = NULL;
    if (symbol == NULL)
        exit_corrupted("NULL symbol");
    sym = *(t_sym **)symbol;
    if (sym == NULL)
        exit_corrupted("NULL symbol");
    return (sym->type);
}

int          compare_types(const void *a, const void *b)
{
    char    letter_a;
    char    letter_b;

    letter_a = get_type(a);
    letter_b = get_type(b);
    return (letter_a - letter_b);
}

static uint64_t     get_value(const void *symbol)
{
    t_sym   *sym;
    
    sym = NULL;
    if (symbol == NULL)
        exit_corrupted("NULL symbol");
    sym = *(t_sym **)symbol;
    if (sym == NULL)
        exit_corrupted("NULL symbol");
    return (sym->value);
}

int                 compare_values(const void *a, const void *b)
{
    char    value_a;
    char    value_b;

    value_a = get_value(a);
    value_b = get_value(b);
    return (value_a - value_b);
}
