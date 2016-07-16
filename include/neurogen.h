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
	int generation;
} neurogen_population_t;

neurogen_genome_t neurogen_genome_construct (neuron_array_t* source);
void neurogen_genome_set_fitness (neurogen_genome_t* genome, double fitness);
neurogen_genome_t* neurogen_genome_compare (neurogen_genome_t* genome_a, neurogen_genome_t* genome_b);
void neurogen_genome_destruct (neurogen_genome_t* genome);

void neurogen_genome_crossover (neurogen_genome_t* parent_a, neurogen_genome_t* parent_b, neurogen_genome_t* child_a, neurogen_genome_t* child_b);
void neurogen_genome_mutate (neurogen_genome_t* array, double probability);

neurogen_population_t neurogen_population_construct (int population_size, int genome_length, double mutation_rate, double crossover_rate);
void neurogen_population_destruct (neurogen_population_t* population);
void neurogen_population_calculate_statistics (neurogen_population_t* population);
void neurogen_population_update(neurogen_population_t* population);
neurogen_genome_t* neurogen_population_roulette_selection(neurogen_population_t* population);


#endif
