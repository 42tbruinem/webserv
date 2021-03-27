# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: novan-ve <marvin@codam.nl>                   +#+                      #
#                                                    +#+                       #
#    Created: 2021/02/01 20:11:54 by novan-ve      #+#    #+#                  #
#    Updated: 2021/03/27 22:29:56 by tbruinem      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC_DIR = ./src/

HEADER =	Server.hpp \
			Client.hpp \
			configuration/Configuration.hpp \
			configuration/Context.hpp \
			configuration/Location.hpp \
			configuration/Parse.hpp \
			EnumString.hpp \
			Message.hpp \
			Method.hpp \
			Cgi.hpp \
			Properties.hpp \
			IOSet.hpp \
			Request.hpp \
			Response.hpp \
			URI.hpp \
			Utilities.hpp \
			WebServer.hpp

HEADER :=	$(addprefix ./incl/, $(HEADER))

INCL_FOLDERS = $(dir $(HEADER))
INCL_FOLDERS := $(sort $(INCL_FOLDERS))

INCL := $(addprefix -I ,$(INCL_FOLDERS))

SRC =	main.cpp \
		utilities.cpp \
		WebServer.cpp \
		configuration/Configuration.cpp \
		configuration/Parse.cpp \
		configuration/Location.cpp \
		configuration/Context.cpp \
		Properties.cpp \
		URI.cpp \
		Client.cpp \
		Server.cpp \
		Method.cpp \
		IOSet.cpp \
		Cgi.cpp \
		Request.cpp \
		Response.cpp \
		getLines.cpp \
		Message.cpp

OBJ := $(SRC:%.cpp=./obj/%.o)
SRC := $(SRC:%=$(SRC_DIR)%)

FLAGS = -Wall -Werror -Wextra -pedantic -std=c++98

ifeq ($(DEBUG),1)
	FLAGS += -g -fsanitize=address
else ifeq ($(DEBUG),2)
	FLAGS += -g
else
	FLAGS += -O3
endif

CC = clang++

all: $(NAME)
	@mkdir -p ./html/uploads

obj/%.o: src/%.cpp $(HEADER)
	mkdir -p $(@D)
	$(CC) $(FLAGS) $(INCL) -c $< -o $@

$(NAME): $(OBJ) $(HEADER)
	$(CC) $(FLAGS) $(INCL) $(OBJ) -o $(NAME)

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
