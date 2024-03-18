#include <nm_functions.h>

static int      find_nb_symbols_64(t_data *dt)
{
    int         syms_nb;
    Elf64_Shdr  symtab_section_h;
    
    syms_nb = 0;
    if (dt->shdr64 == NULL) //
        exit_corrupted("corruption in shdr (nb1)");
    if (dt->symtab_index > (*dt->ehdr64).e_shnum)
        exit_corrupted("shdr index out-of-band");
    symtab_section_h = (Elf64_Shdr)dt->shdr64[dt->symtab_index];
    // debug_one_sheader(symtab_section_h);
    if (symtab_section_h.sh_entsize == 0)
        exit_corrupted("zero-division forbidden");
    syms_nb = symtab_section_h.sh_size / symtab_section_h.sh_entsize;
    return (syms_nb);
}

static int      find_nb_symbols_32(t_data *dt)
{
    int         syms_nb;
    Elf32_Shdr  symtab_section_h;
    
    syms_nb = 0;
    if (dt->shdr32 == NULL) //
        exit_corrupted("corruption in shdr (nb1)");
    if (dt->symtab_index > (*dt->ehdr32).e_shnum)
        exit_corrupted("shdr index out-of-band");
    symtab_section_h = (Elf32_Shdr)dt->shdr32[dt->symtab_index];
    if (symtab_section_h.sh_entsize == 0)
        exit_corrupted("zero-division forbidden");
    syms_nb = symtab_section_h.sh_size / symtab_section_h.sh_entsize;
    return (syms_nb);
}

static void     create_new_sym(t_data *dt, int index)
{
    t_sym *new_sym = NULL;

    if ((new_sym = mmalloc(sizeof(t_sym))) == NULL)
        exit_error("create_new_sym\n");
    ft_memset(new_sym, '\0', sizeof(*new_sym));
    if (dt->arch == ELF_TYPE_32)
        new_sym->raw32  = &dt->symtab32[index];
    else if (dt->arch == ELF_TYPE_64)
        new_sym->raw64  = &dt->symtab64[index];
    new_sym->name = NULL;
    ft_lst_add_node_back(&dt->syms, ft_lst_create_node(new_sym));
}

void            read_and_store_syms(t_data *dt)
{
    int syms_nb;

    if (dt->arch == ELF_TYPE_32)
        syms_nb = find_nb_symbols_32(dt);
    else if (dt->arch == ELF_TYPE_64)
        syms_nb = find_nb_symbols_64(dt);
    if (syms_nb == 0)
        exit_msg("No symbols");

    for (int i = 0; i < syms_nb; i++)
    {
        create_new_sym(dt, i);
        fill_sym(dt, i);
    }
}

