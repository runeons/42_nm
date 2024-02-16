#include <nm_functions.h>

char capitalise(char letter, unsigned char bind)
{
    if (bind == STB_LOCAL)
        return (letter);
    else if (bind == STB_GLOBAL)
        return (letter - 32);
    return '?';
}

char calc_letter(const Elf64_Sym sym, unsigned char type, unsigned char bind)
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

    if (sym.st_shndx == 26)
        return capitalise('b', bind);
    if (sym.st_shndx == 25)
        return capitalise('d', bind);
    if (sym.st_shndx == 19)
        // return capitalise('r', bind);
    // if (ft_strcmp(sym.name, "data_start") == 0) // TO DO
    //     return 'W';
    // if (ft_strcmp(sym.name, "__data_start") == 0) // TO DO
    //     return 'D';

    if (type == STT_OBJECT)
        return 'Z';
    if (type == STT_COMMON)
        return 'C';
    if (type == STT_SECTION)
        return 'Y';

    return '=';
}

void init_sym(t_data *dt, int index)
{
    t_sym *new_sym = NULL;

    if ((new_sym = mmalloc(sizeof(t_sym))) == NULL)
        exit_error("init_sym\n");
    new_sym->raw   = &dt->symtab[index];
    new_sym->value      = 0;
    new_sym->letter     = '\0';
    new_sym->name       = NULL;
    new_sym->type       = 0;
    new_sym->bind       = 0;
    ft_lst_add_node_back(&dt->syms, ft_lst_create_node(new_sym));
}

void fill_sym(t_data *dt, int index)
{
    Elf64_Sym       *symtab = dt->symtab;
    Elf64_Shdr      symtab_section_h = (Elf64_Shdr)dt->sections[dt->symtab_index_in_sh];
    t_lst           *sym_node = ft_lst_get_node_at_index(&dt->syms, index);
    unsigned char   info;
    char            *strtab;

    if (sym_node != NULL)
    {
        t_sym *sym = (t_sym *)sym_node->content;

        strtab = (char *)(dt->ptr + dt->sections[symtab_section_h.sh_link].sh_offset);
        info = (unsigned char)symtab[index].st_info;
        sym->value = symtab[index].st_value;
        sym->type = ELF64_ST_TYPE((int)info);
        sym->bind = ELF64_ST_BIND((int)info);
        sym->letter = calc_letter(symtab[index], sym->type, sym->bind);
        if ((sym->name = mmalloc(ft_strlen(strtab + symtab[index].st_name) + 1)) == NULL)
            exit_error("fill_sym\n");
        ft_strcpy(sym->name, strtab + symtab[index].st_name); // TO DO protect
    }
}
