#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <elf.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define ELF_MAGIC 0x464c457f

void exit_error(char *msg)
{
    printf("%s\n", msg);
    exit(1);
}

#define PFsizeof    "lu"
#define PFu_c8      "hhu"
#define PFu_16      "hu"
#define PFu_32      "u"
#define PFu_64      "lu"

// void    print_sections()
// {
//     Elf64_Ehdr   *h;
//     sections = (Elf32_Shdr *)((char *)ptr + headers->e_shoffs);

    // for (i = 0; i < header->e_m; i++)sym_nb
    //     if (sections[i].sh_type == SHT_SYMTAB) {
    //         symtab = (Elf32_Sym *)((char *)ptr + sectionss[i].ssh_offset);
        //    sym_nb; }
// }

void    print_eheader(const Elf64_Ehdr h)
{
    printf("\n");
    printf("----------   \n");
    printf("Elf64_Ehdr  | %"PFsizeof"\n", sizeof(h));
    printf("e_type      : %"PFu_16"\n", h.e_type);
    printf("e_machine   : %"PFu_16"\n", h.e_machine);
    printf("e_version   : %"PFu_32"\n", h.e_version);
    printf("e_entry     : %"PFu_64"\n", h.e_entry);     // N-dependant
    printf("e_phoff     : %"PFu_64"\n", h.e_phoff);     // N-dependant
    printf("e_shoff     : %"PFu_64"\n", h.e_shoff);     // N-dependant
    printf("e_flags     : %"PFu_32"\n", h.e_flags);
    printf("e_ehsize    : %"PFu_16"\n", h.e_ehsize);
    printf("e_phentsize : %"PFu_16"\n", h.e_phentsize);
    printf("e_phnum     : %"PFu_16"\n", h.e_phnum);
    printf("e_shentsize : %"PFu_16"\n", h.e_shentsize);
    printf("e_shnum     : %"PFu_16"\n", h.e_shnum);
    printf("e_shstrndx  : %"PFu_16"\n", h.e_shstrndx);
}

void    print_sheader(const Elf64_Shdr h)
{
    printf("\n");
    printf("----------   \n");
    printf("Elf64_Shdr  | %"PFsizeof"\n", sizeof(h));
    printf("sh_name     : %"PFu_32"\n", h.sh_name);
    printf("sh_type     : %"PFu_32"\n", h.sh_type);
    printf("sh_flags    : %"PFu_64"\n", h.sh_flags);
    printf("sh_addr     : %"PFu_64"\n", h.sh_addr);     // N-dependant
    printf("sh_offset   : %"PFu_64"\n", h.sh_offset);   // N-dependant
    printf("sh_size     : %"PFu_64"\n", h.sh_size);
    printf("sh_link     : %"PFu_32"\n", h.sh_link);
    printf("sh_info     : %"PFu_32"\n", h.sh_info);
    printf("sh_addralign: %"PFu_64"\n", h.sh_addralign);
    printf("sh_entsize  : %"PFu_64"\n", h.sh_entsize);
}

void    print_sym(const Elf64_Sym h)
{
    printf("\n");
    printf("----------   \n");
    printf("Elf64_Sym   | %"PFsizeof"\n", sizeof(h));
    printf("st_name     : %"PFu_32"\n", h.st_name);
    printf("st_info     : %"PFu_c8"\n", h.st_info);
    printf("st_other    : %"PFu_c8"\n", h.st_other);
    printf("st_shndx    : %"PFu_16"\n", h.st_shndx);
    printf("st_value    : %"PFu_64"\n", h.st_value);    // N-dependant
    printf("st_size     : %"PFu_64"\n", h.st_size);
}

// void display_sym(struct symtab_command *sym, char *ptr)
// {
//     // printf("%d %d %d\n", sym->nsyms, sym->symoff, sym->stroff);
//     int             i;
//     char            *string_table;
//     struct nlist_64 *array;
    
//     array = (void *)ptr + sym->symoff;
//     string_table = (void *)ptr + sym->stroff;
//     for (i = 0; i < sym->nsyms; i++)
//     {
//         printf("%s\n", string_table + array[i].n_un.n_strx);
//     }
// }

void handle_64(char *ptr)
{
    printf("this is x64\n");
    // int                     ncmds;
    Elf64_Ehdr   *header;
    Elf64_Shdr   *sections;
    Elf64_Sym    *symtab;
    int          i;
    int          j;
    // struct load_command     *lc;
    // struct symtab_command   *sym;

    header = (Elf64_Ehdr *)ptr;
    if (header == NULL)
        exit_error("eheader");
    print_eheader(*header);
    sections = (Elf64_Shdr *)((char *)ptr + header->e_shoff);
    if (sections == NULL)
        exit_error("sheader");
    // print_sheader(*sections);
    for (i = 0; i < header->e_shnum; i++)
    {
        if (sections[i].sh_type == SHT_SYMTAB)
        {
            symtab = (Elf64_Sym *)((char *)ptr + sections[i].sh_offset);
            break;
        }
    }
    printf("%d\n", i);
    print_sheader(sections[i]);
    print_sym(*symtab);

    int             sym_nb = sections[i].sh_size / sections[i].sh_entsize;
    char            *string_table;
    char            *sym_names;

    printf("sym_nb = %d\n", sym_nb);
    string_table = (void *)ptr + symtab->st_name;
    sym_names = (char *)(ptr + sections[sections[i].sh_link].sh_offset);
    for (j = 0; j < sym_nb; j++)
    {
        printf("name : %s\n", sym_names + symtab[j].st_name);
    }

    // for (j = 0; j < sym_nb; j++)
    // {
    //     printf("%d\n", j);
    //     print_sym(symtab[j]);
    // }
    // ncmds = header->ncmds;
    // lc = (void *)ptr + sizeof(*header);
    // for (i = 0; i < ncmds; ++i)
    // {
    //     if (lc->cmd == LC_SYMTAB)
    //     {
    //         printf("LC_SYMTAB\n");
    //         sym = (struct symtab_command *) lc;
    //         printf("nsyms: %d\n", sym->nsyms);
    //         display_sym(sym, ptr);
    //         break;
    //     }
    //     lc = (void *)lc + lc->cmdsize; // (void *) important pour controler
    // }
}

void nm(char *ptr)
{
    int magic_number;

    magic_number = *(int  *)ptr;
    if (magic_number == ELF_MAGIC) // #0xfeedfacf  7f 45 4c 46 46 4c 45 7f MH_MAGIC_64
        handle_64(ptr);
}

int main(int ac, char **av)
{
    int fd;
    char *ptr;
    struct stat buf;

    fd = open(av[1], O_RDONLY);
    if (fd < 0)
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
