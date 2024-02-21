#include <nm_functions.h>

t_type all_types[] =
{
    {'b', 1, ".bss",             },//  -1, -1// bss uninitializes
    {'d', 1, ".data",            },//  -1, -1// initialized
    {'g', 1, "",                 },//  -1, -1// initialized for small objects
    {'p', 0, "",                 },//  -1, -1// stack unwind
    {'r', 1, ".rodata",          },//  -1, -1// read only 
    {'s', 1, "",                 },//  -1, -1// uninitialized or zero-initialized data section for small objects
    {'t', 1, ".text",            },//  -1, -1// executable text (code) section
    {'a', -1, "",                 },//  -1, -1// ? capitalise type - STT_FILE
    {'A', -1, "",                 },//  -1, -1// ? capitalise absolute
    {'C', 0, "",                 },//  -1, -1// common (inc. uninitialized)
    {'i', 0, "",                 },//  -1, -1// indirect functions
    {'N', 0, ".debug",           },//  -1, -1// debug
    {'u', 0, "",                 },//  -1, -1// unique global
    {'U', 0, "",                 },//  -1, -1// undefined
    {'v', 1, "",                 },//  -1, -1// weak object
    {'w', 1, "",                 },//  -1, -1// weak symbol not weak object
    {'-', 0, "",                 },//  -1, -1// stabs symbol
    {'?', 0, "",                 },//  -1, -1// unknown
};


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

char    fill_type(const Elf64_Sym raw_sym, t_sym *sym)
{
    // printf(C_G_BLUE"[QUICK DEBUG] sym->section_name: %s"C_RES"\n", sym->section_name);
    if (raw_sym.st_value == 0)
        if (sym->bind == STB_WEAK) 
            return 'w';
        else if (sym->type == STT_FILE)
            return 'a';
        else
            return 'U';
    else if (sym->bind == STB_WEAK)
        return 'W';
    else if (sym->type == STT_FUNC)
        return capitalise('t', sym->bind);
    // printf(C_G_RED"[QUICK DEBUG] sym->section_name: %s"C_RES"\n", sym->section_name);
    if (sym->section_name)
    {
        if (!ft_strcmp(sym->section_name, ".rodata"))
            return capitalise('r', sym->bind);
        if (!ft_strcmp(sym->section_name, ".data"))
            return capitalise('d', sym->bind);
        if (!ft_strcmp(sym->section_name, ".bss"))
            return capitalise('b', sym->bind);
        if (!ft_strcmp(sym->section_name, ".fini_array"))
            return capitalise('d', sym->bind);
        if (!ft_strcmp(sym->section_name, ".init_array"))
            return capitalise('d', sym->bind);
        if (!ft_strcmp(sym->section_name, ".eh_frame"))
            return capitalise('r', sym->bind);
        if (!ft_strcmp(sym->section_name, ".dynamic"))
            return capitalise('d', sym->bind);
        if (!ft_strcmp(sym->section_name, ".eh_frame_hdr"))
            return capitalise('r', sym->bind);
        if (!ft_strcmp(sym->section_name, ".data.rel.ro"))
            return capitalise('d', sym->bind);
        if (!ft_strcmp(sym->section_name, ".got.plt"))
            return capitalise('d', sym->bind);
        if (!ft_strcmp(sym->section_name, ".tbss"))
            return capitalise('b', sym->bind);
        if (!ft_strcmp(sym->section_name, ".got"))
            return capitalise('d', sym->bind);
        if (!ft_strcmp(sym->section_name, ".note.ABI-tag"))
            return capitalise('r', sym->bind);
    }

    printf(C_G_BLUE"[QUICK DEBUG] sym->name: %s"C_RES"\n", sym->name);
    printf(C_G_BLUE"              sym->section_name: %s"C_RES"\n", sym->section_name);
    printf(C_G_BLUE"              sym->type: %d"C_RES"\n", sym->type);
    printf(C_G_BLUE"              sym->bind: %d"C_RES"\n", sym->bind);
    printf(C_G_RED"               sym->raw->st_shndx: %d"C_RES"\n", sym->raw->st_shndx);


    // if (sym->type == STT_OBJECT)
    //     return 'Z';
    // if (sym->type == STT_COMMON)
    //     return 'C';
    // if (sym->type == STT_SECTION)
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
    sym->name           = fill_name(dt, sym, index);
    sym->section_name   = get_section_name(dt, sym);
    sym->letter         = fill_type(dt->symtab[index], sym);
    // debug_one_tsym(*sym);
}