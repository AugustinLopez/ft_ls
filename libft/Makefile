# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aulopez <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/06 13:11:21 by aulopez           #+#    #+#              #
#    Updated: 2019/01/24 13:00:24 by aulopez          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft.a

# --- 1.Source/Header ----------------------------------------------------------

___PUT=	putchar putchar_fd putendl putendl_fd putnbr putnbr_fd putstr putstr_fd
___MEM=	memcmp memdel bzero memset memcpy memccpy memmove memchr memalloc
__LIST=	lstnew lstdelone lstdel lstadd lstinsert lstappend lstfree lstsize
____TO=	atoi itoa atozu
STRCHR=	strchr strastr strastri strchri strlen wstrlen wcharlen strcmp strncmp strnchri
STRSET=	strcat strncat strclr strcpy strdel strdup strjoin strnew
_OTHER=	gnl

_LIBFT=	$(___PUT) $(___MEM) $(__LIST) $(____TO) $(STRCHR) $(STRSET) $(_OTHER)
HEADER=	libft.h

# ----- The following functions have yet to be used in production.
#R_LIST=	lstmap lstiter
#R_MATH=	pow sqrt rand
#R___IS=	isalnum isalpha isascii isdigit isprint
#R___TO=	tolower toupper
#RTRCHR=	strstr strnstr strrstr strrchr strcount strequ strnequ
#RTRSET=	strlcat strncpy striter striteri strmap strmapi strtrim strsplit

# --- 2.Path -------------------------------------------------------------------

PATH_HDR=./includes/
PATH_LIB=./srcs/
PATH_OBJ=./obj/

# --- 3.Object -----------------------------------------------------------------

OBJLIBFT=$(_LIBFT:%=$(PATH_OBJ)ft_%.o)

# --- 4.Variables --------------------------------------------------------------

FLAGS=
OPTION_O=$(FLAGS) -c -I$(PATH_HDR)
CC=gcc
RED=\033[31m
GREEN=\033[32m
YELLOW=\033[33m
BLUE=\033[34m
PURPLE=\033[35m
CYAN=\033[36m
EOC=\033[0m

# --- 5.Rules ------------------------------------------------------------------

all: $(NAME)

$(NAME): $(OBJLIBFT) $(PATH_HDR)$(HEADER)
	@echo "$(YELLOW)Compiling library...$(EOC)"
	@ar rcs $(NAME) $(OBJLIBFT)
	@echo "$(GREEN)Done$(EOC)"


$(PATH_OBJ)%.o:$(PATH_LIB)%.c
	@echo "$(CYAN)Creating $@$(EOC)"
	@$(CC) $(OPTION_O) $^ -o $@

clean:
	@echo "$(YELLOW)Deleting object files...$(EOC)"
	@rm -f $(OBJLIBFT) 
	@echo "$(GREEN)Done.$(EOC)"

fclean: clean
	@echo "$(YELLOW)Deleting library...$(EOC)"
	@rm -f $(NAME)
	@echo "$(GREEN)Done$(EOC)"

re: fclean all

.PHONY: all clean fclean re
