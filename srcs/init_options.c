#include "nm_functions.h"

int    init_options_sort(t_lst *act_options)
{
    int sort;

    sort = 0;
    if (is_activated_option(act_options, 'r'))
        sort = S_REVERSE;
    else if (is_activated_option(act_options, 'p'))
        sort = S_NO_SORT;
    else
        sort = S_ALPHA;
    return (sort);
}

int    init_options_filter(t_lst *act_options)
{
    int filter;

    filter = 0;
    if (is_activated_option(act_options, 'a'))
        filter = F_ALL;
    else if (is_activated_option(act_options, 'g'))
        filter = F_EXTERN_ONLY;
    else if (is_activated_option(act_options, 'u'))
        filter = F_UNDEFINED_ONLY;
    else
        filter = F_NO_DEBUG;
    return (filter);
}
