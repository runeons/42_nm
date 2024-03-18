#include <nm_functions.h>

static char **allocate_files_array(int size)
{
    char **files;

    if ((files = (char **)mmalloc(size * sizeof(char *))) == NULL)
        exit_error("Malloc failure");
    return files;
}

static char **create_all_files(t_parsed_cmd *parsed_cmd, int files_nb, int *len)
{
    char    **files;
    int     i;

    files = allocate_files_array(files_nb);
    t_lst *current = parsed_cmd->not_options;
    i = 0;
    while (current)
    {
        files[i] = (char *)current->content;
        current = current->next;
        i++;
    }
    *len = files_nb;
    return files;
}

static char **create_default_file(t_parsed_cmd *parsed_cmd, int *len)
{
    char **files;
    
    files = allocate_files_array(1);
    if ((files[0] = ft_strdup("a.out")) == NULL)
        exit_error("Malloc failure");
    ft_lst_add_node_back(&parsed_cmd->not_options, ft_lst_create_node(files[0]));
    *len = 1;
    return files;
}

char **parse_input(t_parsed_cmd *parsed_cmd, int ac, char **av, int *len)
{
    int files_nb;
    
    *parsed_cmd = parse_options(ac, av);
    files_nb = ft_lst_size(parsed_cmd->not_options);
    if (files_nb == 0)
        return create_default_file(parsed_cmd, len);
    else
        return create_all_files(parsed_cmd, files_nb, len);
}