#include <nm_functions.h>

void exit_error(const char *msg)
{
    printf(C_G_RED"Error:"C_G_WHITE" %s\n"C_RES, msg);
    free_all_malloc();
    exit(1);
}

Elf64_Sym    *get_symtab(t_data *dt)
{
    int         i;
    Elf64_Sym   *symtab;
    
    for (i = 0; i < dt->header->e_shnum; i++)
    {
        if (dt->sections[i].sh_type == SHT_SYMTAB)
        {
            symtab = (Elf64_Sym *)((char *)dt->ptr + dt->sections[i].sh_offset); // debut de la symtab
            dt->symtab_index_in_sh = i;
            // debug_one_sheader(dt->sections[i]);
            return (symtab);
        }
    }
    return (NULL);
}

void read_and_store_syms(t_data *dt)
{
    Elf64_Shdr  symtab_section_h = (Elf64_Shdr)dt->sections[dt->symtab_index_in_sh];
    int         sym_nb = symtab_section_h.sh_size / symtab_section_h.sh_entsize;
    int         i;
    // printf(C_G_RED"[QUICK DEBUG] sym_nb: %d"C_RES"\n", sym_nb);
    for (i = 0; i < sym_nb; i++)
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

void handle_64(t_data *dt)
{
    // t_lst       *syms;
    // int         symtab_index_in_sh;
    if ((dt->header = (Elf64_Ehdr *)dt->ptr) == NULL)
        exit_error("eheader");
    // debug_eheader(*dt->header);
    if ((dt->sections = (Elf64_Shdr *)((char *)dt->ptr + dt->header->e_shoff)) == NULL)
        exit_error("sheader");
    // debug_one_sheader(*dt->sections);
    if ((dt->symtab = get_symtab(dt)) == NULL)
        exit_error("symtab");
    read_and_store_syms(dt);
    // display_raw_symtab(dt->symtab, dt->ptr, dt->sections, dt->sections[dt->symtab_index_in_sh]);
}

void nm(t_data *dt)
{
    int magic_number;

    magic_number = *(int *)dt->ptr;
    if (magic_number == ELF_MAGIC)
        handle_64(dt);
}

int main(int ac, char **av)
{
    t_data  dt;
    int     fd;
    struct  stat buf;

    if (ac != 2)
        exit_error("args");
    if ((fd = open(av[1], O_RDONLY)) < 0)
        exit_error("open");
    if (fstat(fd, &buf) < 0) // pour recuperer la taille du fichier
        exit_error("fstat");
    if ((dt.ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) // copie le fichier + lien *ptr0
        exit_error("mmap");
    nm(&dt);
    if (munmap(dt.ptr, buf.st_size) < 0)
        exit_error("munmap");
    // printf("end\n");
    free_all_malloc();
    return (0);
}
