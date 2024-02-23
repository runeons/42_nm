#include <nm_functions.h>

Elf64_Sym    *find_symtab64(t_data *dt)
{
    Elf64_Sym   *symtab64;
    
    if (dt->ehdr64)
        for (int i = 0; i < dt->ehdr64->e_shnum; i++)
        {
            if (dt->shdr64[i].sh_type == SHT_SYMTAB)
            {
                check_sheader_format(dt->shdr64[i]);
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

Elf32_Sym    *find_symtab32(t_data *dt)
{
    Elf32_Sym   *symtab32;
    
    if (dt->ehdr32)
        for (int i = 0; i < dt->ehdr32->e_shnum; i++)
        {
            if (dt->shdr32[i].sh_type == SHT_SYMTAB)
            {
                // check_sheader_format32(dt->shdr32[i]); // TO DO ADD PROTECTION
                check_offset_boundaries(dt, dt->shdr32[i].sh_offset);
                if ((symtab32 = (Elf32_Sym *)((char *)dt->ptr + dt->shdr32[i].sh_offset)) == NULL)
                    exit_error("find symtab");
                dt->symtab_index = i;
                // debug_one_sheader(dt->shdr32[i]);
                return (symtab32);
            }
        }
    return (NULL);
}

void init_elf_ptrs64(t_data *dt)
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
    if ((dt->symtab64 = find_symtab64(dt)) == NULL)
        exit_msg("no symbols");
}

void init_elf_ptrs32(t_data *dt)
{
    if ((dt->ehdr32 = (Elf32_Ehdr *)dt->ptr) == NULL)
        exit_error("eheader");
    // debug_eheader(*dt->ehdr32);
    check_offset_boundaries(dt, dt->ehdr32->e_shoff);
    if ((dt->shdr32 = (Elf32_Shdr *)((char *)dt->ptr + dt->ehdr32->e_shoff)) == NULL)
        exit_error("sheader");
    // debug_one_sheader(*dt->shdr32);
    if ((dt->sh_strtab32 = (Elf32_Shdr *)(&dt->shdr32[dt->ehdr32->e_shstrndx])) == NULL)
        exit_error("sh_strtab");
    // debug_one_sheader(*dt->sh_strtab32);
    if ((dt->sh_strtab_p = (char *)(dt->ptr + dt->sh_strtab32->sh_offset)) == NULL)
        exit_error("sh_strtab_p");
    if ((dt->symtab32 = find_symtab32(dt)) == NULL)
        exit_msg("no symbols");
}

int    get_file_type(t_data *dt)
{
    Elf64_Ehdr *header;
    
    if ((header = (Elf64_Ehdr *)dt->ptr) == NULL)
        exit_error("init header");
    if (header->e_ident[EI_CLASS] == ELFCLASS32)
        return (ELF_TYPE_32);
    else if (header->e_ident[EI_CLASS] == ELFCLASS64)
        return (ELF_TYPE_64);
    else
    {
        exit_corrupted("Unknown file type");
        return (ELF_TYPE_UNKNOWN);
    }
}

void    nm_wrapper(t_data *dt) // 86_32, x64, object files, .so
{
    int magic;

    magic = *(int *)dt->ptr;
    if (magic == ELF_MAGIC)
    {
        dt->arch = get_file_type(dt);
        if (dt->arch == ELF_TYPE_64)
        {
            init_elf_ptrs64(dt);
            read_and_store_syms(dt);
            sort_syms(&dt->syms, dt->sort);
            display_syms(dt, dt->syms, dt->filter);
        }
        else if (dt->arch == ELF_TYPE_32)
        {
            init_elf_ptrs32(dt);
            read_and_store_syms(dt);
            sort_syms(&dt->syms, dt->sort);
            display_syms(dt, dt->syms, dt->filter);
        }
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

void    unload_file_from_memory(t_data *dt)
{
    if (munmap(dt->ptr, dt->fstat_size) < 0)
        exit_error("munmap");
}

static void    option_h()
{
    display_help();
    free_all_malloc();
    exit(0);
}

static char **parse_input(t_parsed_cmd *parsed_cmd, int ac, char **av, int *len)
{
    int     files_nb;
    char    **files;
    t_lst   *current;
    int     i;

    *parsed_cmd = parse_options(ac, av);
    files_nb = ft_lst_size(parsed_cmd->not_options);
    if (files_nb == 0)
    {
        if ((files = (char **)mmalloc(1 * sizeof(char *))) == NULL)
            exit_error("Malloc failure");
        files[0] = ft_strdup("a.out");
        ft_lst_add_node_back(&parsed_cmd->not_options, ft_lst_create_node(files[0]));
        *len = 1;
        return (files);
    }
    if ((files = (char **)mmalloc(files_nb * sizeof(char *))) == NULL)
        exit_error("Malloc failure");
    current = parsed_cmd->not_options;
    i = 0;
    while (current)
    {
        files[i] = (char *)current->content;
        current = current->next;
        i++;
        *len = i;
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
    unload_file_from_memory(&dt);
}

int     main(int ac, char **av)
{
    t_parsed_cmd    parsed_cmd;
    char            **files;
    int             files_nb;
    int             sort;
    int             filter;

    files = parse_input(&parsed_cmd, ac, av, &files_nb);
    if (is_activated_option(parsed_cmd.act_options, 'h'))
        option_h();
    sort = init_options_sort(parsed_cmd.act_options);
    filter = init_options_filter(parsed_cmd.act_options);
    for (int i = 0; i < files_nb; i++)
    {
        if (files_nb > 1)
            printf("%s:\n", files[i]);
        nm(files[i], sort, filter);
        if (files_nb > 1)
            printf("\n");
    }
    free_all_malloc();
    return (0);
}
