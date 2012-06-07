CC=g++
FLAGS=-Wall -Werror
LIBS=-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
NAME=Gravastre

all:
	$(CC) $(FLAGS) -c *.cpp Interface/*.cpp
	$(CC) -o $(NAME) *.o $(LIBS)
	rm *.o
	mv $(NAME) ../
