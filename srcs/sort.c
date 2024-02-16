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

int compare_sym(const void *a, const void *b)
{
    int res;

    t_sym *sym_a = *(t_sym **)a;
    t_sym *sym_b = *(t_sym **)b;

    char *name_a = ft_strdup(sym_a->name);
    char *name_b = ft_strdup(sym_b->name);

    while (*name_a == '_')
        name_a++;
    while (*name_b == '_')
        name_b++;
    lower_if_needed(&name_a);
    lower_if_needed(&name_b);
    res = ft_strcmp(name_a, name_b);
    return res;
}

void sort_list(t_lst **syms)
{
    int     sym_count = ft_lst_size(*syms);
    if (sym_count > 1)
    {
        t_sym **sym_array = mmalloc(sym_count * sizeof(t_sym *));
        if (sym_array == NULL)
            exit_error("Malloc failure");
        t_lst   *current = *syms;
        for (int i = 0; i < sym_count; i++)
        {
            sym_array[i] = (t_sym *)current->content;
            current = current->next;
        }
        qsort(sym_array, sym_count, sizeof(t_sym *), compare_sym);
        current = *syms;
        for (int i = 0; i < sym_count; i++)
        {
            current->content = sym_array[i];
            current = current->next;
        }
    }
}
