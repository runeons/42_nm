#include "utils_options.h"

t_option allowed_options[] =
{
    {'h', "help",           0, "",  NULL, "Give this help list"},
    {'a', "debug-syms",     0, "",  NULL, "Display all symbols, even debugger-only symbols"},
    {'g', "extern-only",    0, "",  NULL, "Display only external symbols"},
    {'r', "reverse-sort",   0, "",  NULL, "Reverse the order of the sort"},
    {'p', "no-sort",        0, "",  NULL, "Do not bother to sort the symbols in any order; print them in the order encountered"},
    {'u', "undefined-only", 0, "",  NULL, "Display only undefined symbols (those external to each object file)"},
};


void exit_error_options(const char *msg)
{
    printf("Error: nm: %s\n", msg);
    free_all_malloc();
    exit(1);
}


static int get_name_max_len()
{
    int max_len = 0;
    int curr_len = 0;

    for (size_t i = 0; i < ARRAY_SIZE(allowed_options); i++)
    {
        curr_len = ft_strlen(allowed_options[i].name) + 8 +  ft_strlen(allowed_options[i].param_name);
        if (curr_len > max_len)
            max_len = curr_len;
    }
    return (max_len);
}

void    display_extra_short_usage()
{
    printf("Usage: %s [OPTION...] FILE...\n", CMD_NAME);
}

void    display_short_usage()
{
    printf("Usage:\n");
    printf("    %s ", CMD_NAME);
    for (size_t i = 0; i < ARRAY_SIZE(allowed_options); i++)
    {
        if (allowed_options[i].need_param == 1)
            printf("[-%c %s] ", allowed_options[i].id, allowed_options[i].param_name);
        else if (allowed_options[i].need_param == 0)
            printf("[-%c] ", allowed_options[i].id);
    }
}

void    display_long_usage()
{
    printf("Usage: %s", CMD_NAME);
    for (size_t i = 0; i < ARRAY_SIZE(allowed_options); i++)
    {
        if (i % 4 == 0)
            printf("\n                ");
        if (allowed_options[i].need_param == 1)
            printf("[--%s %s] ", allowed_options[i].name, allowed_options[i].param_name);
        else if (allowed_options[i].need_param == 0)
            printf("[--%s] ", allowed_options[i].name);
    }
    printf("\n                HOST\n");
}

void    display_help()
{
    int max_len = get_name_max_len();
    char formatted_name[max_len + 1];

    display_extra_short_usage();
    printf("\n");
    printf("Description:\n");
    printf("    List symbols from object files.\n\n");
    printf("Options:\n");
    for (size_t i = 0; i < ARRAY_SIZE(allowed_options); i++)
    {
        if (allowed_options[i].need_param == 1)
            sprintf(formatted_name, "--%s=%s", allowed_options[i].name, allowed_options[i].param_name);
        else if (allowed_options[i].need_param == 0)
            sprintf(formatted_name, "--%s ", allowed_options[i].name);
        printf("    -%c, %-*s %s\n", allowed_options[i].id, max_len, formatted_name, allowed_options[i].description);
    }
    printf("\n");
}

t_option *get_option(t_lst *act_options, char c)
{
    while (act_options != NULL)
    {
        t_option *tmp = (t_option *)act_options->content;
        if (tmp->id == c)
            return (tmp);
        act_options = act_options->next;
    }
    return (NULL);
}

void    print_option(t_lst *act_options, char c)
{
    t_option *option = get_option(act_options, c);
    if (option->id != 0)
    {
        printf("id: %c\n", option->id);
        printf("name: %s\n", option->name);
        printf("need_param: %d\n", option->need_param);
        printf("description: %s\n", option->description);
        printf("param: %s\n", option->param);
    }
}

static void debug_option(void *content)
{
    if (content)
    {
        t_option *option = (t_option *)content;
        printf("id: %c, name: %s, need_param: %d, param: %s, \n", option->id, option->name, option->need_param, option->param);
    }
}

void    debug_activated_options(t_lst *act_options)
{
    if (act_options)
    {
        printf("\n\n");
        printf("Activated options:\n");
        ft_lst_iter_content(act_options, debug_option);
        printf("\n");
    }
}

int is_activated_option(t_lst *act_options, char c)
{
    if (act_options)
    {
        t_lst *current_node = act_options;

        while (current_node)
        {
            t_option *current_option = (t_option *)current_node->content;
            if (current_option->id == c)
                return(1);
			current_node = current_node->next;
        }
    }
    return(0);
}

static t_option    *is_allowed_option(char c)
{
    for (size_t i = 0; i < ARRAY_SIZE(allowed_options); i++) 
    {
        if (allowed_options[i].id == c)
            return &allowed_options[i];
    }
    return (NULL);
}

static t_option    *is_allowed_option_long(char *str)
{
    int len = 0;

    if (str[1] != '-')
        return (NULL);
    for (size_t i = 0; i < ARRAY_SIZE(allowed_options); i++) 
    {
        if (ft_strlen((char *)str + 2) == ft_strlen(allowed_options[i].name))
        {
            len = ft_strlen(allowed_options[i].name);
            if (ft_strnstr(allowed_options[i].name, (char *)str + 2, len))
                return (&allowed_options[i]);
        }
    }
    return (NULL);
}

static t_option *check_option(char **av, int i)
{
    t_option *res = NULL;
    if (ft_strlen(av[i]) == 2 && (res = is_allowed_option(av[i][1])) != NULL)
        return (res);
    else if (ft_strlen(av[i]) > 2 && (res = is_allowed_option_long(av[i])) != NULL)
        return (res);
    exit_error_options("nm: error in option\n");
    return (NULL);
}

t_parsed_cmd   parse_options(int ac, char **av)
{
    t_parsed_cmd    result;
    t_lst           *act_options = NULL;
    t_lst           *not_options = NULL;
    t_option        *res = NULL;

    for (int i = 1; i < ac; i++)
    {
        if (ft_strlen(av[i]) && av[i][0] == '-')
        {
            res = check_option(av, i);
            if (res->need_param)
            {
                if (++i == ac)
                    exit_error_options("nm: option requires an argument\nTry 'nm --help' for more information.\n");
                res->param = ft_strdup(av[i]);
                if (res->param == NULL)
                    exit_error_options("nm: malloc failure.\n");
            }
            ft_lst_add_node_back(&act_options, ft_lst_create_node(res));
        }
        else
            ft_lst_add_node_back(&not_options, ft_lst_create_node(av[i]));
    }
    result.act_options = act_options;
    result.not_options = not_options;
    return (result);
}
