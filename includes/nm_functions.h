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

// exit
void    exit_error(const char *msg);
void    exit_corrupted(const char *msg);
void    exit_msg(const char *msg);
void    exit_error_close(const char *msg, int fd);

// debug
void    debug_to_fix(t_data *dt, int i);
void    debug_eheader(const Elf64_Ehdr h);
void    debug_one_sheader(const Elf64_Shdr h);
void    debug_one_sym(const Elf64_Sym sym);
void    debug_one_tsym(t_sym sym);

// store / type
void    fill_sym(t_data *dt, int index);
void    read_and_store_syms(t_data *dt);

// display
void    display_one_sym(t_sym *sym);
void    display_syms(t_lst *syms);

// sort
int     compare_sym(const void *a, const void *b);
int     compare_values(const void *a, const void *b);
int     compare_letters(const void *a, const void *b);
void    sort_syms(t_lst **syms);

// check
void    check_sheader_format(Elf64_Shdr sh);
void    check_offset_boundaries(t_data *dt, uint64_t offset);

#endif
