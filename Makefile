
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -fPIC -O0 -g -pthread
LDFLAGS = -shared -pthread


ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME    = libft_malloc_$(HOSTTYPE).so
SYMLINK = libft_malloc.so


SRC = \
	allocation.c \
	create_block.c \
	create_zone.c \
	free.c \
	main_func.c \
	print_allocated.c \
	print_utils.c \
	size_utils.c \
	zone_utils.c \
	realloc.c \
	debug.c \
	str_utils.c \
	malloc.c \
	block_util.c

OBJ = $(SRC:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)
	@rm -f $(SYMLINK)
	@ln -s $(NAME) $(SYMLINK)
	@echo "✔ libft_malloc built: $(NAME)"

%.o: %.c malloc.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@echo "✔ Objects removed"

fclean: clean
	@rm -f $(NAME) $(SYMLINK)
	@echo "✔ Library removed"

re: fclean all
