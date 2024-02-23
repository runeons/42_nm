#include <nm_functions.h>

void    create_new_sym(t_data *dt, int index)
{
    t_sym *new_sym = NULL;

    if ((new_sym = mmalloc(sizeof(t_sym))) == NULL)
        exit_error("create_new_sym\n");
    ft_memset(new_sym, '\0', sizeof(*new_sym));
    new_sym->raw  = &dt->symtab[index];
    new_sym->name = NULL;
    ft_lst_add_node_back(&dt->syms, ft_lst_create_node(new_sym));
}

int    find_nb_symbols(t_data *dt)
{
    int         syms_nb;
    Elf64_Shdr  symtab_section_h;
    
    syms_nb = 0;
    if (dt->shdr == NULL) //
        exit_corrupted("corruption in shdr (nb1)");
    if (dt->symtab_index > (*dt->ehdr).e_shnum)
        exit_corrupted("shdr index out-of-band");
    symtab_section_h = (Elf64_Shdr)dt->shdr[dt->symtab_index];
    // debug_one_sheader(symtab_section_h);
    if (symtab_section_h.sh_entsize == 0)
        exit_corrupted("zero-division forbidden");
    syms_nb = symtab_section_h.sh_size / symtab_section_h.sh_entsize;
    return (syms_nb);
}

void    debug_to_fix(t_data *dt, int i) // CAN DELETE
{
    if (dt)
    {
        t_lst *sym_node = ft_lst_get_node_at_index(&dt->syms, i);
        if (sym_node)
        {
            t_sym *current_sym = (t_sym *)sym_node->content;
            if (current_sym->type == 'R')
            // if (current_sym->raw->st_shndx == 20)
            {
                printf(C_G_RED"[%s]"C_RES"\n", current_sym->name);
                debug_one_sym(*current_sym->raw);
            }
        }
    }
}

void    read_and_store_syms(t_data *dt)
{
    int syms_nb;
    // t_sym *sym;

    if ((syms_nb = find_nb_symbols(dt)) == 0)
        exit_msg("No symbols");

    for (int i = 0; i < syms_nb; i++)
    {
        create_new_sym(dt, i); // could improve and return new sym instead of using index
        fill_sym(dt, i);       // so that I can use it here and skip double checking
        // t_sym *sym = (t_sym *)ft_lst_get_node_at_index(&dt->syms, i)->content;
        // debug_one_tsym(*sym);

        // debug_to_fix(dt, i);
    }
}

