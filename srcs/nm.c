#include <nm_functions.h>

Elf64_Sym    *find_symtab(t_data *dt)
{
    Elf64_Sym   *symtab;
    
    if (dt->header)
        for (int i = 0; i < dt->header->e_shnum; i++)
        {
            if (dt->sections_hdrs[i].sh_type == SHT_SYMTAB)
            {
                check_sheader_format(dt->sections_hdrs[i]);
                check_offset_boundaries(dt, dt->sections_hdrs[i].sh_offset);
                if ((symtab = (Elf64_Sym *)((char *)dt->ptr + dt->sections_hdrs[i].sh_offset)) == NULL)
                    exit_error("symtab");
                dt->symtab_index = i;
                // debug_one_sheader(dt->sections_hdrs[i]);
                return (symtab);
            }
        }
    return (NULL);
}

void init_elf_ptrs(t_data *dt)
{
    if ((dt->header = (Elf64_Ehdr *)dt->ptr) == NULL)
        exit_error("eheader");
    // debug_eheader(*dt->header);
    check_offset_boundaries(dt, dt->header->e_shoff);
    if ((dt->sections_hdrs = (Elf64_Shdr *)((char *)dt->ptr + dt->header->e_shoff)) == NULL)
        exit_error("sheader");
    // debug_one_sheader(*dt->sections_hdrs);
    if ((dt->symtab = find_symtab(dt)) == NULL)
        exit_error("symtab");
}

void    nm_wrapper(t_data *dt) // 86_32, x64, object files, .so
{
    int magic;

    magic = *(int *)dt->ptr;
    if (magic == ELF_MAGIC)
    {
        init_elf_ptrs(dt);
        read_and_store_syms(dt);
        sort_syms(&dt->syms);
        display_syms(dt->syms);
    }
    else
        exit_msg("file format not recognized");
}

void    load_file_in_memory(t_data *dt, char *filename)
{
    int         fd;
    struct stat buf;

    fd = 0;
    memset(&buf, '\0', sizeof(buf));
    if ((fd = open(filename, O_RDONLY)) < 0)
        exit_error("open");
    if (fstat(fd, &buf) < 0)
        exit_error_close("fstat", fd);
    if ((dt->ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
        exit_error_close("mmap", fd);
    dt->fstat_size = buf.st_size;
    if (close(fd) < 0)
        exit_error("close");
}

void    unload_file_and_clear(t_data *dt)
{
    if (munmap(dt->ptr, dt->fstat_size) < 0)
        exit_error("munmap");
    free_all_malloc();
}

void    nm(char *filename)
{
    t_data      dt;

    memset(&dt, '\0', sizeof(dt));
    load_file_in_memory(&dt, filename);
    nm_wrapper(&dt);
    unload_file_and_clear(&dt);
}

int     main(int ac, char **av)
{
    if (ac == 1)
        nm("a.out");
    else if (ac >= 10)
        exit_error("too many files");
    else
        for (int i = 1; i < ac; i++)
            nm(av[i]);
    return (0);
}
