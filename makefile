all: build run # Detta target medför både kompilering och körning direkt (behöver bara skriva make).

# Target för att kompilera koden. Tab måste användas, ej blanksteg i makefil.
build:
		@g++	*.cpp -o main -Wall -l gpiod
run:
		@./main
