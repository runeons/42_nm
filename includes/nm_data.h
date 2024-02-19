#ifndef NM_DATA_H
# define NM_DATA_H

# define ELF_MAGIC   0x464c457f
# define PF_PADDING  16
# define DEBUG       0
 
# define PFsizeof    "lu"
# define PFu_c8      "hhu"
# define PFu_16      "hu"
# define PFu_32      "u"
# define PFu_64      "lu"

typedef struct  s_sym
{
    Elf64_Sym       *raw;
    uint64_t        value;
    char            letter;
    char            *name;
    unsigned char   type;
    unsigned char   bind;
}               t_sym;

typedef struct  s_data
{
    int         fstat_size;
    char        *ptr;
    Elf64_Ehdr  *header;             // pointeur vers debut ELF
    Elf64_Shdr  *sections_hdrs;      // pointeur vers debut sections
    int         symtab_index;        // index de symtab in sections_hdrs
    Elf64_Sym   *symtab;             // pointeur vers debut symtab
    t_lst       *syms;
}					t_data;

#endif