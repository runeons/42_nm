#include <nm_functions.h>

static Elf64_Sym    *find_symtab_64(t_data *dt)
{
    Elf64_Sym   *symtab64;
    
    if (dt->ehdr64)
        for (int i = 0; i < dt->ehdr64->e_shnum; i++)
        {
            if (dt->shdr64[i].sh_type == SHT_SYMTAB)
            {
                check_sheader_format64(dt->shdr64[i]);
                check_offset_boundaries(dt, dt->shdr64[i].sh_offset);
                if ((symtab64 = (Elf64_Sym *)((char *)dt->ptr + dt->shdr64[i].sh_offset)) == NULL)
                    exit_error("find symtab");
                dt->symtab_index = i;
                // debug_one_sheader(dt->shdr64[i]);
                return (symtab64);
            }
        }
    return (NULL);
}

void                init_elf_ptrs_64(t_data *dt)
{
    if ((dt->ehdr64 = (Elf64_Ehdr *)dt->ptr) == NULL)
        exit_error("eheader");
    // debug_eheader(*dt->ehdr64);
    check_offset_boundaries(dt, dt->ehdr64->e_shoff);
    if ((dt->shdr64 = (Elf64_Shdr *)((char *)dt->ptr + dt->ehdr64->e_shoff)) == NULL)
        exit_error("sheader");
    // debug_one_sheader(*dt->shdr64);
    if ((dt->sh_strtab64 = (Elf64_Shdr *)(&dt->shdr64[dt->ehdr64->e_shstrndx])) == NULL)
        exit_error("sh_strtab");
    // debug_one_sheader(*dt->sh_strtab64);
    if ((dt->sh_strtab_p = (char *)(dt->ptr + dt->sh_strtab64->sh_offset)) == NULL)
        exit_error("sh_strtab_p");
    if ((dt->symtab64 = find_symtab_64(dt)) == NULL)
        exit_msg("no symbols");
}

static Elf32_Sym    *find_symtab_32(t_data *dt)
{
    Elf32_Sym   *symtab32;
    
    if (dt->ehdr32)
        for (int i = 0; i < dt->ehdr32->e_shnum; i++)
        {
            if (dt->shdr32[i].sh_type == SHT_SYMTAB)
            {
                check_sheader_format32(dt->shdr32[i]);
                check_offset_boundaries(dt, dt->shdr32[i].sh_offset);
                if ((symtab32 = (Elf32_Sym *)((char *)dt->ptr + dt->shdr32[i].sh_offset)) == NULL)
                    exit_error("find symtab");
                dt->symtab_index = i;
                return (symtab32);
            }
        }
    return (NULL);
}

void                init_elf_ptrs_32(t_data *dt)
{
    if ((dt->ehdr32 = (Elf32_Ehdr *)dt->ptr) == NULL)
        exit_error("eheader");
    check_offset_boundaries(dt, dt->ehdr32->e_shoff);
    if ((dt->shdr32 = (Elf32_Shdr *)((char *)dt->ptr + dt->ehdr32->e_shoff)) == NULL)
        exit_error("sheader");
    if ((dt->sh_strtab32 = (Elf32_Shdr *)(&dt->shdr32[dt->ehdr32->e_shstrndx])) == NULL)
        exit_error("sh_strtab");
    if ((dt->sh_strtab_p = (char *)(dt->ptr + dt->sh_strtab32->sh_offset)) == NULL)
        exit_error("sh_strtab_p");
    if ((dt->symtab32 = find_symtab_32(dt)) == NULL)
        exit_msg("no symbols");
}
