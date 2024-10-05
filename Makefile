
all: main

main:
	gcc src/main.c src/genome.c src/creature.c src/neuron.c -o main

