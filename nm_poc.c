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

void    print_one_sheader(const Elf64_Shdr h)
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

void    print_one_sym(const Elf64_Sym h)
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

// #define ELF64_ST_TYPE(info)          ((info) & 0xf)

void display_one_sym(Elf64_Sym *symtab, char *ptr, Elf64_Shdr *sections, const Elf64_Shdr symtab_section_h, int j)
{
    char    *string_table;
    char    *sym_names;

    string_table = (void *)ptr + symtab->st_name;
    sym_names    = (char *)(ptr + sections[symtab_section_h.sh_link].sh_offset);
    printf("name : %s\n", sym_names + symtab[j].st_name);
}

void display_symtab(Elf64_Sym *symtab, char *ptr, Elf64_Shdr *sections, const Elf64_Shdr symtab_section_h)
{
    int     j;
    int     sym_nb = symtab_section_h.sh_size / symtab_section_h.sh_entsize;

    printf("sym_nb = %d\n", sym_nb);
    for (j = 0; j < sym_nb; j++)
    {
        display_one_sym(symtab, ptr, sections, symtab_section_h, j);
        // printf("info : %s\n", ELF64_ST_TYPE((int)(sym_names + symtab[j].st_info)));
    }
}

void handle_64(char *ptr)
{
    printf("this is x64\n");
    Elf64_Ehdr   *header;
    Elf64_Shdr   *sections;
    Elf64_Sym    *symtab;
    int          i;

    if ((header = (Elf64_Ehdr *)ptr) == NULL)
        exit_error("eheader");
    // print_eheader(*header);
    if ((sections = (Elf64_Shdr *)((char *)ptr + header->e_shoff)) == NULL)
        exit_error("sheader");
    // print_one_sheader(*sections);
    for (i = 0; i < header->e_shnum; i++)
    {
        if (sections[i].sh_type == SHT_SYMTAB)
        {
            symtab = (Elf64_Sym *)((char *)ptr + sections[i].sh_offset); // debut de la symtab
            print_one_sheader(sections[i]);
            // print_one_sheader(sections[36]);
            // print_one_sym(symtab[0]);
            print_one_sym(symtab[3]);
            display_symtab(symtab, ptr, sections, sections[i]);
            display_one_sym(symtab, ptr, sections, sections[i], i);
            break;
        }
    }
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
