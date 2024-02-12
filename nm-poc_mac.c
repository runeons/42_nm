#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <sys/stat.h>
#include <sys/mman.h>

void exit_error(char *msg)
{
    printf("%s\n", msg);
    exit(1);
}

void display_sym(struct symtab_command *sym, char *ptr)
{
    // printf("%d %d %d\n", sym->nsyms, sym->symoff, sym->stroff);
    int             i;
    char            *string_table;
    struct nlist_64 *array;
    
    array = (void *)ptr + sym->symoff;
    string_table = (void *)ptr + sym->stroff;
    for (i = 0; i < sym->nsyms; i++)
    {
        printf("%s\n", string_table + array[i].n_un.n_strx);
    }
}

void handle_64(char *ptr)
{
    printf("x64\n");
    int                     i;
    int                     ncmds;
    struct mach_header_64   *header;
    struct load_command     *lc;
    struct symtab_command   *sym;

    header = (struct mach_header_64 *)ptr;
    ncmds = header->ncmds;
    lc = (void *)ptr + sizeof(*header);
    for (i = 0; i < ncmds; ++i)
    {
        if (lc->cmd == LC_SYMTAB)
        {
            printf("LC_SYMTAB\n");
            sym = (struct symtab_command *) lc;
            printf("nsyms: %d\n", sym->nsyms);
            display_sym(sym, ptr);
            break;
        }
        lc = (void *)lc + lc->cmdsize; // (void *) important pour controler
    }
}

void nm(char *ptr)
{
    (void)ptr;
    int magic_number;

    magic_number = *(int *)ptr;
    if (magic_number == MH_MAGIC_64) // #0xfeedfacf
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
    if (munmap(ptr, buf.st_size) < 0)
        exit_error("munmap");
    return (0);
}
