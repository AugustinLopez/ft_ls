# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aulopez <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/12 11:06:27 by aulopez           #+#    #+#              #
#    Updated: 2019/02/12 15:12:33 by aulopez          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=ft_ls

# --- 1.Source/Header ----------------------------------------------------------

SRC=main ls_parsing ls_error
HDR=libft.h ft_ls.h

# --- 2.Path -------------------------------------------------------------------

PATH_HDR=includes/
PATH_SRC=srcs/
PATH_OBJ=srcs/obj/
PATH_LIB=srcs/libft/

# --- 3.File Variables ---------------------------------------------------------

OBJ_LS=$(SRC:%=$(PATH_OBJ)%.o)
INCLUDES=$(HDR:%=$(PATH_HDR)%)

# --- 4.Compilation Variables --------------------------------------------------

CC=gcc
FLAGS=-Wall -Wextra -Werror -g3
OPTION_O=$(CC) $(FLAGS) -c -I$(PATH_HDR)
OPTION_C=$(CC) $(FLAGS) -o $(NAME) -I$(PATH_HDR) -L$(PATH_LIB) -lft
RED=\033[31m
GREEN=\033[32m
YELLOW=\033[33m
BLUE=\033[34m
PURPLE=\033[35m
CYAN=\033[36m
EOC=\033[0m

# --- 5.Rules ------------------------------------------------------------------

all: $(NAME)

$(NAME): $(OBJ_LS)
	@echo "$(YELLOW)Updating library... $(EOC)"
	@make -C $(PATH_LIB) >> /dev/null
	@echo "$(YELLOW)Compiling $(NAME)...$(EOC)"
	@$(OPTION_C) $(OBJ_LS)
	@echo "$(GREEN)Done$(EOC)"

$(PATH_OBJ)%.o:$(PATH_SRC)%.c $(INCLUDES)
	@echo "$(CYAN)Creating $@$(EOC)"
	@$(OPTION_O) $< -o $@

fcleanlib:
	@echo "$(PURPLE)Removing library... $(EOC)"
	@make -C $(PATH_LIB) fclean > /dev/null

cleanlib:
	@echo "$(BLUE)Cleaning library... $(EOC)"
	@make -C $(PATH_LIB) clean > /dev/null

clean: cleanlib
	@echo "$(BLUE)Deleting object files...$(EOC)"
	@rm -f $(OBJ_LS)

fclean: clean fcleanlib
	@echo "$(PURPLE)Deleting $(NAME)...$(EOC)"
	@rm -f $(NAME)
	@echo "$(GREEN)Done$(EOC)"

re: fclean all

.PHONY: all clean fclean re fcleanlib cleanlib
