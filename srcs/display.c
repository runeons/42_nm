#include <nm_functions.h>

void    display_one_sym(t_sym *sym, int filter)
{
    if (sym)
    {
        if (filter == F_UNDEFINED_ONLY)
        {
            if (sym->type != 'U' && sym->type != 'w')
                return;
            if (sym->type == 'a')
                return;
        }
        else if (filter == F_EXTERN_ONLY)
        {
            if ((sym->type < 'A' || sym->type > 'Z') && sym->type != 'w')
                return;
            if (sym->type == 'a')
                return;
        }
        else if (filter == F_NO_DEBUG)
        {
            if (sym->type == 'a')
                return;
        }
        if (sym->value == 0 && sym->type == 'U' && !ft_strcmp(sym->name, ""))
            return ;
        else if (sym->value == 0 && sym->type != 'a')
            printf("%16c %c %s\n", ' ', sym->type, sym->name);
        else
            printf("%016"PFu_64" %c %s\n", sym->value, sym->type, sym->name);
        // debug_one_sym(*(sym->raw));
    }

}

void    display_syms(t_lst *syms, int filter)
{
    t_lst *current = syms;

    // if (current != NULL)
    //     current = current->next;
    while (current != NULL)
    {
        t_sym *sym = (t_sym *)current->content;
        display_one_sym(sym, filter);
        current = current->next;
    }   
}
