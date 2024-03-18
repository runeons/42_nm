#include <nm_functions.h>

static t_sym    **copy_to_array(t_lst **syms, int sym_nb)
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

static void     copy_back_to_list(t_lst **syms, t_sym **sym_array, int sym_nb)
{
    t_lst   *current;

    current = *syms;
    for (int i = 0; i < sym_nb; i++)
    {
        current->content = sym_array[i];
        current = current->next;
    }
}

static void     swap(t_sym *a, t_sym *b)
{
    t_sym tmp;

    if (a == NULL || b == NULL)
        exit_corrupted("NULL symbol");
    tmp = *a;
    *a = *b;
    *b = tmp;
}

static void     sort(t_sym **sym_array, int sym_nb, cmp_function cmp, int sort_type)
{
    for (int i = 0; i < sym_nb - 1; i++)
    {
        for (int j = 0; j < sym_nb - i - 1; j++)
        {
            if (sort_type == S_REVERSE)
            {
                if (cmp(&sym_array[j], &sym_array[j + 1]) < 0)
                    swap(sym_array[j], sym_array[j + 1]);
            }
            else
            {
                if (cmp(&sym_array[j], &sym_array[j + 1]) > 0)
                    swap(sym_array[j], sym_array[j + 1]);
            }
        }
    }
}

void            sort_syms(t_lst **syms, int sort_type)
{
    t_sym   **sym_array;
    int     sym_nb;

    if (syms == NULL)
        exit_error("No symbols");
    if (sort_type == S_NO_SORT)
        return;
    sym_nb = ft_lst_size(*syms);
    if (sym_nb > 1)
    {
        sym_array = copy_to_array(syms, sym_nb);
        sort(sym_array, sym_nb, compare_values, sort_type);
        sort(sym_array, sym_nb, compare_types, sort_type);
        sort(sym_array, sym_nb, compare_names, sort_type);
        copy_back_to_list(syms, sym_array, sym_nb);
    }
}
