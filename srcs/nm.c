#include <nm_functions.h>

Elf64_Sym    *find_symtab(t_data *dt)
{
    Elf64_Sym   *symtab;
    
    if (dt->ehdr)
        for (int i = 0; i < dt->ehdr->e_shnum; i++)
        {
            if (dt->shdr[i].sh_type == SHT_SYMTAB)
            {
                check_sheader_format(dt->shdr[i]);
                check_offset_boundaries(dt, dt->shdr[i].sh_offset);
                if ((symtab = (Elf64_Sym *)((char *)dt->ptr + dt->shdr[i].sh_offset)) == NULL)
                    exit_error("symtab");
                dt->symtab_index = i;
                // debug_one_sheader(dt->shdr[i]);
                return (symtab);
            }
        }
    return (NULL);
}

void init_elf_ptrs(t_data *dt)
{
    if ((dt->ehdr = (Elf64_Ehdr *)dt->ptr) == NULL)
        exit_error("eheader");
    // debug_eheader(*dt->ehdr);
    check_offset_boundaries(dt, dt->ehdr->e_shoff);
    if ((dt->shdr = (Elf64_Shdr *)((char *)dt->ptr + dt->ehdr->e_shoff)) == NULL)
        exit_error("sheader");
    // debug_one_sheader(*dt->shdr);
    if ((dt->sh_strtab = (Elf64_Shdr *)(&dt->shdr[dt->ehdr->e_shstrndx])) == NULL)
        exit_error("sh_strtab");
    // debug_one_sheader(*dt->sh_strtab);
    if ((dt->sh_strtab_p = (char *)(dt->ptr + dt->sh_strtab->sh_offset)) == NULL)
        exit_error("sh_strtab_p");
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
    ft_memset(&buf, '\0', sizeof(buf));
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

    ft_memset(&dt, '\0', sizeof(dt));
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
