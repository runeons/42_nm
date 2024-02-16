#include <nm_functions.h>

char capitalise(char letter, unsigned char bind)
{
    if (bind == STB_LOCAL)
        return (letter);
    else if (bind == STB_GLOBAL)
        return (letter - 32);
    return '?';
}

char calc_letter(const Elf64_Sym sym, unsigned char type, unsigned char bind)
{
    if (sym.st_value == 0)
        if (bind == STB_WEAK) 
            return 'w';
        else if (type == STT_FILE)
            return 'a';
        else
            return 'U';
    else if (bind == STB_WEAK)
        return 'W';
    else if (type == STT_FUNC)
        return capitalise('t', bind);

    if (type == STT_OBJECT)
        return 'Z';
    if (type == STT_COMMON)
        return 'C';
    if (type == STT_SECTION)
        return 'Y';

    return '=';
}

void display_one_raw_sym(Elf64_Sym *symtab, char *ptr, Elf64_Shdr *sections, const Elf64_Shdr symtab_section_h, int j)
{
    char            *strtab;
    unsigned char   info;
    unsigned char   type;
    unsigned char   bind;
    uint64_t        value; // Elf64_Addr
    char            letter;
    
    strtab = (char *)(ptr + sections[symtab_section_h.sh_link].sh_offset);
    value = (uint64_t)(symtab[j].st_value);
    info = (unsigned char)(symtab[j].st_info);
    type = ELF64_ST_TYPE((int)info);
    bind = ELF64_ST_BIND((int)info);
    letter = calc_letter(symtab[j], type, bind);
    // if (letter == '=')
        // debug_one_sym(symtab[j]);
    if (value == 0 && letter != 'a')
        printf("%16c %c %s (%d | %d)\n", ' ', letter, strtab + symtab[j].st_name, type, bind);
    else
        printf("%016"PFu_64" %c %s (%d | %d)\n", value, letter, strtab + symtab[j].st_name, type, bind);
}

void display_raw_symtab(Elf64_Sym *symtab, char *ptr, Elf64_Shdr *sections, const Elf64_Shdr symtab_section_h)
{
    int     j;
    int     sym_nb = symtab_section_h.sh_size / symtab_section_h.sh_entsize;

    // printf("sym_nb = %d\n", sym_nb);
    for (j = 0; j < sym_nb; j++)
    {
        display_one_raw_sym(symtab, ptr, sections, symtab_section_h, j);
    }
}

Elf64_Sym    *get_symtab(t_data *dt)
{
    int         i;
    Elf64_Sym   *symtab;
    
    printf("get_symtab\n");
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

void create_sym_node(t_data *dt, int index)
{
    t_sym *sym = NULL;

    sym = mmalloc(sizeof(t_sym));
    if (sym == NULL)
        exit_error("create_sym_node\n");
    sym->curr_sym   = &dt->symtab[index];
    sym->value      = 0;
    sym->letter     = '\0';
    sym->name       = NULL;
    // debug_one_sym(*sym->curr_sym);
    ft_lst_add_node_back(&dt->syms, ft_lst_create_node(sym));
}

void read_and_store_syms(t_data *dt)
{
    // allocate 1 tab/linked list of syms
    // for each sym in symnb
    //   create new t_sym and add it to the list
    Elf64_Shdr  symtab_section_h = (Elf64_Shdr)dt->sections[dt->symtab_index_in_sh];
    int         sym_nb = symtab_section_h.sh_size / symtab_section_h.sh_entsize;
    int         i;

    printf("sym_nb = %d\n", sym_nb);
    for (i = 0; i < sym_nb; i++)
    {
        create_sym_node(dt, i); // create node & add it to list
        // calc values
        // handle_and_store_one_sym(symtab, ptr, sections, symtab_section_h, j);
    }
}

void handle_64(t_data *dt)
{
    // t_lst       *syms;
    // int         symtab_index_in_sh;-
    if ((dt->header = (Elf64_Ehdr *)dt->ptr) == NULL)
        exit_error("eheader");
    // debug_eheader(*dt->header);
    if ((dt->sections = (Elf64_Shdr *)((char *)dt->ptr + dt->header->e_shoff)) == NULL)
        exit_error("sheader");
    // debug_one_sheader(*dt->sections);
    if ((dt->symtab = get_symtab(dt)) == NULL)
        exit_error("symtab");
    display_raw_symtab(dt->symtab, dt->ptr, dt->sections, dt->sections[dt->symtab_index_in_sh]);
    read_and_store_syms(dt);
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
    // if (munmap(ptr, buf.st_size) < 0)
    //     exit_error("munmap");
    // printf("end\n");
    return (0);
}
