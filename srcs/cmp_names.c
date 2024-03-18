#include <nm_functions.h>

static void     lower_if_needed(char **name)
{
    size_t len = ft_strlen(*name);

    for (size_t i = 0; i < len; i++)
    {
        if ((*name)[i] >= 'A' && (*name)[i] <= 'Z')
            (*name)[i] = (*name)[i] + 32;
    }
}

static int      is_special_char(char c)
{
    if (c >= '!' && c <= '~')
    {
        if (ft_isalnum(c) || (c == '$'))
		    return (0);
    }
	return (1);
}

static void     remove_special_chars(char **name)
{
    char *current = *name;
    char *result  = *name;

    while (*current != '\0')
    {
        if (is_special_char(*current) == 0)
        {
            *result = *current;
            result++;
        }
        current++;
    }
    *result = '\0';
}

static char    *get_cleaned_sym_name(const void *symbol)
{
    t_sym   *sym;
    char    *name;
    
    if (symbol == NULL)
        exit_corrupted("NULL symbol");
    sym = *(t_sym **)symbol;
    if (sym == NULL)
        exit_corrupted("NULL symbol");
    name = ft_strdup(sym->name);
    remove_special_chars(&name);
    lower_if_needed(&name);
    return (name);
}

static char    *get_raw_sym_name(const void *symbol)
{
    t_sym   *sym;
    char    *name;
    
    if (symbol == NULL)
        exit_corrupted("NULL symbol");
    sym = *(t_sym **)symbol;
    if (sym == NULL)
        exit_corrupted("NULL symbol");
    name = ft_strdup(sym->name);
    return (name);
}

int             compare_names(const void *a, const void *b)
{
    char    *name_a;
    char    *name_b;
    int     cmp;

    name_a = get_cleaned_sym_name(a);
    name_b = get_cleaned_sym_name(b);
    cmp = ft_strcmp(name_a, name_b);
    if (cmp == 0)
        return (ft_strcmp(get_raw_sym_name(a), get_raw_sym_name(b)));
    return (ft_strcmp(name_a, name_b));
}
