#include "nm_functions.h"

void   option_r(t_data *dt)
{
    if (is_activated_option(dt->act_options, 'r'))
        dt->sort = S_REVERSE;
    else
        dt->sort = S_ALPHA;
}

void   option_p(t_data *dt)
{
    if (is_activated_option(dt->act_options, 'p'))
        dt->sort = S_NO_SORT;
    else
        dt->sort = S_ALPHA;
}

void   option_a(t_data *dt)
{
    if (is_activated_option(dt->act_options, 'a'))
        dt->filter = F_ALL;
    else
        dt->filter = F_NO_DEBUG;
}

void   option_g(t_data *dt)
{
    if (is_activated_option(dt->act_options, 'g'))
        dt->filter = F_EXTERN_ONLY;
    else
        dt->filter = F_NO_DEBUG;
}

void   option_u(t_data *dt)
{
    if (is_activated_option(dt->act_options, 'u'))
        dt->filter = F_UNDEFINED_ONLY;
    else
        dt->filter = F_NO_DEBUG;
}

void    init_options_params(t_data *dt)
{
    option_a(dt);
    option_g(dt);
    option_r(dt);
    option_u(dt);
    option_p(dt);
}
