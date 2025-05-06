NAME			=	webserv

CFLAGS			=	-Wall -Wextra -Werror -I INCLUDES -std=c++98

CC				=	c++

GREEN			=	\033[0;32m

PURPLE 			=	\033[10;95m

YELLOW			=	\033[0;33m

END				=	\033[0m

# **************************************************************************** #
#                                                                              #
#                                  SOURCES                                     #
#                                                                              #
# **************************************************************************** #

SOURCES_PATH	=	./srcs/

SOURCES_MAIN	= 	main.cpp \
						Server.cpp \
						Location.cpp \
						conf_parsing.cpp
	
# **************************************************************************** #
#                                                                              #
#                                  OBJECTS                                     #
#                                                                              #
# **************************************************************************** #

OBJECTS_PATH	=	objs/

OBJECTS			=	$(addprefix ${OBJECTS_PATH}, ${SOURCES_MAIN:.cpp=.o}) \

# **************************************************************************** #
#                                                                              #
#                                  INCLUDES                                    #
#                                                                              #
# **************************************************************************** #

INCLUDES		=	srcs/Server.hpp \
					srcs/Location.hpp \
					srcs/parsing.hpp \

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
	@printf "$(PURPLE)"
	@echo "	\ \      / /__| |__  ___  ___ _ ____   __ "
	@echo "	 \ \ /\ / / _ \  _ \/ __|/ _ \ '__\ \ / / " 
	@echo "	  \ V  V /  __/ |_) \__ \  __/ |   \ V /  " 
	@echo "	   \_/\_/ \___|_.__/|___/\___|_|    \_/   "
	@printf "${COLOUR_END}"

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
