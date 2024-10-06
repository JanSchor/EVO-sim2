mainFileName = main
srcFiles = src/main.c src/genome.c src/creature.c src/neuron.c src/grid.c

all: main
main:
	@echo Compiling into $(mainFileName).exe
	@gcc $(srcFiles) -o $(mainFileName)