#include <nm_functions.h>

void    display_one_sym(t_sym *sym)
{
    if (sym)
    {
        if (sym->value == 0 && sym->letter == 'U' && !ft_strcmp(sym->name, ""))
            return ;
        else if (sym->value == 0 && sym->letter != 'a')
            printf("%16c %c %s\n", ' ', sym->letter, sym->name);
        else
            printf("%016"PFu_64" %c %s\n", sym->value, sym->letter, sym->name);
        // debug_one_sym(*(sym->raw));
    }

}

void    display_syms(t_lst *syms)
{
    t_lst *current = syms;

    if (current != NULL)
        current = current->next;
    while (current != NULL)
    {
        t_sym *sym = (t_sym *)current->content;
        display_one_sym(sym);
        current = current->next;
    }   
}
