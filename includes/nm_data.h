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

typedef struct s_type
{
    char            letter;
    int             capitalise;
    char            *section_name;
    // unsigned char   raw_type;
    // unsigned char   raw_bind;
}              t_type;

typedef struct  s_sym
{
    Elf64_Sym       *raw;
    uint64_t        value;
    char            letter;
    char            *name;
    char            *section_name;
    unsigned char   type;
    unsigned char   bind;
}               t_sym;

typedef struct  s_data
{
    int         fstat_size;
    char        *ptr;
    Elf64_Ehdr  *ehdr;                  // debut ELF
    Elf64_Shdr  *shdr;                  // debut sections
    Elf64_Shdr  *sh_strtab;             // section ehdr strtab
    char        *sh_strtab_p;           // debut section strtab
    int         symtab_index;           // index de symtab in shdr
    Elf64_Sym   *symtab;                // debut symtab
    t_lst       *syms;
    t_lst       *types;
}					t_data;

#endif