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
//     printf(C_G_RED"Error :"C_G_WHITE"%s\n"C_RES, line);
// }

// void    check_line_format(char *line)
// {
//     const char *ptr = line;

//     if (ft_strlen(line) <= 20)
//         exit_err("Error : file is not an nm output.\n");
//     for (int i = 0; i < 16; i++)
//     {
//         if (!ft_isdigit(*ptr) && (*ptr) != ' ')
//             exit_err("Error : file is not an nm output.\n");
//         ptr++;
//     }
//     if (*ptr++ != ' ')
//         exit_err("Error : file is not an nm output.\n");
//     if (is_nm_letter(*ptr++))
//         exit_err("Error : file is not an nm output.\n");
//     if (*ptr++ != ' ')
//         exit_err("Error : file is not an nm output.\n");
//     if (!ft_isalpha(*ptr++))
//         exit_err("Error : file is not an nm output.\n");
// }

typedef struct  s_cmd
{
    char    cmd[256];
    char    *res;
    char    *exec;
    char    *filename;
    char    *output;
    int     output_len;
}               t_cmd;

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
        if (n <= 1)
            exit_err("error in file content\n");
        strncpy(&res[i], line + 17, n);
        if ((i + n - 1) < 0 || (i + n - 1) > (int)ft_strlen(res))
            exit_err("error in index\n");
        res[i + n - 1] = '\n';
        // free(line);
        i += n;
    }
    res[i] = '\0';
    *size = i;
    close(fd);
}

int    compare_outputs(char *res1, char *res2, int len)
{
    int i = 0;

    for (i = 0; i < len; i++)
    {
        if (res1[i] != res2[i])
            return (i);
        i++;
    }
    return (-1);
}

void    get_command(char cmd[256], char *format, char *exec, char *file, char *output)
{
    int len;

    if (ft_strlen(file) > 230)
        exit_err("filename too long");
    len = ft_strlen(format) + ft_strlen(exec) + ft_strlen(file) + ft_strlen(output) - 5;
    if (len >= 256)
        exit_err("filename too long");
    snprintf(cmd, len, format, exec, file, output);
    // printf("cmd: %s\n", cmd);/
}

void    init(t_cmd *cmd, char *filename, char *exec, char *output)
{
    cmd->filename   = ft_strdup(filename);
    cmd->exec       = ft_strdup(exec);
    cmd->output     = ft_strdup(output);
}

void launch(t_cmd *cmd)
{
    struct stat buf;
    int         fd = 0;

    get_command(cmd->cmd, "%s %s > %s", cmd->exec, cmd->filename, cmd->output);
    system(cmd->cmd);
    if ((fd = open(cmd->output, O_RDONLY)) == -1)
        exit_err("open");
    if (fstat(fd, &buf) < 0)
        exit_err("fstat");
    if ((cmd->res = mmalloc(buf.st_size)) == NULL)
        exit_err("Allocation failure");
    ft_memset(cmd->res, '\0', buf.st_size);
    clean(ft_strjoin("./", cmd->output), cmd->res, &cmd->output_len);
}

void compare(t_cmd cmd1, t_cmd cmd2)
{
    int     cmp;

    if (cmd1.output_len != cmd2.output_len)
        printf(C_G_BLUE"Info  : "C_G_RED"[KO] "C_RES"Output lengths differ (%d != %d)\n", cmd1.output_len, cmd2.output_len);
    cmp = compare_outputs(cmd1.res, cmd2.res, cmd1.output_len);
    if (cmp != -1)
    {
        printf(C_G_RED"Result: [KO]"C_RES"\n");
        printf(C_G_RED"Error :"C_RES" files differ from position %d - please see below\n", cmp);
        printf(C_G_BLUE"Info  : "C_RED"[My result]"C_RES" [%.32s]\n", &cmd1.res[cmp]);
        printf(C_G_BLUE"Info  : "C_GREEN"[Expected]"C_RES" [%.32s]\n", &cmd2.res[cmp]);
    }
    else
        printf(C_G_GREEN"Result: [OK]"C_RES"\n");
}

int main(int ac, char **av)
{
    t_cmd   cmd1;
    t_cmd   cmd2;

    if (ac != 2)
        exit_err(C_G_RED"Error : "C_RES"filename required\n");
    init(&cmd1, av[1], "./ft_nm", "tmp1");
    init(&cmd2, av[1], "nm -a", "tmp2");
    launch(&cmd1);
    launch(&cmd2);
    printf(C_G_BLUE"Info  : "C_RES"comparing results on '%s'\n", av[1]);
    compare(cmd1, cmd2);
    free_all_malloc();
    return (0);
}