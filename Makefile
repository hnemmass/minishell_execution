CC = gcc -g #-fsanitize=address #-Wall -Wextra -Werror
RD = -lreadline

HED = inc/
SRC = src/main.c src/initialization/get_env.c utils/strutils1.c utils/free_minishell.c \
src/initialization/env_utils0.c src/tokenizer/ft_tokenizer.c src/parsing/parser.c \
utils/ft_split.c utils/strutils2.c src/expanding/expander.c src/parsing/handle_quotes.c \
src/parsing/syntax.c utils/strutils3.c utils/memory_utils0.c utils/parser_utils0.c src/builtin/exit.c \
src/builtin/cd.c src/builtin/echo.c src/builtin/env.c src/builtin/export.c src/builtin/pwd.c \
src/builtin/unset.c src/execution/gnl/get_next_line.c src/execution/gnl/get_next_line_utils.c \
src/execution/ft_execute_helper.c src/execution/ft_execute.c src/execution/redirections.c
OBJ = $(SRC:.c=.o)

NAME = minishell

all: $(NAME) clean

$(NAME): $(OBJ)
	$(CC) -I$(HED) $(OBJ) -o $(NAME) $(RD)

%.o: %.c $(HED)
	$(CC) -c $< -o $@ $(RD)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all re clean fclean