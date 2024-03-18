#include <nm_functions.h>

void    check_sheader_format64(Elf64_Shdr sh)
{
    if (sh.sh_type > 5 || sh.sh_addralign % 8 != 0)
        exit_corrupted("corruption in sheaders");
}

void    check_sheader_format32(Elf32_Shdr sh)
{
    if (sh.sh_type > 5 || sh.sh_addralign % 4 != 0)
        exit_corrupted("corruption in sheaders");
}

void     check_offset_boundaries(t_data *dt, uint64_t offset)
{
    int ehdr_offset;

    ehdr_offset = 0;
    if (dt->arch == ELF_TYPE_32)
        ehdr_offset = sizeof(dt->ehdr32);
    else if (dt->arch == ELF_TYPE_64)
        ehdr_offset = sizeof(dt->ehdr64);
    if (offset > ((uint64_t)dt->fstat_size - ehdr_offset))
        exit_corrupted("file has a section extending past end of file");
}
