#include <nm_functions.h>

t_type section_types[] =
{
    {".rodata",                  'r'},
    {".data",                    'd'},
    {".bss",                     'b'},
    {".fini_array",              'd'},
    {".init_array",              'd'},
    {".eh_frame",                'r'},
    {".dynamic",                 'd'},
    {".eh_frame_hdr",            'r'},
    {".data.rel.ro",             'd'},
    {".got.plt",                 'd'},
    {".tbss",                    'b'},
    {".got",                     'd'},
    {".note.ABI-tag",            'r'},
    {".group",                   'n'},
    {".preinit_array",           'd'},
    {".data.rel.local",          'd'},
};

char    capitalise(char type, unsigned char bind)
{
    if (bind == STB_LOCAL)
        return (type);
    else if (bind == STB_GLOBAL)
        return (type - 32);
    return '?';
}

char    fill_type(t_data *dt, t_sym *sym)
{
    if (dt->arch == ELF_TYPE_64)
    {
        if (sym->raw64->st_value == 0)
        {
            if (sym->raw_bind == STB_WEAK) 
                return 'w';
            else if (sym->raw_type == STT_FILE)
                return 'a';
            else if (dt->ehdr64->e_type != ET_REL) // ET_REL == .o
                return 'U';
        }
    }
    else if (dt->arch == ELF_TYPE_32)
    {
        if (sym->raw32->st_value == 0)
        {
            if (sym->raw_bind == STB_WEAK) 
                return 'w';
            else if (sym->raw_type == STT_FILE)
                return 'a';
            else if (dt->ehdr32->e_type != ET_REL) // ET_REL == .o
                return 'U';
        }
    }
    if (sym->raw_bind == STB_WEAK)
        return 'W';
    else if (sym->raw_type == STT_FUNC)
        return capitalise('t', sym->raw_bind);
    if (sym->section_name)
    {
        for (size_t i = 0; i < ARRAY_SIZE(section_types); i++)
        {
            if (!ft_strcmp(sym->section_name, section_types[i].section_name))
                return capitalise(section_types[i].type, sym->raw_bind);
        }
    }
    if (dt->ehdr64->e_type == ET_REL)
        return 'U';
    // if (!ft_strcmp(sym->name, "section_types"))
    // {
    //     printf(C_G_BLUE"[QUICK DEBUG] sym->name: %s"C_RES"\n", sym->name);
    //     printf(C_G_BLUE"              sym->section_name: %s"C_RES"\n", sym->section_name);
    //     printf(C_G_BLUE"              sym->raw_type: %d"C_RES"\n", sym->raw_type);
    //     printf(C_G_BLUE"              sym->raw_bind: %d"C_RES"\n", sym->raw_bind);
    //     printf(C_G_RED"               sym->raw64->st_shndx: %d"C_RES"\n", sym->raw64->st_shndx);
    // }
    return '?';
}


char    *get_section_name32(t_data *dt, t_sym *sym)
{
    char            *section_name;
    unsigned int    section_name_offset;

    if (sym == NULL || dt->shdr32 == NULL || dt->sh_strtab32  == NULL || dt->sh_strtab_p == NULL)
        return (NULL);
    if (sym->raw32->st_shndx > dt->ehdr32->e_shnum) // if current index > nb sections
        return (NULL);
    section_name_offset = dt->shdr32[sym->raw32->st_shndx].sh_name;
    section_name = dt->sh_strtab_p + section_name_offset;
    return (section_name);
}

char    *get_section_name64(t_data *dt, t_sym *sym)
{
    char            *section_name;
    unsigned int    section_name_offset;

    if (sym == NULL || dt->shdr64 == NULL || dt->sh_strtab64  == NULL || dt->sh_strtab_p == NULL)
        return (NULL);
    if (sym->raw64->st_shndx > dt->ehdr64->e_shnum) // if current index > nb sections
        return (NULL);
    section_name_offset = dt->shdr64[sym->raw64->st_shndx].sh_name;
    section_name = dt->sh_strtab_p + section_name_offset;
    return (section_name);
}

char    *fill_name64(t_data *dt, t_sym *sym, int index)
{
    Elf64_Shdr      symtab_section_h;
    char            *strtab;

    if (dt->symtab_index > (*dt->ehdr64).e_shnum)
        exit_corrupted("shdr index out-of-band");
    symtab_section_h = (Elf64_Shdr)dt->shdr64[dt->symtab_index];;
    if (symtab_section_h.sh_link > (*dt->ehdr64).e_shnum)
        exit_corrupted("shdr index out-of-band");
    check_offset_boundaries(dt, dt->shdr64[symtab_section_h.sh_link].sh_offset);
    strtab = (char *)(dt->ptr + dt->shdr64[symtab_section_h.sh_link].sh_offset);
    if ((sym->name  = mmalloc(ft_strlen(strtab + dt->symtab64[index].st_name) + 1)) == NULL)
        exit_error("fill_name\n");
    ft_strcpy(sym->name, strtab + dt->symtab64[index].st_name);
    return (sym->name);
}

char    *fill_name32(t_data *dt, t_sym *sym, int index)
{
    Elf32_Shdr      symtab_section_h;
    char            *strtab;

    if (dt->symtab_index > (*dt->ehdr32).e_shnum)
        exit_corrupted("shdr index out-of-band");
    symtab_section_h = (Elf32_Shdr)dt->shdr32[dt->symtab_index];;
    if (symtab_section_h.sh_link > (*dt->ehdr32).e_shnum)
        exit_corrupted("shdr index out-of-band");
    check_offset_boundaries(dt, dt->shdr32[symtab_section_h.sh_link].sh_offset);
    strtab = (char *)(dt->ptr + dt->shdr32[symtab_section_h.sh_link].sh_offset);
    if ((sym->name  = mmalloc(ft_strlen(strtab + dt->symtab32[index].st_name) + 1)) == NULL)
        exit_error("fill_name\n");
    ft_strcpy(sym->name, strtab + dt->symtab32[index].st_name);
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
    if (dt->arch == ELF_TYPE_64)
    {
        info = (unsigned char)dt->symtab64[index].st_info;
        sym->value          = dt->symtab64[index].st_value;
        sym->raw_type       = ELF64_ST_TYPE((int)info);
        sym->raw_bind       = ELF64_ST_BIND((int)info);
        sym->name           = fill_name64(dt, sym, index);
        sym->section_name   = get_section_name64(dt, sym);

    }
    else if (dt->arch == ELF_TYPE_32)
    {
        info = (unsigned char)dt->symtab32[index].st_info;
        sym->value          = dt->symtab32[index].st_value;
        sym->raw_type       = ELF32_ST_TYPE((int)info);
        sym->raw_bind       = ELF32_ST_BIND((int)info);
        sym->name           = fill_name32(dt, sym, index);
        sym->section_name   = get_section_name32(dt, sym);

    }
    sym->type           = fill_type(dt, sym);
}