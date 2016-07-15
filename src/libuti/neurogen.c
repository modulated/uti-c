#include <stdio.h>
#include <stdlib.h>
#include "../neurogen.h"
#include "../neuron.h"

/*
** Public Functions
*/

neurogen_genome_t neurogen_genome_construct (neuron_array_t* source)
{
	neurogen_genome_t genome;
	genome.chromosome = neuron_array_duplicate(source);
	genome.fitness = 0.0;

	return genome;
}

void neurogen_genome_set_fitness (neurogen_genome_t* genome, double fitness) {
	genome->fitness = fitness;
}

neurogen_genome_t* neurogen_genome_compare (neurogen_genome_t* genome_a, neurogen_genome_t* genome_b)
{
	return genome_a->fitness > genome_b->fitness ? genome_a : genome_b;
}

void neurogen_genome_destruct (neurogen_genome_t* genome)
{
	neuron_array_destruct(&genome->chromosome);
}


void neurogen_genome_crossover (neurogen_genome_t* parent_a, neurogen_genome_t* parent_b, 

neurogen_genome_t* child_a, neurogen_genome_t* child_b)
{
	if (parent_a->chromosome.length != parent_b->chromosome.length)
	{
		puts("ERROR: crossover lengths not equal.");
		exit(1);
	}
	
	int crossover_loc = ((parent_a->chromosome.length) / (RAND_MAX)) * (rand());

	neuron_array_t array_a = neuron_array_crossover(&parent_a->chromosome, &parent_b->chromosome, crossover_loc);
	*child_a = neurogen_genome_construct(&array_a);

	neuron_array_t array_b = neuron_array_crossover(&parent_b->chromosome, &parent_a->chromosome, crossover_loc);
	*child_b = neurogen_genome_construct(&array_b);

	neuron_array_destruct(&array_a);
	neuron_array_destruct(&array_b);
}

void neurogen_genome_mutate (neurogen_genome_t* genome, double probability)
{
	for (int i = 0; i < genome->chromosome.length; i++)
	{
		double random_prob = (double) rand()/RAND_MAX;
		if (random_prob <= probability)
		{
			double random_val = (double) rand()/RAND_MAX;
			neuron_array_set(&genome->chromosome, i, random_val);
		}
	}
}

neurogen_population_t neurogen_population_construct (int population_size, int genome_length, double mutation_rate, double crossover_rate)
{
	neurogen_population_t population;
	population.population_size = population_size;
	population.genome_length = genome_length;
	population.mutation_rate = mutation_rate;
	population.crossover_rate = crossover_rate;
	population.average_fitness = 0;
	population.worst_fitness = 0;
	population.best_fitness = 0;
	population.total_fitness = 0;
	population.generation = 0;
	population.fittest_genome = NULL;
	population.genomes = malloc(population_size * sizeof(neurogen_genome_t));
	
	neuron_array_t array = neuron_array_construct(genome_length);
	for (int i = 0; i < population_size; i++)
	{
		population.genomes[i] = neurogen_genome_construct(&array);
	}

	neuron_array_destruct(&array);
	return population;
}
void neurogen_population_destruct (neurogen_population_t* population)
{
	for (int i = 0; i < population->population_size; i++)
	{
		neurogen_genome_destruct(&population->genomes[i]);
	}

	free(population->genomes);
	population->genomes = NULL;
	population->genome_length = 0;
	population->population_size = 0;	
	population->mutation_rate = 0;
	population->crossover_rate = 0;
	population->average_fitness = 0;
	population->worst_fitness = 0;
	population->best_fitness = 0;
	population->total_fitness = 0;
	population->generation = 0;
	population->fittest_genome = NULL;
}

void neurogen_population_update(neurogen_population_t* population)
{
	for (int i = 0; i < population->population_size; i++)
	{
		// Run neural network
		// Calculate fitness / error
		// Mutation and Crossover
	}

	population->generation++;
}


void neurogen_population_calculate_statistics (neurogen_population_t* population)
{
	double total_fitness = 0.0;
	double current_best = 0.0;
	double current_worst = __DBL_MAX__;

	for (int i = 0; i < population->population_size; i++)
	{
		double val = population->genomes[i].fitness;
		total_fitness += val;
		
		if (val >= current_best)
		{
			current_best = val;
			population->fittest_genome = &population->genomes[i];
		}
		if (val <= current_worst) current_worst = val;
	}

	population->average_fitness = total_fitness / (double) population->population_size;
	population->total_fitness = total_fitness;
	population->best_fitness = current_best;
	population->worst_fitness = current_worst;
}
