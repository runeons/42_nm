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

void store_one_sym(Elf64_Sym *symtab, char *ptr, Elf64_Shdr *sections, const Elf64_Shdr symtab_section_h, int j)
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

void store_symbols(Elf64_Sym *symtab, char *ptr, Elf64_Shdr *sections, const Elf64_Shdr symtab_section_h)
{
    int     j;
    int     sym_nb = symtab_section_h.sh_size / symtab_section_h.sh_entsize;

    // printf("sym_nb = %d\n", sym_nb);
    for (j = 0; j < sym_nb; j++)
    {
        store_one_sym(symtab, ptr, sections, symtab_section_h, j);
    }
}

Elf64_Sym    *get_symtab(char *ptr, Elf64_Ehdr *header, Elf64_Shdr *sections, int *symtab_index_in_sh)
{
    int         i;
    Elf64_Sym   *symtab;
    
    printf("get_symtab\n");
    for (i = 0; i < header->e_shnum; i++)
    {
        if (sections[i].sh_type == SHT_SYMTAB)
        {
            symtab = (Elf64_Sym *)((char *)ptr + sections[i].sh_offset); // debut de la symtab
            *symtab_index_in_sh = i;
            return (symtab);
        }
    }
    return (NULL);
}

// void read_and_store_syms()
// {
//     // allocate 1 tab/linked list of syms
//     // for each sym in symnb
//     //   create new t_sym and add it to the list
//     int     j;
//     int     sym_nb = symtab_section_h.sh_size / symtab_section_h.sh_entsize;

//     // printf("sym_nb = %d\n", sym_nb);
//     for (j = 0; j < sym_nb; j++)
//     {
//         // handle_and_store_one_sym(symtab, ptr, sections, symtab_section_h, j);
//     }
// }

void handle_64(char *ptr)
{
    Elf64_Ehdr  *header;
    Elf64_Shdr  *sections;
    Elf64_Sym   *symtab;
    int         symtab_index_in_sh;

    if ((header = (Elf64_Ehdr *)ptr) == NULL)
        exit_error("eheader");
    // debug_eheader(*header);
    if ((sections = (Elf64_Shdr *)((char *)ptr + header->e_shoff)) == NULL)
        exit_error("sheader");
    // debug_one_sheader(*sections);
    if ((symtab = get_symtab(ptr, header, sections, &symtab_index_in_sh)) == NULL)
        exit_error("symtab");
    display_raw_symtab(symtab, ptr, sections, sections[symtab_index_in_sh]);
    // read_and_store_syms(symtab, ptr, sections, sections[symtab_index_in_sh]);
}

void nm(char *ptr)
{
    int magic_number;

    magic_number = *(int  *)ptr;
    if (magic_number == ELF_MAGIC)
        handle_64(ptr);
}

int main(int ac, char **av)
{
    int fd;
    char *ptr;
    struct stat buf;

    (void)ac; // TO DO
    if ((fd = open(av[1], O_RDONLY)) < 0)
        exit_error("open");
    if (fstat(fd, &buf) < 0)
        exit_error("fstat");
    ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED)
        exit_error("mmap");
    nm(ptr);
    // if (munmap(ptr, buf.st_size) < 0)
    //     exit_error("munmap");
    return (0);
}
