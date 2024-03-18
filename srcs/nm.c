#include <nm_functions.h>

static void    option_h()
{
    display_help();
    free_all_malloc();
    exit(0);
}

void    unload_file_from_memory(t_data *dt)
{
    if (munmap(dt->ptr, dt->fstat_size) < 0)
        exit_error("munmap");
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
        exit_corrupted("Unknown file type");
    return (ELF_TYPE_UNKNOWN);
}

void    nm_wrapper(t_data *dt)
{
    int magic;

    magic = *(int *)dt->ptr;
    if (magic == ELF_MAGIC)
    {
        dt->arch = get_file_type(dt);
        if (dt->arch == ELF_TYPE_64)
            init_elf_ptrs_64(dt);
        else if (dt->arch == ELF_TYPE_32)
            init_elf_ptrs_32(dt);
        read_and_store_syms(dt);
        sort_syms(&dt->syms, dt->sort);
        display_syms(dt, dt->syms, dt->filter);
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
    int             opt_sort;
    int             opt_filter;

    files = parse_input(&parsed_cmd, ac, av, &files_nb);
    if (is_activated_option(parsed_cmd.act_options, 'h'))
        option_h();
    opt_sort   = init_options_sort(parsed_cmd.act_options);
    opt_filter = init_options_filter(parsed_cmd.act_options);
    for (int i = 0; i < files_nb; i++)
    {
        if (files_nb > 1)
            printf("%s:\n", files[i]);
        nm(files[i], opt_sort, opt_filter);
        if (files_nb > 1)
            printf("\n");
    }
    free_all_malloc();
    return (0);
}
