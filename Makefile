FILLER_NAME		=	anesteru.filler

VIS_NAME		=	visualization

TEST_NAME		=	test

FILLER_CC		=	clang
CC				=	clang++

WFLAGS			=	-Wall -Werror -Wextra -O3
CPP_FLAGS		=	-O3 -std=c++14
SFML_FLAGS		=	-framework sfml-graphics -framework sfml-window -framework sfml-system
SFML_DIR_FLAGS	=	-F ~/Library/Frameworks

FILLER_SRC		=	main.c		read_data.c		change_map_values.c

VIS_SRC			=	main.cpp	tilemap.cpp		read_data.cpp		manipulate_piece.cpp

TEST_SRC		=	test.c

FILLER_INC		=	filler.h

VIS_INC			=	visualization.hpp		tilemap.hpp

FILLER_OBJ		=	$(addprefix $(FILLER_OBJ_DIR), $(FILLER_SRC:.c=.o))
VIS_OBJ			=	$(addprefix $(VIS_OBJ_DIR), $(VIS_SRC:.cpp=.o))
TEST_OBJ		=	$(addprefix $(TEST_SRC_DIR), $(TEST_SRC:.c=.o))

FILLER_HEADER	=	$(addprefix $(FILLER_SRC_DIR), $(FILLER_INC))
VIS_HEADER		=	$(addprefix $(VIS_SRC_DIR), $(VIS_INC))

FILLER_SRC_DIR	=	filler_src/
FILLER_OBJ_DIR	=	filler_obj/

VIS_SRC_DIR		=	visualization_src/
VIS_OBJ_DIR		=	visualization_obj/

TEST_SRC_DIR	=	test_src/


all: $(FILLER_NAME) $(VIS_NAME) $(TEST_NAME)

filler_obj:
	mkdir -p $(FILLER_OBJ_DIR)

$(FILLER_OBJ_DIR)%.o: $(FILLER_SRC_DIR)%.c $(FILLER_HEADER) | filler_obj
	$(FILLER_CC) $(WFLAGS) -c $< -o $@

$(FILLER_NAME): $(FILLER_OBJ)
	$(FILLER_CC) $(WFLAGS) $(FILLER_OBJ) -o $(FILLER_NAME)



visualization_obj:
	mkdir -p $(VIS_OBJ_DIR)

$(VIS_OBJ_DIR)%.o: $(VIS_SRC_DIR)%.cpp $(VIS_HEADER) | visualization_obj
	$(CC) $(WFLAGS) $(CPP_FLAGS) $(SFML_DIR_FLAGS) -c $< -o $@

$(VIS_NAME): $(VIS_OBJ)
	$(CC) $(WFLAGS) $(CPP_FLAGS) $(SFML_FLAGS) $(SFML_DIR_FLAGS) $(VIS_OBJ) -o $(VIS_NAME)



$(TEST_SRC_DIR)%.o: $(TEST_SRC_DIR)%.c
	$(FILLER_CC) $(WFLAGS) -c $< -o $@

$(TEST_NAME): $(TEST_OBJ)
	$(FILLER_CC) $(WFLAGS) $(TEST_OBJ) -o $(TEST_NAME)


clean:
	rm -rf $(VIS_OBJ_DIR) $(FILLER_OBJ_DIR) $(TEST_OBJ)

fclean: clean
	rm -f $(VIS_NAME) $(FILLER_NAME) $(TEST_NAME)

re: fclean all
