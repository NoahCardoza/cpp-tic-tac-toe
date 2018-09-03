NAME 	= TicTacToe
RM 		= /bin/rm

FLAGS 	= -Wall -Wextra -Werror -Wno-c++11-extensions

BUILD_DIR := ./build/
SRC_DIR := ./src/
LIB_DIR := ./lib/
OBJ_DIR := ./objs/

# HEADERS = ./src/*.h

FILES = main
CFILES = $(patsubst %, $(SRC_DIR)%.cpp, $(FILES))
OFILES = $(patsubst %, $(OBJ_DIR)%.o, $(FILES))

.PHONY: all clean fclean re

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo [INFO] $(NAME) Object Files Directory Created

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	g++ $(FLAGS) -o $@ -c $< 

$(NAME): $(OBJ_DIR) $(OFILES)
	@mkdir -p $(BUILD_DIR)
	@g++ $(FLAGS) $(OFILES) -o $(BUILD_DIR)$(NAME)
	@echo [INFO] $(NAME) Binary Created

clean:
	@$(RM) -rf $(OBJ_DIR)
	@echo [INFO] $(NAME) Object Files Directory Destroyed

fclean: clean
	@$(RM) -rf $(BUILD_DIR)
	@echo [INFO] $(NAME) Binary Destroyed

re: fclean all

