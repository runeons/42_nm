#include <nm_functions.h>

static char    *get_section_name_64(t_data *dt, t_sym *sym)
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

static char    *fill_name_64(t_data *dt, t_sym *sym, int index)
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

static char    *get_section_name_32(t_data *dt, t_sym *sym)
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

static char    *fill_name_32(t_data *dt, t_sym *sym, int index)
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

void            fill_sym(t_data *dt, int index)
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
        sym->name           = fill_name_64(dt, sym, index);
        sym->section_name   = get_section_name_64(dt, sym);

    }
    else if (dt->arch == ELF_TYPE_32)
    {
        info = (unsigned char)dt->symtab32[index].st_info;
        sym->value          = dt->symtab32[index].st_value;
        sym->raw_type       = ELF32_ST_TYPE((int)info);
        sym->raw_bind       = ELF32_ST_BIND((int)info);
        sym->name           = fill_name_32(dt, sym, index);
        sym->section_name   = get_section_name_32(dt, sym);

    }
    sym->type               = compute_type(dt, sym);
}