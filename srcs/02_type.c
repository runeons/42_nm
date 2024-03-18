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
    {".jcr",                     'd'},
    {".go.buildinfo",            'd'},
    {".noptrbss",                'b'},
    {".noptrdata",               'd'},
};

t_type section_starts_types[] =
{
    {".data.rel.ro",             'd'},
};

char    capitalise(char type, unsigned char bind)
{
    if (bind == STB_LOCAL)
        return (type);
    else if (bind == STB_GLOBAL)
        return (type - 32);
    return '?';
}

char    compute_type(t_data *dt, t_sym *sym)
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
        for (size_t i = 0; i < ARRAY_SIZE(section_starts_types); i++)
        {
            if (!ft_strncmp(sym->section_name, section_starts_types[i].section_name, ft_strlen(section_starts_types[i].section_name)))
                return capitalise(section_starts_types[i].type, sym->raw_bind);
        }
    }
    if (dt->arch == ELF_TYPE_64)
        if (dt->ehdr64->e_type == ET_REL)
            return 'U';
    if (dt->arch == ELF_TYPE_32)
        if (dt->ehdr32->e_type == ET_REL)
            return 'U';
    // if (!ft_strcmp(sym->name, "esection_types"))
    // {
    //     printf(C_G_BLUE"[QUICK DEBUG] sym->name: %s"C_RES"\n", sym->name);
    //     printf(C_G_BLUE"              sym->section_name: %s"C_RES"\n", sym->section_name);
    //     printf(C_G_BLUE"              sym->raw_type: %d"C_RES"\n", sym->raw_type);
    //     printf(C_G_BLUE"              sym->raw_bind: %d"C_RES"\n", sym->raw_bind);
    //     printf(C_G_RED"               sym->raw64->st_shndx: %d"C_RES"\n", sym->raw64->st_shndx);
    // }
    return '?';
}
