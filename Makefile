##
## Makefile for Malloc in /home/fillon_g/projects/tek2/unix/malloc
## 
## Made by guillaume fillon
## Login   <fillon_g@epitech.net>
## 
## Started on  Fri Jan 31 14:24:48 2014 guillaume fillon
## Last update Sun Feb 16 12:30:38 2014 luc sinet
##

NAME 	= libmy_malloc_$(HOSTTYPE).so

LINK	= libmy_malloc.so

CC      = gcc

SRCDIR 	= src/

SRC	= malloc.c 		\
	  free.c 		\
	  calloc.c 		\
	  realloc.c 		\
	  show_alloc_mem.c	\
	  thread.c 		\

RM      = rm -f

OBJDIR	= obj/

OBJ    = $(addprefix $(OBJDIR), $(SRC:.c=.o))

CFLAGS	+= -W -Wall -Wextra -Werror -fPIC
LDFLAGS	+= -shared -fPIC -lpthread

dummy := $(shell test -d $(OBJDIR) || mkdir $(OBJDIR))

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(LDFLAGS) $(OBJ)
	ln -sf $(NAME) $(LINK)

all: $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(LINK)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
