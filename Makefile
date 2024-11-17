mainFileName = main
srcFiles = src/main.c src/genome.c src/creature.c src/neuron.c src/grid.c src/globals.c src/alive_zone.c src/wall.c src/scenario.c src/help_lib.c src/graph.c
flags = -std=c11 -Wall -Wextra -Werror

all: main
main:
	@echo Compiling into $(mainFileName).exe
	@gcc $(srcFiles) -o $(mainFileName)