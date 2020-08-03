NAME			=	wolf3d

# FT_DIR			=	./libft
# JTOC_DIR		=	./libjtoc

# INC_FT			=	$(FT_DIR)/include
# INC_JTOC		=	$(JTOC_DIR)/include
INC_DIR			=	./include

SRC_DIR			=	./src
OBJ_DIR			=	./obj

SRC				=	main.c \
					keys.c \
					sdl.c \
					raycast.c \
					parser.c



OBJ				=	$(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

INCS			=	-I$(INC_DIR) \
					-I./frameworks/SDL2.framework/Versions/A/Headers \
					-I./frameworks/SDL2_image.framework/Versions/A/Headers \
					-I./frameworks/SDL2_ttf.framework/Versions/A/Headers \
					-I./frameworks/SDL2_mixer.framework/Versions/A/Headers \

FRAMEWORKS		=	-F./frameworks \
					-rpath ./frameworks \
					-framework SDL2 -framework SDL2_ttf -framework SDL2_image -framework SDL2_mixer

CC				=	gcc

CFLAGS			=	-Wall -Wextra -Werror -O3

TEMP		:=	ajbfjkbjafas1243.txt
CAT_TEMP	=	$(shell cat $(TEMP))

all: $(NAME)

$(NAME):
	@make $(OBJ_DIR)
	@make compile
	@rm -f $(TEMP)

compile: $(OBJ)
		make build;

build:
	$(CC) $(CFLAGS) $(OBJ) $(INCS) $(LIBS) $(FRAMEWORKS) -o $(NAME)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCS) -o $@ -c $<
	@echo "1" > $(TEMP)

$(OBJ_DIR)/%.o: $(SRC_DIR)/rc_jtoc/%.c
	$(CC) $(CFLAGS) $(INCS) -o $@ -c $<
	@echo "1" > $(TEMP)

clean:
	@rm -f $(TEMP)
	rm -rf $(OBJ_DIR)

fclean:
	@rm -f $(TEMP)
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)

re: fclean all

norm:
	@norminette $(SRC_DIR)
	@norminette $(INC_DIR)

.PHONY: all $(NAME) clean fclean re norm