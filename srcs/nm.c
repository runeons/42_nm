#include <nm_functions.h>

void exit_error(const char *msg)
{
    printf(C_G_RED"Error: "C_RES"nm: %s: %s\n", msg, strerror(errno));
    free_all_malloc();
    exit(1);
}

void exit_corrupted(const char *msg)
{
    printf(C_G_BLUE"Error: "C_RES"nm: %s: %s\n", msg, strerror(errno));
    free_all_malloc();
    exit(1);
}

void exit_msg(const char *msg)
{
    printf(C_G_RED"Error: "C_RES"nm: %s\n", msg);
    free_all_malloc();
    exit(1);
}

void exit_error_close(const char *msg, int fd)
{
    printf(C_G_RED"Error: "C_RES"nm: %s: %s\n", msg, strerror(errno));
    free_all_malloc();
    close(fd);
    exit(1);
}

// void    store_symbols(t_data *dt)
// {

// }

int    find_nb_symbols(t_data *dt)
{
    int         syms_nb;
    Elf64_Shdr  symtab_section_h;
    
    syms_nb = 0;
    symtab_section_h = (Elf64_Shdr)dt->sections_hdrs[dt->symtab_index];
    // debug_one_sheader(symtab_section_h);
    syms_nb = symtab_section_h.sh_size / symtab_section_h.sh_entsize;

    return (syms_nb);
}


void read_and_store_syms(t_data *dt)
{
    int i;
    int syms_nb;

    syms_nb = find_nb_symbols(dt);

    // printf(C_G_RED"[QUICK DEBUG] syms_nb: %d"C_RES"\n", syms_nb);
    for (i = 0; i < syms_nb; i++)
    {
        init_sym(dt, i);
        fill_sym(dt, i);
        // t_lst *sym_node = ft_lst_get_node_at_index(&dt->syms, i);
        // t_sym *current_sym = (t_sym *)sym_node->content;
        // // if (current_sym->letter == 'r')
        // if (current_sym->raw->st_shndx == 20)
        // {
        //     printf(C_G_RED"[%s]"C_RES"\n", current_sym->name);
        //     debug_one_sym(*current_sym->raw);
        // }
    }
    sort_list(&dt->syms);
    display_sym_list(dt->syms);
}

void     check_offset_boundaries(t_data *dt, uint64_t offset)
{
    if (offset > (uint64_t)dt->fstat_size) // TO DO check max size OK
        exit_corrupted("file has a section extending past end of file");
}

Elf64_Sym    *find_symtab(t_data *dt)
{
    Elf64_Sym   *symtab;
    
    if (dt->header)
        for (int i = 0; i < dt->header->e_shnum; i++)
        {
            if (dt->sections_hdrs[i].sh_type == SHT_SYMTAB)
            {
                check_offset_boundaries(dt, dt->sections_hdrs[i].sh_offset);
                // printf(C_G_RED"[QUICK DEBUG] dt->sections_hdrs[i]: %ld"C_RES"\n", dt->sections_hdrs[i]);
                // printf(C_G_RED"[QUICK DEBUG] dt->sections_hdrs[i].sh_offset: %ld"C_RES"\n", dt->sections_hdrs[i].sh_offset);
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
