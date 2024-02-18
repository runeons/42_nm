// gcc -Wall -Wextra -Werror -fsanitize=address -g3 srcs/simple_tester.c -I ./libft/includes -I ./includes -L ./libft/ -lft -o tester && ./tester

#include <stdio.h>
#include <fcntl.h>
#include <libft.h>
#include <get_next_line.h>
#include <sys/stat.h>
#include <utils_colors.h>

# define MAX_LEN 4096 // TO DO
# define NM_LETTERS "wWaUtTdDrRbB" // cf. symbols

// TO DO check_line_format

// int    is_nm_letter(char c)
// {
//     for (int i = 0; i < (int)ft_strlen(NM_LETTERS); i++)
//     {
//         if (c == NM_LETTERS[i])
//             return (1);
//     }
//     return (0);
// }

// void    exit_parsing(char *line)
// {
//     printf(C_G_RED"Error:"C_G_WHITE"%s\n"C_RES, line);
// }

// void    check_line_format(char *line)
// {
//     const char *ptr = line;

//     if (ft_strlen(line) <= 20)
//         exit_err("Error: file is not an nm output.\n");
//     for (int i = 0; i < 16; i++)
//     {
//         if (!ft_isdigit(*ptr) && (*ptr) != ' ')
//             exit_err("Error: file is not an nm output.\n");
//         ptr++;
//     }
//     if (*ptr++ != ' ')
//         exit_err("Error: file is not an nm output.\n");
//     if (is_nm_letter(*ptr++))
//         exit_err("Error: file is not an nm output.\n");
//     if (*ptr++ != ' ')
//         exit_err("Error: file is not an nm output.\n");
//     if (!ft_isalpha(*ptr++))
//         exit_err("Error: file is not an nm output.\n");
// }

void clean(char *file, char *res ,int *size)
{
    int     i = 0;
    int     fd = 0;
    char    *line = NULL;
    struct  stat buf;

    if ((fd = open(file, O_RDONLY)) == -1)
        exit_err("open");
    if (fstat(fd, &buf) < 0)
        exit_err("fstat");
    ft_memset(res, '\0', buf.st_size);
    while (get_next_line(fd, &line) > 0)
    {
        // check_line_format(line);
        int n = ft_strlen(line) - 16;
        strncpy(&res[i], line + 17, n);
        res[i + n - 1] = '\n';
        free(line);
        i += n;
    }
    res[i] = '\0';
    *size = i;
    close(fd);
}

int    compare(char *mine, char *syst, int len)
{
    int i = 0;

    for (i = 0; i < len; i++) // TO DO TMP
    {
        if (mine[i] != syst[i])
            return (i);
        i++;
    }
    return (-1);
}

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    char    *mine = NULL;
    char    *syst = NULL;
    int     mine_len;
    int     syst_len;
    int     cmp;
    if (ac != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", av[0]);
        exit(EXIT_FAILURE);
    }

    char command_mine[256];
    char command_syst[256];
    snprintf(command_mine, sizeof(command_mine), "./ft_nm %s > tmp_mine", av[1]);
    snprintf(command_syst, sizeof(command_syst), "nm -a %s > tmp_syst", av[1]);
    printf("Testing for : %s\n", av[1]);
    if ((mine = malloc(MAX_LEN)) == NULL)
        exit_err("Allocation failure");
    if ((syst = malloc(MAX_LEN)) == NULL)
        exit_err("Allocation failure");
    system(command_mine);
    // system("./ft_nm a.out > tmp_mine");
    clean("./tmp_mine", mine, &mine_len);
    // system("rm tmp_mine");
    system(command_syst);
    // system("nm -a a.out > tmp_syst");
    clean("./tmp_syst", syst, &syst_len);
    // system("rm tmp_syst");
    if (mine_len != syst_len)
        printf("mine_len != syst_len (%d != %d)\n", mine_len, syst_len);
    cmp = compare(mine, syst, mine_len);
    if (cmp != -1)
    {
        printf(C_G_RED"Error:"C_RES" files differ from position %d\n", cmp);
        printf(C_G_RED  "[MINE]: %.32s"C_RES"\n", &mine[cmp]);
        printf(C_G_GREEN"[SYST]: %.32s"C_RES"\n", &syst[cmp]);
    }
    else
        printf(C_G_GREEN  "[OK]"C_RES"\n");
    free(mine);
    free(syst);
    return (0);
}
