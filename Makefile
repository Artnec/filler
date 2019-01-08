FILLER_NAME		=	anesteru.filler

VIS_NAME		=	visualization

FILLER_CC		=	clang
CC				=	clang++

WFLAGS			=	-O3 -Wall -Werror -Wextra
CPP_FLAGS		=	-O3 -std=c++14
SFML_FLAGS		=	-framework sfml-graphics -framework sfml-window -framework sfml-system
SFML_DIR_FLAGS	=	-F ~/Library/Frameworks

FILLER_SRC		=	main.c		read_data.c		change_map_values.c

VIS_SRC			=	main.cpp	tilemap.cpp		read_data.cpp		manipulate_piece.cpp


FILLER_INC		=	filler.h

VIS_INC			=	visualization.hpp		tilemap.hpp

FILLER_OBJ		=	$(addprefix $(FILLER_OBJ_DIR), $(FILLER_SRC:.c=.o))
VIS_OBJ			=	$(addprefix $(VIS_OBJ_DIR), $(VIS_SRC:.cpp=.o))

FILLER_HEADER	=	$(addprefix $(FILLER_SRC_DIR), $(FILLER_INC))
VIS_HEADER		=	$(addprefix $(VIS_SRC_DIR), $(VIS_INC))

FILLER_SRC_DIR	=	filler_src/
FILLER_OBJ_DIR	=	filler_obj/

VIS_SRC_DIR		=	visualization_src/
VIS_OBJ_DIR		=	visualization_obj/


all: $(FILLER_NAME) $(VIS_NAME)

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



clean:
	rm -rf $(VIS_OBJ_DIR) $(FILLER_OBJ_DIR)

fclean: clean
	rm -f $(VIS_NAME) $(FILLER_NAME)

re: fclean all
