NAME	=	webserv

INC_DIR	=	./incs
OBJ_DIR	=   ./objs
SRC_DIR = $(shell find srcs -type d)
vpath %.cpp $(foreach dir, $(SRC_DIR), $(dir):)
SRCS	=	main.cpp		\
			WebServ.cpp		\
			Config.cpp		\
			Server.cpp		\
			Location.cpp	\
			Client.cpp		\
			Utils.cpp

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:%.cpp=%.o))

CC		=	c++

FLAGS	=	-Wall -Werror -Wextra -std=c++98

.PHONY : all
all:	$(NAME)

$(NAME):	$(OBJS) $(INC_DIR)/*.hpp
			$(CC) $(FLAGS) -o $(NAME) $(OBJS) -I $(INC_DIR)

$(OBJ_DIR)/%.o : %.cpp $(INC_DIR)/*.hpp | $(OBJ_DIR)
			$(CC) $(FLAGS) -c $< -o $@ -I $(INC_DIR)

$(OBJ_DIR) :
	mkdir -p $@

.PHONY : clean
clean:
			rm -rf $(OBJ_DIR)

.PHONY : fclean
fclean:		clean
			rm -f $(NAME)

.PHONY : re
re:			fclean all