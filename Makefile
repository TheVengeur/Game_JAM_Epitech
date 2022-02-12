##
## EPITECH PROJECT, 2021
## qPUG
## File description:
## qPUG
##

SRC		=	$(wildcard src/*.cpp)
OBJ		=	$(SRC:.cpp=.o)

INCLUDE		=	-I ./include
CPPFLAGS	=	-W -Wall -Wextra $(INCLUDE) -std=c++17 -lsfml-graphics -lsfml-system -lsfml-window

CC		=	g++
NAME		=	qPUG
DNAME		=	-o $(NAME)

COM_COLOR	=	\033[0;94m
REM_COLOR	=	\033[0;93m
OBJ_COLOR	=	\033[0;96m
OK_COLOR	=	\033[0;1;92m
ERROR_COLOR	=	\033[0;1;91m
WARN_COLOR	=	\033[0;1;95m
NO_COLOR	=	\033[0m

OK_STRING	=	"[OK]"
ERROR_STRING	=	"[ERROR]"
WARN_STRING	=	"[WARNING]"
COM_STRING	=	"Compiling"
REM_STRING	=	"Removing"

RM		=	rm -f

%.o: %.cpp
	@printf "%-60b" "$(COM_COLOR)$(COM_STRING) $(OBJ_COLOR)$@$(NO_COLOR)";
	@$(CC) -c $(CPPFLAGS) -o $@ $< 2> $@.log;					\
		RESULT=$$?;								\
		if [ $$RESULT -ne 0 ]; then						\
			printf "%b" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n";	\
		elif [ -s $@.log ]; then						\
			printf "%b" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n";		\
		else									\
			printf "%b" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n";		\
		fi;									\
		cat $@.log;								\
		$(RM) $@.log;								\
		exit $$RESULT

all:		$(NAME)

$(NAME):	$(OBJ)
	@printf "\n%-60b" "$(COM_COLOR)$(COM_STRING) $(OBJ_COLOR)$(@)$(NO_COLOR)";
	@$(CC) $(DNAME) $(OBJ) $(CPPFLAGS) 2> $(NAME).log;				\
		RESULT=$$?;								\
		if [ $$RESULT -ne 0 ]; then						\
			printf "%b" "$(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR)\n";	\
		elif [ -s $(NAME).log ]; then						\
			printf "%b" "$(WARN_COLOR)$(WARN_STRING)$(NO_COLOR)\n";		\
		else									\
			printf "%b" "$(OK_COLOR)$(OK_STRING)$(NO_COLOR)\n";		\
		fi;									\
		cat $(NAME).log;							\
		$(RM) $(NAME).log;							\
		exit $$RESULT

clean:
	@printf "%b" "$(REM_COLOR)$(REM_STRING)  $(OBJ_COLOR)$(OBJ)$(NO_COLOR)\n";
	@$(RM) $(OBJ)

fclean:		clean
	@printf "%b" "$(REM_COLOR)$(REM_STRING)  $(OBJ_COLOR)$(NAME)$(NO_COLOR)\n";
	@$(RM) $(NAME)

re:		fclean	\
		all

#############
#╭─────────╮#
#│--debug--│#
#╰─────────╯#
#############
debug:	CPPFLAGS += -g
debug:		fclean	\
		$(NAME)

.PHONY: all clean fclean re debug $(NAME)
