#ifndef NM_FUNCTIONS_H
# define NM_FUNCTIONS_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <elf.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <libft.h>
# include <utils_colors.h>
# include <nm_data.h>
# include <errno.h>

void    exit_error(const char *msg);

// debug
void    debug_eheader(const Elf64_Ehdr h);
void    debug_one_sheader(const Elf64_Shdr h);
void    debug_one_sym(const Elf64_Sym sym);

// symbols
char    capitalise(char letter, unsigned char bind);
char    calc_letter(const Elf64_Sym sym, unsigned char type, unsigned char bind);
void    init_sym(t_data *dt, int index);
void    fill_sym(t_data *dt, int index);

// display
void    display_one_sym(t_sym *sym);
void    display_syms(t_lst *syms);

// sort
void    lower_if_needed(char **name);
int     compare_sym(const void *a, const void *b);
void    sort_syms(t_lst **syms);

#endif
