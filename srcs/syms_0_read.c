#include <nm_functions.h>

char    capitalise(char letter, unsigned char bind)
{
    if (bind == STB_LOCAL)
        return (letter);
    else if (bind == STB_GLOBAL)
        return (letter - 32);
    return '?';
}

char    calc_letter(const Elf64_Sym sym, unsigned char type, unsigned char bind)
{
    if (sym.st_value == 0)
        if (bind == STB_WEAK) 
            return 'w';
        else if (type == STT_FILE)
            return 'a';
        else
            return 'U';
    else if (bind == STB_WEAK)
        return 'W';
    else if (type == STT_FUNC)
        return capitalise('t', bind);
    if (sym.st_shndx >= 17 && sym.st_shndx <= 20) // could make it prettier
        return capitalise('r', bind);
    if (sym.st_shndx >= 21 && sym.st_shndx <= 25) // 20 may be OS dependent
        return capitalise('d', bind);
    if (sym.st_shndx == 26)
        return capitalise('b', bind);
    if (sym.st_shndx == 4)
        return capitalise('r', bind);
    // if (type == STT_OBJECT)
    //     return 'Z';
    // if (type == STT_COMMON)
    //     return 'C';
    // if (type == STT_SECTION)
    //     return 'Y';
    return '?';
}

char    *fill_name(t_data *dt, t_sym *sym, int index)
{
    Elf64_Shdr      symtab_section_h;
    char            *strtab;

    if (dt->sections_hdrs == NULL || dt->symtab_index > (int)sizeof(*dt->sections_hdrs))
        exit_corrupted("corruption in sections_hdrs");
    symtab_section_h = (Elf64_Shdr)dt->sections_hdrs[dt->symtab_index];;
    if (dt->sections_hdrs == NULL || symtab_section_h.sh_link > (int)sizeof(*dt->sections_hdrs))
        exit_corrupted("corruption in symtab_section_h");
    check_offset_boundaries(dt, dt->sections_hdrs[symtab_section_h.sh_link].sh_offset);
    strtab = (char *)(dt->ptr + dt->sections_hdrs[symtab_section_h.sh_link].sh_offset);
    if ((sym->name  = mmalloc(ft_strlen(strtab + dt->symtab[index].st_name) + 1)) == NULL) // should I check name format and len
        exit_error("fill_name\n");
    ft_strcpy(sym->name, strtab + dt->symtab[index].st_name); // TO DO use ft_strncpy
    return (sym->name);
}

void    fill_sym(t_data *dt, int index)
{
    t_sym           *sym;
    t_lst           *sym_node;
    unsigned char   info;

    sym = NULL;
    sym_node = NULL;
    if ((sym_node = ft_lst_get_node_at_index(&dt->syms, index)) == NULL)
        exit_corrupted("fill_sym");
    if ((sym = (t_sym *)sym_node->content) == NULL)
        exit_corrupted("fill_sym");
    info = (unsigned char)dt->symtab[index].st_info;
    sym->value      = dt->symtab[index].st_value;
    sym->type       = ELF64_ST_TYPE((int)info);
    sym->bind       = ELF64_ST_BIND((int)info);
    sym->letter     = calc_letter(dt->symtab[index], sym->type, sym->bind);
    sym->name       = fill_name(dt, sym, index);
}

void    create_new_sym(t_data *dt, int index)
{
    t_sym *new_sym = NULL;

    if ((new_sym = mmalloc(sizeof(t_sym))) == NULL)
        exit_error("create_new_sym\n");
    memset(new_sym, '\0', sizeof(*new_sym));
    new_sym->raw  = &dt->symtab[index];
    new_sym->name = NULL;
    ft_lst_add_node_back(&dt->syms, ft_lst_create_node(new_sym));
}

int    find_nb_symbols(t_data *dt)
{
    int         syms_nb;
    Elf64_Shdr  symtab_section_h;
    
    syms_nb = 0;
    if (dt->sections_hdrs == NULL || dt->symtab_index > (int)sizeof(*dt->sections_hdrs))
        exit_corrupted("corruption in sections_hdrs");
    symtab_section_h = (Elf64_Shdr)dt->sections_hdrs[dt->symtab_index];
    // debug_one_sheader(symtab_section_h);
    if (symtab_section_h.sh_entsize == 0)
        exit_corrupted("zero-division forbidden");
    syms_nb = symtab_section_h.sh_size / symtab_section_h.sh_entsize;
    return (syms_nb);
}

void    read_and_store_syms(t_data *dt)
{
    int syms_nb;

    if ((syms_nb = find_nb_symbols(dt)) == 0)
        exit_msg("No symbols");
    for (int i = 0; i < syms_nb; i++)
    {
        create_new_sym(dt, i); // could improve and return new sym instead of using index
        fill_sym(dt, i);       // so that I can use it here and skip double checking
        // debug_to_fix(dt, i);
    }
}

