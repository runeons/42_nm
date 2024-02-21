#include <nm_functions.h>


// typedef struct  s_type
// {
//     Elf64_Sym       *raw;
//     char            *section_name;
//     char            letter;
//     char            *name;
//     unsigned char   type;
//     unsigned char   bind;
// }               t_type;

// // letter, capitalise, section_location
// {
//     'b', "1", ".bss", // bss uninitializes
//     'd', "1", ".data", // initialized
//     'g', "1", "", // initialized for small objects
//     'p', "0", "", // stack unwind
//     'r', "1", ".rodata", // read only 
//     's', "1", "", // uninitialized or zero-initialized data section for small objects
//     't', "1", ".text", // executable text (code) section

//     'a', "?", "", // ? type - STT_FILE
//     'A', "?", "", // absolute
//     'C', "0", "", // common (inc. uninitialized)
//     'i', "0", "", // indirect functions
//     'N', "0", ".debug", // debug
//     'u', "0", "", // unique global
//     'U', "0", "", // undefined
//     'v', "1", "", // weak object
//     'w', "1", "", // weak symbol not weak object
//     '-', "0", "", // stabs symbol
//     '?', "0", "", // unknown
// }


// not used PREDEFINED USER sections = 
// .comment    // comment
// .init       // runtime initialization instructions
// .line       // line # info for symbolic debugging
// .note       // note information


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
    if (sym.st_shndx == 14)
        return capitalise('r', bind);
    if (sym.st_shndx >= 17 && sym.st_shndx <= 20) // could make it prettier
        return capitalise('r', bind);
    if (sym.st_shndx >= 21 && sym.st_shndx <= 25) // 20 may be OS dependent // could use a method more robust than numbers
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

char    *get_section_name(t_data *dt, t_sym *sym)
{
    char            *section_name;
    unsigned int    section_name_offset;

    if (sym == NULL || dt->shdr == NULL || dt->sh_strtab  == NULL || dt->sh_strtab_p == NULL)
        return (NULL);
    if (sym->raw->st_shndx > dt->ehdr->e_shnum) // if current index > nb sections
        return (NULL);
    section_name_offset = dt->shdr[sym->raw->st_shndx].sh_name;
    section_name = dt->sh_strtab_p + section_name_offset;
    return (section_name);
}

char    *fill_name(t_data *dt, t_sym *sym, int index)
{
    Elf64_Shdr      symtab_section_h;
    char            *strtab;

    if (dt->shdr == NULL || dt->symtab_index > (int)sizeof(*dt->shdr))
        exit_corrupted("corruption in shdr");
    symtab_section_h = (Elf64_Shdr)dt->shdr[dt->symtab_index];;
    if (dt->shdr == NULL || symtab_section_h.sh_link > (int)sizeof(*dt->shdr))
        exit_corrupted("corruption in symtab_section_h");
    check_offset_boundaries(dt, dt->shdr[symtab_section_h.sh_link].sh_offset);
    strtab = (char *)(dt->ptr + dt->shdr[symtab_section_h.sh_link].sh_offset);
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
    sym->value          = dt->symtab[index].st_value;
    sym->type           = ELF64_ST_TYPE((int)info);
    sym->bind           = ELF64_ST_BIND((int)info);
    sym->letter         = calc_letter(dt->symtab[index], sym->type, sym->bind);
    sym->name           = fill_name(dt, sym, index);
    sym->section_name   = get_section_name(dt, sym);
}
