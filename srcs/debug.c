#include <nm_functions.h>

void    debug_to_fix(t_data *dt, int i)
{
    if (dt)
    {
        t_lst *sym_node = ft_lst_get_node_at_index(&dt->syms, i);
        if (sym_node)
        {
            t_sym *current_sym = (t_sym *)sym_node->content;
            // if (current_sym->letter == 'r')
            if (current_sym->raw->st_shndx == 20)
            {
                printf(C_G_RED"[%s]"C_RES"\n", current_sym->name);
                debug_one_sym(*current_sym->raw);
            }
        }
    }
}

void    debug_eheader(const Elf64_Ehdr h)
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

void    debug_one_sheader(const Elf64_Shdr h)
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

void    debug_one_sym(const Elf64_Sym sym)
{
    printf("\n");
    printf("----------   \n");
    printf("Elf64_Sym   | %"PFsizeof"\n", sizeof(sym));
    printf("st_name     : %"PFu_32"\n", sym.st_name);
    printf("st_info     : %"PFu_c8"\n", sym.st_info);
    printf("st_other    : %"PFu_c8"\n", sym.st_other);
    printf("st_shndx    : %"PFu_16"\n", sym.st_shndx);
    printf("st_value    : %"PFu_64"\n", sym.st_value);    // N-dependant
    printf("st_size     : %"PFu_64"\n", sym.st_size);
}
