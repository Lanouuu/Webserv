NAME			=	webserv

CFLAGS			=	-Wall -Wextra -Werror -I ${INC_PATH} -g3 -std=c++98

CC				=	c++

GREEN			=	\033[0;32m

PURPLE			=	\033[10;95m

YELLOW			=	\033[0;33m

END				=	\033[0m

# **************************************************************************** #
#                                                                              #
#                                  SOURCES                                     #
#                                                                              #
# **************************************************************************** #

SOURCES_PATH	=	./srcs/

SOURCES_MAIN	= 	main.cpp \
						
						
SOURCES_PARSING	=	parsing/parsing_conf/parsing_conf.cpp \
						parsing/parsing_conf/Lexer.cpp \
						parsing/parsing_conf/Parser.cpp \
						parsing_request/request.cpp \

SOURCES_SERVER	=	server/launch_servers.cpp \
						server/Server.cpp \
						server/Location.cpp \

SOURCES_CLIENT	=	client/Client.cpp \
	
# **************************************************************************** #
#                                                                              #
#                                  OBJECTS                                     #
#                                                                              #
# **************************************************************************** #

OBJECTS_PATH	=	objs/

OBJECTS			=	$(addprefix ${OBJECTS_PATH}, ${SOURCES_MAIN:.cpp=.o}) \
					$(addprefix ${OBJECTS_PATH}, ${SOURCES_PARSING:.cpp=.o}) \
					$(addprefix ${OBJECTS_PATH}, ${SOURCES_SERVER:.cpp=.o}) \
					$(addprefix ${OBJECTS_PATH}, ${SOURCES_CLIENT:.cpp=.o}) \

# **************************************************************************** #
#                                                                              #
#                                  INCLUDES                                    #
#                                                                              #
# **************************************************************************** #

INC_PATH		=	./include/

INCLUDES		=	${INC_PATH}Server.hpp \
					${INC_PATH}Client.hpp \
					${INC_PATH}Location.hpp \
					${INC_PATH}Lexer.hpp \
					${INC_PATH}Parser.hpp \
					${INC_PATH}server.h \
					${INC_PATH}request.hpp \

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
	@echo "\ \      / /__| |__  ___  ___ _ ____   __ "
	@echo " \ \ /\ / / _ \  _ \/ __|/ _ \ '__\ \ / / " 
	@echo "  \ V  V /  __/ |_) \__ \  __/ |   \ V /  " 
	@echo "   \_/\_/ \___|_.__/|___/\___|_|    \_/   "
	@echo "\n"
	@echo "© 2025 Webserv, Inc. aciezadl - jaung - ade-fran"
	@echo "\n"
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
