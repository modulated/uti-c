#ifndef _NEUROGEN_H
#define _NEUROGEN_H

#include "neuron.h" // size_t, neuron_array_t

// ===========================
// Public Structs
// ===========================

typedef struct
{
	neuron_array_t array;
	double fitness;
} neurogen_genome_t;

typedef struct
{
	neurogen_genome_t* genome_array;
	size_t population_size;
	size_t genome_length;
	double total_fitness;
	double best_fitness;
	double average_fitness;
	double worst_fitness;
	neurogen_genome_t* fittest_genome;
	double mutation_rate;
	double crossover_rate;	
	size_t generation;
	double (*error_function)(const neuron_array_t*, const neuron_array_t*);
} neurogen_population_t;

// ===========================
// Genome Functions
// ===========================

// Creates new genome from neuron_array. Fitness by default 0.
neurogen_genome_t neurogen_genome_construct (const neuron_array_t* array);

// Sets fitness value of genome.
void neurogen_genome_set_fitness (neurogen_genome_t* genome, double fitness);

// Compares two genomes fitness, return the greater genome.
const neurogen_genome_t* neurogen_genome_compare_fitness (const neurogen_genome_t* genome_a, const neurogen_genome_t* genome_b);

// Cleans up genome struct
void neurogen_genome_destruct (neurogen_genome_t* genome);

// Returns a duplicate population struct to *dest* pointer.
void neurogen_genome_duplicate (neurogen_genome_t* dest, const neurogen_genome_t* source);

// Crosses over two parent genomes at given *index*. Sets child pointers to new genomes. 
void neurogen_genome_crossover_index (const neurogen_genome_t* parent_a, const neurogen_genome_t* parent_b, neurogen_genome_t* child_a, neurogen_genome_t* child_b, size_t index);

// Crosses over two parent genomes with random *probability*. Sets child pointers to new genomes. 
void neurogen_genome_crossover_random (neurogen_genome_t* parent_a, neurogen_genome_t* parent_b, neurogen_genome_t* child_a, neurogen_genome_t* child_b, double probability);

// A *probability* chance of mutating a genome member. 
void neurogen_genome_mutate (neurogen_genome_t* genome, double probability);


// ===========================
// Population Functions
// ===========================

// Create new population of *population_size* genomes of *genome_length* with *mutation_rate*, *crossover_rate* and *error_function*. 
neurogen_population_t neurogen_population_construct (size_t population_size, size_t genome_length, double mutation_rate, double crossover_rate, double (*error_function)(const neuron_array_t*, const neuron_array_t*));

// Clean up population struct.
void neurogen_population_destruct (neurogen_population_t* population);

// Returns a duplicate population struct. 
neurogen_population_t neurogen_population_duplicate (const neurogen_population_t* population);

// Calculates and sets the statistics of population based on genome fitnesses.
void neurogen_population_calculate_statistics (neurogen_population_t* population);

// Runs the network, calculates the fitness and evolves the population.
neurogen_population_t neurogen_population_update(neuron_network_t* network, neurogen_population_t* population, const neuron_dataset_t* set);

// Runs the *network* with *set* as input and output on the *population* genomes, setting fitness of genomes in the population.
void neurogen_population_run_network (neuron_network_t* network, neurogen_population_t* population, const neuron_dataset_t* set);

// Returns a genome from *population* based on fitness.
neurogen_genome_t* neurogen_population_roulette_selection(const neurogen_population_t* population);

// Returns the absolute difference of *output* and *expected* arrays.
double neurogen_errorfunction_simple(const neuron_array_t* output, const neuron_array_t* expected);

// Evolves and returns a new population by roullete selecting two parents, crossing over then mutating each genome from the *population*. 
neurogen_population_t neurogen_population_evolve (const neurogen_population_t* population);

#endif
