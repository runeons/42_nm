#include <nm_functions.h>

static void     display_sym_64(t_data *dt, t_sym *sym)
{
    if (sym->value == 0 && (dt->ehdr64->e_type == ET_REL) && !ft_strcmp(sym->name, ""))
        return ;
    if (sym->raw64->st_shndx == SHN_UNDEF)
        printf("%16c %c %s\n", ' ', sym->type, sym->name);
    else
        printf("%016"PFx_64" %c %s\n", sym->value, sym->type, sym->name);
}

static void     display_sym_32(t_data *dt, t_sym *sym)
{
    if (sym->value == 0 && (dt->ehdr32->e_type == ET_REL) && !ft_strcmp(sym->name, ""))
        return ;
    if (sym->raw32->st_shndx == SHN_UNDEF)
        printf("%8c %c %s\n", ' ', sym->type, sym->name);
    else
        printf("%08"PFx_64" %c %s\n", sym->value, sym->type, sym->name);
}

static int      apply_filter(t_sym *sym, int filter)
{
    if (filter == F_UNDEFINED_ONLY)
    {
        if (sym->type != 'U' && sym->type != 'w')
            return 1;
        if (sym->type == 'a')
            return 1;
        if (sym->name && sym->name[0] == '.')
            return 1;
    }
    else if (filter == F_EXTERN_ONLY)
    {
        if ((sym->type < 'A' || sym->type > 'Z') && sym->type != 'w')
            return 1;
        if (sym->type == 'a')
            return 1;
        if (sym->name && sym->name[0] == '.')
            return 1;
    }
    else if (filter == F_NO_DEBUG)
    {
        if (sym->type == 'a')
            return 1;
        if (sym->name && sym->name[0] == '.')
            return 1;
    }
    return 0;
}

static int      exception(t_sym *sym)
{
    if (sym->value == 0 && sym->type == 'a')
        return 0;
    if (!ft_strcmp(sym->name, ""))
        return 1;
    if (sym->value == 0 && sym->type == 'U' && !ft_strcmp(sym->name, ""))
        return 1;
    return 0;
}

static void     display_one_sym(t_data *dt, t_sym *sym, int filter)
{
    if (apply_filter(sym, filter))
        return;
    if (exception(sym))
        return;
    if (dt->arch == ELF_TYPE_64)
        display_sym_64(dt, sym);
    else if (dt->arch == ELF_TYPE_32)
        display_sym_32(dt, sym);
}

void            display_syms(t_data *dt, t_lst *syms, int filter)
{
    t_lst *current = syms;

    while (current != NULL)
    {
        t_sym *sym = (t_sym *)current->content;
        display_one_sym(dt, sym, filter);
        current = current->next;
    }   
}
