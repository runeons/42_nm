#include <nm_functions.h>

void    display_one_sym(t_data *dt, t_sym *sym, int filter)
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
        // if (!ft_strcmp(sym->name, "exit_error"))
        // {
        //     printf(C_G_BLUE"[QUICK DEBUG] sym->name: %s"C_RES"\n", sym->name);
        //     printf(C_G_BLUE"              sym->section_name: %s"C_RES"\n", sym->section_name);
        //     printf(C_G_BLUE"              sym->raw_type: %d"C_RES"\n", sym->raw_type);
        //     printf(C_G_BLUE"              sym->raw_bind: %d"C_RES"\n", sym->raw_bind);
        //     printf(C_G_RED"               sym->raw64->st_shndx: %d"C_RES"\n", sym->raw64->st_shndx);
        //     printf(C_G_RED"               sym->value: %ld"C_RES"\n", sym->value);
        //     printf(C_G_RED"               sym->type: %d"C_RES"\n", sym->type);
        // }
        if (sym->value == 0 && sym->type == 'U' && !ft_strcmp(sym->name, ""))
            return ;
        if (dt->arch == ELF_TYPE_64)
        {
            if (sym->value == 0 && (dt->ehdr64->e_type == ET_REL) && !ft_strcmp(sym->name, ""))
                return ;
            if (sym->raw64->st_shndx == SHN_UNDEF)
                printf("%16c %c %s\n", ' ', sym->type, sym->name);
            else
                printf("%016"PFx_64" %c %s\n", sym->value, sym->type, sym->name);
        }
        else if (dt->arch == ELF_TYPE_32)
        {
            if (sym->value == 0 && (dt->ehdr32->e_type == ET_REL) && !ft_strcmp(sym->name, ""))
                return ;
            if (sym->raw32->st_shndx == SHN_UNDEF)
                printf("%8c %c %s\n", ' ', sym->type, sym->name);
            else
                printf("%08"PFx_64" %c %s\n", sym->value, sym->type, sym->name);
        }
    }

}

void    display_syms(t_data *dt, t_lst *syms, int filter)
{
    t_lst *current = syms;

    while (current != NULL)
    {
        t_sym *sym = (t_sym *)current->content;
        display_one_sym(dt, sym, filter);
        current = current->next;
    }   
}
