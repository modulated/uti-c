#ifndef _NEUROGEN_H
#define _NEUROGEN_H

#include "neuron.h"

typedef struct
{
	neuron_array_t chromosome;
	double fitness;
} neurogen_genome_t;

typedef struct
{
	neurogen_genome_t* genomes;
	int population_size;
	int genome_length;
	double total_fitness;
	double best_fitness;
	double average_fitness;
	double worst_fitness;
	neurogen_genome_t* fittest_genome;
	double mutation_rate;
	double crossover_rate;
	int children_per_parent;
	int generation;
	double (*error_function)(const neuron_array_t*, const neuron_array_t*);
} neurogen_population_t;

neurogen_genome_t neurogen_genome_construct (const neuron_array_t* source);
void neurogen_genome_set_fitness (neurogen_genome_t* genome, double fitness);
double neurogen_genome_calculate_fitness (neuron_network_t* network, neurogen_genome_t* genome, neuron_dataset_t* set);
neurogen_genome_t* neurogen_genome_compare (neurogen_genome_t* genome_a, neurogen_genome_t* genome_b);
void neurogen_genome_destruct (neurogen_genome_t* genome);

void neurogen_genome_crossover_index (neurogen_genome_t* parent_a, neurogen_genome_t* parent_b, neurogen_genome_t* child_a, neurogen_genome_t* child_b, unsigned int index);
void neurogen_genome_crossover (neurogen_genome_t* parent_a, neurogen_genome_t* parent_b, neurogen_genome_t* child_a, neurogen_genome_t* child_b, double probability);
void neurogen_genome_mutate (neurogen_genome_t* array, double probability);

neurogen_population_t neurogen_population_construct (int population_size, int genome_length, double mutation_rate, double crossover_rate, double (*error_function)(const neuron_array_t*, const neuron_array_t*));
void neurogen_population_destruct (neurogen_population_t* population);
void neurogen_population_calculate_statistics (neurogen_population_t* population);
neurogen_population_t neurogen_population_update(neuron_network_t* network, neurogen_population_t* population, neuron_dataset_t* set);
neurogen_population_t neurogen_population_evolve (neurogen_population_t* population);
void neurogen_genome_run_network (neuron_network_t* network, neurogen_population_t* population, neuron_dataset_t* set);
void neurogen_population_get_fitness (neuron_network_t* network, neurogen_genome_t* population, neuron_dataset_t* set);
neurogen_genome_t* neurogen_population_roulette_selection(neurogen_population_t* population);
double neurogen_errorfunction_simple(const neuron_array_t* output, const neuron_array_t* expected);


#endif
