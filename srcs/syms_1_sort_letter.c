#include <nm_functions.h>

char    get_letter(const void *symbol)
{
    t_sym   *sym;
    
    sym = NULL;
    if (symbol == NULL)
        exit_corrupted("NULL symbol");
    sym = *(t_sym **)symbol;
    if (sym == NULL)
        exit_corrupted("NULL symbol");
    return (sym->letter);
}

int     compare_letters(const void *a, const void *b)
{
    char    letter_a;
    char    letter_b;

    letter_a = get_letter(a);
    letter_b = get_letter(b);
    return (letter_a - letter_b);
}
