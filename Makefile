##
## EPITECH PROJECT, 2025
## my_robot_factory
## File description:
## description
##
CC := g++
TEMP = $(shell find . -name "*.o" -type f;\
	find . -name "*~" -type f; find . -name "*#" -type f)
SRC := ./src/*.cpp
NAME := treasure_scan
FLAGS = -lsfml-window -lsfml-graphics -lsfml-system -g3
all:
	${CC} ${SRC} -o ${NAME} ${FLAGS}
clean:
	rm -f ${TEMP}
	rm *.i -f
	rm *.bc -f
fclean: clean
	rm -f ${NAME}
re: fclean all
