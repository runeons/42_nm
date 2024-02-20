#include <nm_functions.h>

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
