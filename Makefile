NAME			=	webserv

CFLAGS			=	-Wall -Wextra -Werror -std=c++98 -I ${INCLUDES}

CC				=	c++

GREEN			=	\033[0;32m

YELLOW			=	\033[0;33m

END				=	\033[0m

# **************************************************************************** #
#                                                                              #
#                                  SOURCES                                     #
#                                                                              #
# **************************************************************************** #

SOURCES_PATH	=	./srcs/

SOURCES_MAIN	= 	main.cpp \

SOURCES_PARSING_REQUEST	=	parsing_request/request.cpp \
	
# **************************************************************************** #
#                                                                              #
#                                  OBJECTS                                     #
#                                                                              #
# **************************************************************************** #

OBJECTS_PATH	=	objs/

OBJECTS			=	$(addprefix ${OBJECTS_PATH}, ${SOURCES_MAIN:.cpp=.o}) \
					$(addprefix ${OBJECTS_PATH}, ${SOURCES_PARSING_REQUEST:.cpp=.o}) \

# **************************************************************************** #
#                                                                              #
#                                  INCLUDES                                    #
#                                                                              #
# **************************************************************************** #

INCLUDES		= ${SOURCES_PATH}parsing_request/request.hpp

# **************************************************************************** #
#                                                                              #
#                                  RULES                                       #
#                                                                              #
# **************************************************************************** #

all:  ${NAME}

${NAME}: ${OBJECTS} ${INCLUDES}
	@${CC} ${CFLAGS} ${OBJECTS} -o ${NAME}
	@echo "${GREEN}\33[2K\nwebserv compiled with : \n${END}"
	@echo "	${CC} ${CFLAGS}\n"

${OBJECTS_PATH}%.o:	${SOURCES_PATH}%.cpp
	@mkdir -p ${dir $@}
	@${CC} ${CFLAGS} -c $< -o $@ && printf "\33[2K\r${YELLOW}Compiling webserv :${END} $@"

clean:
	@rm -rf ${OBJECTS_PATH}

fclean:
	@rm -rf ${OBJECTS_PATH}
	@rm -f ${NAME}
	@echo "${GREEN}\nwebserv cleaned\n${END}"

re: fclean all

.PHONY: fclean clean all re
