#include <stdio.h>
#include <stdlib.h>
#include "../include/random.h"
#include "../include/neurogen.h"
#include "../include/neuron.h"

/*
** Public Functions
*/

size_t debug_get_length(neurogen_population_t* pop)
{
	int length = 0;
	for (size_t i = 0; i < pop->population_size; i++)
	{
		length += pop->genomes[i].chromosome.length;
	}

	return length;
}

neurogen_genome_t neurogen_genome_construct (const neuron_array_t* source)
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

neurogen_genome_t neurogen_genome_duplicate (const neurogen_genome_t* genome)
{
	neurogen_genome_t output = neurogen_genome_construct(&genome->chromosome);
	output.fitness = genome->fitness;

	return output;
}

void neurogen_genome_crossover_index (neurogen_genome_t* parent_a, neurogen_genome_t* parent_b,
neurogen_genome_t* child_a, neurogen_genome_t* child_b, unsigned int index)
{
	if (parent_a->chromosome.length != parent_b->chromosome.length)
	{
		puts("ERROR: crossover lengths not equal.");
		neuron_array_print(&parent_a->chromosome);
		neuron_array_print(&parent_b->chromosome);
		exit(1);
	}

	neuron_array_t array_a = neuron_array_crossover(&parent_a->chromosome, &parent_b->chromosome, index);	
	*child_a = neurogen_genome_construct(&array_a);	

	neuron_array_t array_b = neuron_array_crossover(&parent_b->chromosome, &parent_a->chromosome, index);
	*child_b = neurogen_genome_construct(&array_b);
	

	neuron_array_destruct(&array_a);
	neuron_array_destruct(&array_b);
}

void neurogen_genome_crossover (neurogen_genome_t* parent_a, neurogen_genome_t* parent_b, neurogen_genome_t* child_a, neurogen_genome_t* child_b, double probability)
{
	double random_prob = prandom_double_range(0.0, 1.0);
	if (random_prob <= probability)
	{	
		int crossover_loc = prandom_int_range(0, parent_a->chromosome.length);
		neurogen_genome_crossover_index(parent_a, parent_b, child_a, child_b, crossover_loc);		
	}
	else
	{		
		child_a = parent_a;
		child_b = parent_b;
	}
}

void neurogen_genome_mutate (neurogen_genome_t* genome, double probability)
{	
	if (genome == NULL || genome->chromosome.array == NULL){
		puts("ERROR: pointer null.");
		exit(1);
	}

	for (int i = 0; i < genome->chromosome.length; i++)
	{
		double random_prob = prandom_double_range(0.0, 1.0);
		if (random_prob <= probability)
		{
			double random_val = prandom_double_range(0.0, 1.0);
			neuron_array_set(&genome->chromosome, i, random_val);
		}
	}
}

neurogen_population_t neurogen_population_construct (int population_size, int genome_length, double mutation_rate, double crossover_rate,  double (*error_function)(const neuron_array_t*, const neuron_array_t*))
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
	population.children_per_parent = 10;
	population.error_function = error_function;
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

neurogen_population_t neurogen_population_update(neuron_network_t* network, neurogen_population_t* population, neuron_dataset_t* set)
{
	// size_t old = debug_get_length(population);
	neurogen_population_run_network(network, population, set);
	neurogen_population_calculate_statistics(population);
	neurogen_population_t new_pop = neurogen_population_evolve(population);
	// printf("old: %lu new: %lu\n", old, debug_get_length(&new_pop));
	return new_pop;
}


void neurogen_population_run_network (neuron_network_t* network, neurogen_population_t* population, neuron_dataset_t* set){

	double fitness;

	for (size_t i = 0; i < population->population_size; i++)
	{
		fitness = 0.0;
		neuron_network_set_weights(network, &population->genomes[i].chromosome);

		for (int j = 0; j < set->length; j++)
		{		
			neuron_array_t output = neuron_network_update(network, &set->data[j].input);
			double current_fitness = population->error_function(&output, &set->data[j].expected);
			fitness += current_fitness;
		}

		#ifdef DEBUG
		printf("Genome: %lu \t Fitness: %f\n", i, fitness);
		#endif

		neurogen_genome_set_fitness(&population->genomes[i], fitness);
	}

}

neurogen_population_t neurogen_population_duplicate (neurogen_population_t* population)
{
	neurogen_population_t output = neurogen_population_construct(population->population_size, population->genome_length, population->mutation_rate, population->crossover_rate,  population->error_function);
	
	output.average_fitness = population->average_fitness;
	output.worst_fitness = population->worst_fitness;
	output.best_fitness = population->best_fitness;
	output.total_fitness = population->total_fitness;
	output.generation = population->generation;
	output.children_per_parent = 10;
	output.fittest_genome = population->fittest_genome;
	
	for (size_t i = 0; i < output.population_size; i++)
	{
		output.genomes[i] = neurogen_genome_duplicate(&population->genomes[i]);		
	}

	return output;
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
		
		if (val > current_best)
		{
			
			current_best = val;
			population->fittest_genome = &population->genomes[i];
		}
		if (val < current_worst) current_worst = val;
	}

	population->average_fitness = total_fitness / (double) population->population_size;
	population->total_fitness = total_fitness;
	population->best_fitness = current_best;
	population->worst_fitness = current_worst;
}

neurogen_genome_t* neurogen_population_roulette_selection(neurogen_population_t* population)
{
	double total = population->total_fitness;
	double random = prandom_double_range(0.0, total);

	double sum = 0;
	

	for (int i = 0; i < population->population_size; i++)
	{
		double current = population->genomes[i].fitness;

		// printf("Total: %f, sum: %f, current: %f, random: %f\n", total, sum, current, random);		
		
		if (current + sum >= random)
		{
			return &population->genomes[i];
		}
		else {
			sum += current;
		}
	}
	
	puts("ERROR: did not return genome");
	exit(1);
}

double neurogen_errorfunction_simple(const neuron_array_t* output, const neuron_array_t* expected)
{
	return neuron_array_difference(output, expected);
}

neurogen_population_t neurogen_population_evolve_old (neurogen_population_t* population)
{
	// size_t old_length = debug_get_length(population);
	neurogen_population_t new_pop = neurogen_population_duplicate(population);
	
	neurogen_population_calculate_statistics(population);

	for (int i = 0; i < population->population_size; i += population->children_per_parent)
	{
		neurogen_genome_t* parent_a = neurogen_population_roulette_selection(population); // randomly selects two parents 
		neurogen_genome_t* parent_b = neurogen_population_roulette_selection(population); // based on fitness weighting.

		neurogen_genome_t child_a = neurogen_genome_construct(&parent_a->chromosome);
		neurogen_genome_t child_b = neurogen_genome_construct(&parent_a->chromosome);

		for (int j = 0; j < population->children_per_parent; j += 2)
		{
			// puts("Crossover");
			neurogen_genome_crossover(parent_a, parent_b, &child_a, &child_b, population->crossover_rate);
			// puts("Mutate");
			neurogen_genome_mutate(&child_a, population->mutation_rate);
			neurogen_genome_mutate(&child_b, population->mutation_rate);
			// puts("Assigning");
			new_pop.genomes[i+j] = child_a;
			new_pop.genomes[i+j+1] = child_b;


		}

		neurogen_genome_destruct(parent_a);
		neurogen_genome_destruct(parent_b);
	}

	new_pop.generation++; // advance generation

	return new_pop;
}

neurogen_population_t neurogen_population_evolve (neurogen_population_t* pop)
{
	neurogen_population_t new_pop = neurogen_population_duplicate(pop);

	for (size_t i = 0; i < pop->population_size; i += 2)
	{
		neurogen_genome_t* parent_a = neurogen_population_roulette_selection(pop);
		neurogen_genome_t* parent_b = neurogen_population_roulette_selection(pop);
		neurogen_genome_t child_a;
		neurogen_genome_t child_b;

		neurogen_genome_crossover(parent_a, parent_b, &child_a, &child_b, pop->crossover_rate);
		
		neurogen_genome_mutate(&child_a, pop->mutation_rate);
		neurogen_genome_mutate(&child_b, pop->mutation_rate);

		new_pop.genomes[i] = child_a;
		new_pop.genomes[i+1] = child_b;
		neuron_array_print(&new_pop.genomes[i].chromosome);
		neuron_array_print(&new_pop.genomes[i+1].chromosome);
	}		

	return new_pop;
}
