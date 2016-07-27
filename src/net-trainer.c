#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/neuron.h"
#include "../include/neurogen.h"

#define POP_SIZE 100
#define CROSS_RATE 0.60
#define MUTATION_RATE 0.01
#define GEN_MAX 100
#define LAYER_NUM 4
#define LAYER_SIZE 4

double get_fitness (neuron_network_t* network, neurogen_genome_t* genome, neuron_dataset_t* set);

int main (int argc, char* argv[])
{
	
	neuron_dataset_t set = neuron_dataset_construct(); // XOR Gate
	neuron_dataset_push(&set, 2, (double[]) {0.0, 0.0}, 1, (double[]) {0.0});
	neuron_dataset_push(&set, 2, (double[]) {1.0, 0.0}, 1, (double[]) {1.0});
	neuron_dataset_push(&set, 2, (double[]) {0.0, 1.0}, 1, (double[]) {1.0});
	neuron_dataset_push(&set, 2, (double[]) {1.0, 1.0}, 1, (double[]) {0.0});	
	int net_inputs = set.data[1].input.length;
	int net_outputs = set.data[1].expected.length;
	neuron_network_t network = neuron_network_construct(net_inputs, net_outputs , LAYER_SIZE, LAYER_NUM, neuron_relu);
	int chrom_length = neuron_network_get_num_weights(&network);

	neurogen_population_t pop = neurogen_population_construct(POP_SIZE, chrom_length, CROSS_RATE, MUTATION_RATE, neurogen_errorfunction_simple);
	
	puts("Starting neural network training...");
	printf("Population size: %d \tGenome size: %d.\n", POP_SIZE, chrom_length);
	printf("Inputs: %d \t Outputs: %d.\n", net_inputs, net_outputs);
	printf("Hidden layers: %d \t Layer size: %d\n", LAYER_NUM, LAYER_SIZE);
	puts("");


	for (int generation = 0; generation < GEN_MAX; generation++)
	{		
		neurogen_population_update(&network, &pop, &set);
		printf("Gen: %d \tBest Fitness %f \tAvg Fitness %f Total Fitness: %f\n", pop.generation, pop.best_fitness, pop.average_fitness, pop.total_fitness);

	}

	return EXIT_SUCCESS;
}


