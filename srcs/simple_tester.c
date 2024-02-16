// gcc -Wall -Wextra -Werror -fsanitize=address -g3 srcs/simple_tester.c -I ./libft/includes -I ./includes -L ./libft/ -lft -o tester && ./tester

#include <stdio.h>
#include <fcntl.h>
#include <libft.h>
#include <get_next_line.h>
#include <sys/stat.h>
#include <utils_colors.h>

# define MAX_LEN 5000 // TO DO

// TO DO check line format

void clean(char *file, char *res ,int *size)
{
    int     i = 0;
    int     fd = 0;
    char    *line = NULL;
    struct  stat buf;

    if ((fd = open(file, O_RDONLY)) == -1)
        exit_error("open");
    if (fstat(fd, &buf) < 0)
        exit_error("fstat");
    ft_memset(res, '\0', buf.st_size);
    while (get_next_line(fd, &line) > 0)
    {
        int n = ft_strlen(line) - 16;
        strncpy(&res[i], line + 17, n);
        res[i + n - 1] = '\n';
        // printf("[line] : %s\n", line);
        // printf("[res ] : %s\n", &res[i]);
        free(line);
        i += n;
    }
    res[i] = '\0';
    // write(1, &res[0], ft_strlen(res));
    *size = i;
    close(fd);
}

int    compare(char *mine, char *syst, int len)
{
    int i = 0;

    for (i = 0; i < len; i++)
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

    if ((mine = malloc(MAX_LEN)) == NULL)
        exit_error("Allocation failure");
    if ((syst = malloc(MAX_LEN)) == NULL)
        exit_error("Allocation failure");
    system("./ft_nm a.out > tmp_mine");
    clean("./tmp_mine", mine, &mine_len);
    // system("rm tmp_mine");
    system("nm -a a.out > tmp_syst");
    clean("./tmp_syst", syst, &syst_len);
    // system("rm tmp_syst");
    if (mine_len != syst_len)
        printf("mine_len != syst_len (%d != %d)\n", mine_len, syst_len);
    cmp = compare(mine, syst, mine_len);
    if (cmp != -1)
    {
        printf(C_G_GREEN"[SYST]: %.32s"C_RES"\n", &syst[cmp]);
        printf(C_G_RED  "[MINE]: %.32s"C_RES"\n", &mine[cmp]);
    }
    else
        printf(C_G_GREEN  "[OK]"C_RES"\n");
    free(mine);
    free(syst);
    return (0);
}
