#include <nm_functions.h>

void    check_sheader_format(Elf64_Shdr sh)
{
    if (sh.sh_type > 5 || sh.sh_addralign % 8 != 0) // could check sh.sh_size
        exit_corrupted("corruption in sheaders");
}

void     check_offset_boundaries(t_data *dt, uint64_t offset)
{
    if (offset > (uint64_t)dt->fstat_size) // TO DO check max size OK
        exit_corrupted("file has a section extending past end of file");
}