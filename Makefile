# =========================
# Project name
# =========================

NAME_BASE    = libft_malloc
LIB_NAME     = $(NAME_BASE)_$(HOSTTYPE).so
SYMLINK_NAME = $(NAME_BASE).so

# =========================
# Compiler
# =========================

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -fPIC -fvisibility=hidden
LDFLAGS = -shared

# =========================
# Directories
# =========================

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes

# =========================
# Sources
# =========================

SRCS = \
	$(SRC_DIR)/malloc.c \
	$(SRC_DIR)/free.c \
	$(SRC_DIR)/realloc.c \
	$(SRC_DIR)/zone.c \
	$(SRC_DIR)/block.c \
	$(SRC_DIR)/show_alloc_mem.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# =========================
# HOSTTYPE
# =========================

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# =========================
# Rules
# =========================

all: $(LIB_NAME) symlink

$(LIB_NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

symlink:
	ln -sf $(LIB_NAME) $(SYMLINK_NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(LIB_NAME) $(SYMLINK_NAME)

re: fclean all

.PHONY: all clean fclean re symlink
