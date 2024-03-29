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
# include <utils_options.h>

// options
int     init_options_sort(t_lst *act_options);
int     init_options_filter(t_lst *act_options);

// init
char    **parse_input(t_parsed_cmd *parsed_cmd, int ac, char **av, int *len);
void    init_elf_ptrs_64(t_data *dt);
void    init_elf_ptrs_32(t_data *dt);

// read / fill / type / display
void    read_and_store_syms(t_data *dt);
void    fill_sym(t_data *dt, int index);
char    compute_type(t_data *dt, t_sym *sym);
void    display_syms(t_data *dt, t_lst *syms, int filter);

// sort
int     compare_names(const void *a, const void *b);
int     compare_values(const void *a, const void *b);
int     compare_types(const void *a, const void *b);
void    sort_syms(t_lst **syms, int sort_type);

// check
void    check_sheader_format_64(Elf64_Shdr sh);
void    check_sheader_format_32(Elf32_Shdr sh);
void    check_offset_boundaries(t_data *dt, uint64_t offset);

// debug
void    debug_to_fix(t_data *dt, int i);
void    debug_eheader(const Elf64_Ehdr h);
void    debug_one_sheader(const Elf64_Shdr h);
void    debug_one_sym(const Elf64_Sym sym);
void    debug_one_tsym(t_sym sym);

// exit
void    exit_error(const char *msg);
void    exit_corrupted(const char *msg);
void    warning(const char *msg);
void    exit_msg(const char *msg);
void    exit_error_close(const char *msg, int fd);

#endif
