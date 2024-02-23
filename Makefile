# clang -fsanitize=address -g3 srcs/*.c -Wall -Wextra -Werror -I includes/ -I libft/includes/ -I libft -L libft -lft && ./ft_nm a.out

# *** NAME - CC - FLAGS ****************************************************** #

NAME					= 	ft_nm

CC						= 	gcc -g3

FFLAGS					=   -fsanitize=address

LFLAGS					=   -fsanitize=leak

CFLAGS					= 	-I includes/ \
							-I libft/includes/ \
							-Wall -Wextra -Werror 

INC_LIB					= 	-I libft
LIB						= 	-L libft -lft

NAME_TESTER				=	tester


# *** PROJECT HEADER ********************************************************* #

HDIR					=	$(addprefix -I, $(HEADER_DIR))

HEAD					=	$(addprefix $(HEADER_DIR), $(HEADER))

HEADER_DIR				=	\
							./includes/

HEADER					=	\
							nm_data.h \
							nm_functions.h \
							utils_colors.h \

# *** SRCS ******************************************************************* #

SRCS_DIR					=	./srcs/

SRCS_LIST					=	\
							check.c \
							debug.c \
							exit.c \
							nm.c \
							syms_0_store.c \
							syms_0_type.c \
							syms_1_sort_general.c \
							syms_1_sort_letter.c \
							syms_1_sort_sym.c \
							syms_2_display.c \
							init_options.c \
							utils_options.c \

SRCS					=	$(addprefix $(SRCS_DIR), $(SRCS_LIST))

# *** OBJS ******************************************************************* #

OBJS_DIR				=	./objs/

OBJS_LIST				=	$(patsubst %.c, %.o, $(SRCS_LIST))

OBJS					=	$(addprefix $(OBJS_DIR), $(OBJS_LIST))

# *** TESTER ******************************************************************* #
# gcc -Wall -Wextra -Werror srcs/simple_tester.c -I ./libft/includes -I ./includes -L ./libft/ -lft -o tester && ./tester


all: $(OBJS) $(SRCS) $(NAME)

$(NAME): $(OBJS) $(SRCS)
	@ make -C libft/
	@ $(CC) $(CFLAGS) $(HDIR) $(OBJS) $(INC_LIB) $(LIB) -o $@

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $(HDIR) -D BONUS=$(BONUS_FLAG) $(INC_LIB) -c -o $@ $<

fsan:  $(OBJS) $(SRCS)
	@ make -C libft/
	@ $(CC) $(CFLAGS) $(FFLAGS) $(HDIR) $(OBJS) $(INC_LIB) $(LIB) -o $(NAME)

leak:  $(OBJS) $(SRCS)
	@ make -C libft/
	@ $(CC) $(CFLAGS) $(LFLAGS) $(HDIR) $(OBJS) $(INC_LIB) $(LIB) -o $(NAME)

tester:
	@ make -C libft/
	@ $(CC) $(CFLAGS) srcs/simple_tester.c $(HDIR) $(INC_LIB) $(LIB) -o $(NAME_TESTER)

tclean:
	@ rm -rf $(NAME_TESTER)

clean:
	@ make clean -C libft/
	@ rm -rf $(OBJS_DIR)

fclean:
	@ make fclean -C libft/
	@ rm -f $(NAME)
	@ rm -rf $(OBJS_DIR)

re: fclean all

test: tclean tester

rfsan: fclean fsan

.PHONY: all re clean fclean tester tclean test fsan rfsan leak
