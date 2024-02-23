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
                    exit_error("find symtab");
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
        exit_msg("no symbols");
}

int    get_file_type(t_data *dt)
{
    Elf64_Ehdr *header;
    
    if ((header = (Elf64_Ehdr *)dt->ptr) == NULL)
        exit_error("init header");
    if (header->e_ident[EI_CLASS] == ELFCLASS32)
    {
        printf("x32 file\n");
        return (ELF_TYPE_32);
    }
    else if (header->e_ident[EI_CLASS] == ELFCLASS64)
    {
        printf("x64 file\n");
        return (ELF_TYPE_64);
    }
    else
    {
        exit_corrupted("Unknown file type");
        return (ELF_TYPE_UNKNOWN);
    }
}

void    nm_wrapper(t_data *dt) // 86_32, x64, object files, .so
{
    int magic;
    // int elf_type;

    magic = *(int *)dt->ptr;
    if (magic == ELF_MAGIC)
    {
        // elf_type = get_file_type(dt);
        // printf("elf_type: %d\n", elf_type);
        init_elf_ptrs(dt);
        read_and_store_syms(dt);
        // printf("dt->sort: %d\n", dt->sort);
        sort_syms(&dt->syms, dt->sort);
        // printf("dt->sort: %d\n", dt->sort);
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
        exit_error(filename);
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

static void    option_h()
{
    display_help();
    free_all_malloc();
    exit(0);
}

static char **parse_input(t_parsed_cmd *parsed_cmd, int ac, char **av)
{
    int     files_nb;
    char    **files;
    t_lst   *current;

    if (ac < 2)
        option_h();
    *parsed_cmd = parse_options(ac, av);
    // debug_activated_options(parsed_cmd->act_options);
    files_nb = ft_lst_size(parsed_cmd->not_options);
    if (files_nb == 0)
    {
        if ((files = (char **)mmalloc(1 * sizeof(char *))) == NULL)
            exit_error("Malloc failure");
        files[0] = ft_strdup("a.out");
        return (files);
    }
    if ((files = (char **)mmalloc(files_nb * sizeof(char *))) == NULL)
        exit_error("Malloc failure");
    current = parsed_cmd->not_options;
    for (int i = 0; i < files_nb; i++)
    {
        files[i] = (char *)current->content;
        current = current->next;
    }
    return (files);
}

void    nm(char *filename, int sort, int filter)
{
    t_data          dt;

    ft_memset(&dt, '\0', sizeof(dt));
    dt.sort = sort;
    dt.filter = filter;
    load_file_in_memory(&dt, filename);
    nm_wrapper(&dt);
    unload_file_and_clear(&dt);
}

int     main(int ac, char **av)
{
    t_parsed_cmd    parsed_cmd;
    char            **files;
    int             files_nb;
    int             sort;
    int             filter;

    files = parse_input(&parsed_cmd, ac, av);
    files_nb = ft_tablen(files);
    if (is_activated_option(parsed_cmd.act_options, 'h'))
        option_h();
    sort = init_options_sort(parsed_cmd.act_options);
    filter = init_options_filter(parsed_cmd.act_options);
    for (int i = 0; i < ft_tablen(files); i++)
    {
        if (files_nb > 1)
            printf("%s:\n", files[i]);
        nm(files[i], sort, filter);
        if (files_nb > 1)
            printf("\n");

    }
    return (0);
}
