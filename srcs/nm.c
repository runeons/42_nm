#include <nm_functions.h>

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

void lower_if_needed(char **name)
{
    size_t len = ft_strlen(*name);

    for (size_t i = 0; i < len; i++)
    {
        if ((*name)[i] >= 'A' && (*name)[i] <= 'Z')
            (*name)[i] = (*name)[i] + 32;
    }
}
int compare_sym(const void *a, const void *b)
{
    int res;

    t_sym *sym_a = *(t_sym **)a;
    t_sym *sym_b = *(t_sym **)b;

    char *name_a = ft_strdup(sym_a->name);
    char *name_b = ft_strdup(sym_b->name);

    while (*name_a == '_')
        name_a++;
    while (*name_b == '_')
        name_b++;
    lower_if_needed(&name_a);
    lower_if_needed(&name_b);
    res = ft_strcmp(name_a, name_b);
    return res;
}

void sort_list(t_lst **syms)
{
    int     sym_count = ft_lst_size(*syms);
    if (sym_count > 1)
    {
        t_sym **sym_array = mmalloc(sym_count * sizeof(t_sym *));
        if (sym_array == NULL)
            exit_error("Malloc failure");
        t_lst   *current = *syms;
        for (int i = 0; i < sym_count; i++)
        {
            sym_array[i] = (t_sym *)current->content;
            current = current->next;
        }
        qsort(sym_array, sym_count, sizeof(t_sym *), compare_sym);
        current = *syms;
        for (int i = 0; i < sym_count; i++)
        {
            current->content = sym_array[i];
            current = current->next;
        }
    }
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
