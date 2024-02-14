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

void    debug_eheader(const Elf64_Ehdr h);
void    debug_one_sheader(const Elf64_Shdr h);
void    debug_one_sym(const Elf64_Sym sym);

#endif
