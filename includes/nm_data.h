#ifndef NM_DATA_H
# define NM_DATA_H

#define ELF_MAGIC   0x464c457f
#define PF_PADDING  16

#define PFsizeof    "lu"
#define PFu_c8      "hhu"
#define PFu_16      "hu"
#define PFu_32      "u"
#define PFu_64      "lu"

typedef struct  s_sym
{
    Elf64_Sym       *curr_sym;
    uint64_t        value;
    char            letter;
    char            *name;
}               t_sym;


typedef struct  s_data
{
    char        *ptr;
    Elf64_Ehdr  *header;        // pointeur vers debut ELF
    Elf64_Shdr  *sections;      // pointeur vers debut sections
    Elf64_Sym   *symtab;        // pointeur vers debut symtab
    int         symtab_index_in_sh;
    t_lst       *syms;
}					t_data;

#endif
