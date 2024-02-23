#ifndef NM_DATA_H
# define NM_DATA_H

# define ELF_MAGIC   0x464c457f
# define PF_PADDING  16
 
# define PFsizeof    "lu"
# define PFu_c8      "hhu"
# define PFu_16      "hu"
# define PFu_32      "u"
# define PFu_64      "lu"

# define ELF_TYPE_UNKNOWN 0
# define ELF_TYPE_32 1
# define ELF_TYPE_64 2

// options
# define S_ALPHA     1
# define S_REVERSE   2  // -r
# define S_NO_SORT   3  // -p
# define F_EXTERN_ONLY     1   // -g
# define F_UNDEFINED_ONLY  2   // -u
# define F_NO_DEBUG        3   //
# define F_ALL             4   // -a

typedef int (*cmp_function)(const void *a, const void *b);

typedef struct s_type
{
    char            *section_name;
    char            type;
}              t_type;

typedef struct  s_sym
{
    Elf64_Sym       *raw64;
    Elf32_Sym       *raw32;
    uint64_t        value;
    char            type;
    char            *name;
    char            *section_name;
    unsigned char   raw_type;
    unsigned char   raw_bind;
}               t_sym;

typedef struct  s_data
{
    t_lst       *act_options;
    int         fstat_size;
    char        *ptr;
    Elf64_Ehdr  *ehdr64;                  // debut ELF
    Elf64_Shdr  *shdr64;                  // debut sections
    Elf64_Shdr  *sh_strtab64;             // section ehdr strtab
    char        *sh_strtab_p;           // debut section strtab
    int         symtab_index;           // index de symtab in shdr
    Elf64_Sym   *symtab64;                // debut symtab
    t_lst       *syms;
    t_lst       *types;
    int         sort;
    int         filter;
}					t_data;

#endif
