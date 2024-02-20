#include <nm_functions.h>

void lower_if_needed(char **name)
{
    size_t len = ft_strlen(*name);

    for (size_t i = 0; i < len; i++)
    {
        if ((*name)[i] >= 'A' && (*name)[i] <= 'Z')
            (*name)[i] = (*name)[i] + 32;
    }
}

int    is_special_char(char c)
{
    if (c >= 33 && c <= 126)
    {
        if (ft_isalnum(c))
		    return (0);
    }
	return (1);
}

void    remove_special_chars(char **name)
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

char    *get_cleaned_sym_str(const void *symbol)
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


int compare_sym(const void *a, const void *b)
{
    char *name_a;
    char *name_b;

    name_a = get_cleaned_sym_str(a);
    name_b = get_cleaned_sym_str(b);
    return (ft_strcmp(name_a, name_b));
}

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

int compare_letters(const void *a, const void *b)
{
    char    letter_a;
    char    letter_b;

    letter_a = get_letter(a);
    letter_b = get_letter(b);
    return (letter_a - letter_b);
}

t_sym   **copy_to_array(t_lst **syms, int sym_nb)
{
    t_sym   **sym_array;
    t_lst   *current;

    if ((sym_array = mmalloc(sym_nb * sizeof(t_sym *))) == NULL)
        exit_error("Malloc failure");
    current = *syms;
    for (int i = 0; i < sym_nb; i++)
    {
        sym_array[i] = (t_sym *)current->content;
        current = current->next;
    }
    return (sym_array);
}

void    copy_back_to_list(t_lst **syms, t_sym **sym_array, int sym_nb)
{
    t_lst   *current;

    current = *syms;
    for (int i = 0; i < sym_nb; i++)
    {
        current->content = sym_array[i];
        current = current->next;
    }
}

void    sort_syms(t_lst **syms)
{
    t_sym   **sym_array;
    int     sym_nb;

    if (syms == NULL)
        exit_error("No symbols");
    sym_nb = ft_lst_size(*syms);
    if (sym_nb > 1)
    {
        sym_array = copy_to_array(syms, sym_nb);
        qsort(sym_array, sym_nb, sizeof(t_sym *), compare_letters);
        qsort(sym_array, sym_nb, sizeof(t_sym *), compare_sym);
        copy_back_to_list(syms, sym_array, sym_nb);
    }
}
