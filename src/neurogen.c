#include <stdio.h>
#include <stdlib.h>
#include "../include/random.h"
#include "../include/neurogen.h"
#include "../include/neuron.h"

// ===========================
// Genome Functions
// ===========================

neurogen_genome_t neurogen_genome_construct (const neuron_array_t* array)
{
	neurogen_genome_t genome;
	genome.array = neuron_array_duplicate(array);
	genome.fitness = 0.0;

	return genome;
}

void neurogen_genome_set_fitness (neurogen_genome_t* genome, double fitness) {
	genome->fitness = fitness;
}

const neurogen_genome_t* neurogen_genome_compare_fitness (const neurogen_genome_t* genome_a, const neurogen_genome_t* genome_b)
{
	return genome_a->fitness > genome_b->fitness ? genome_a : genome_b;
}

void neurogen_genome_destruct (neurogen_genome_t* genome)
{
	neuron_array_destruct(&genome->array);
	genome->fitness = 0.0;
}

void neurogen_genome_duplicate (neurogen_genome_t* dest, const neurogen_genome_t* source)
{
	neurogen_genome_t output = neurogen_genome_construct(&source->array);
	output.fitness = source->fitness;
	*dest = output;
}

void neurogen_genome_crossover_index (const neurogen_genome_t* parent_a, const neurogen_genome_t* parent_b,
neurogen_genome_t* child_a, neurogen_genome_t* child_b, size_t index)
{
	if (parent_a->array.length != parent_b->array.length)
	{
		puts("ERROR: crossover lengths not equal.");
		neuron_array_print(&parent_a->array);
		neuron_array_print(&parent_b->array);
		exit(1);
	}

	neuron_array_t array_a = neuron_array_crossover(&parent_a->array, &parent_b->array, index);	
	*child_a = neurogen_genome_construct(&array_a);	

	neuron_array_t array_b = neuron_array_crossover(&parent_b->array, &parent_a->array, index);
	*child_b = neurogen_genome_construct(&array_b);
	

	neuron_array_destruct(&array_a);
	neuron_array_destruct(&array_b);
}

void neurogen_genome_crossover_random (neurogen_genome_t* parent_a, neurogen_genome_t* parent_b, neurogen_genome_t* child_a, neurogen_genome_t* child_b, double probability)
{
	if (!parent_a || !parent_b) {
		puts("ERROR: Parent NULL");
		exit(1);
	}
	double random_prob = prandom_double_range(0.0, 1.0);
	if (random_prob <= probability)
	{	
		int crossover_loc = prandom_int_range(0, parent_a->array.length);
		neurogen_genome_crossover_index(parent_a, parent_b, child_a, child_b, crossover_loc);		
	}
	else
	{		// no crossover = return parents
		neurogen_genome_duplicate(child_a, parent_a);
		neurogen_genome_duplicate(child_b, parent_b);
	}
}

void neurogen_genome_mutate (neurogen_genome_t* genome, double probability)
{	
	if (genome == NULL || genome->array.array == NULL){
		puts("ERROR: pointer null.");
		exit(1);
	}

	for (int i = 0; i < genome->array.length; i++)
	{
		double random_prob = prandom_double_range(0.0, 1.0);
		if (random_prob <= probability)
		{
			double random_val = prandom_double_range(0.0, 1.0);
			neuron_array_set(&genome->array, i, random_val);
		}
	}
}


// ===========================
// Genome Functions
// ===========================

neurogen_population_t neurogen_population_construct (size_t population_size, size_t genome_length, double mutation_rate, double crossover_rate,  double (*error_function)(const neuron_array_t*, const neuron_array_t*))
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
	population.error_function = error_function;
	population.fittest_genome = NULL;
	population.genome_array = malloc(population_size * sizeof(neurogen_genome_t));
	
	neuron_array_t array = neuron_array_construct(genome_length);
	for (int i = 0; i < population_size; i++)
	{
		population.genome_array[i] = neurogen_genome_construct(&array);
	}

	neuron_array_destruct(&array);
	return population;
}
void neurogen_population_destruct (neurogen_population_t* population)
{
	for (int i = 0; i < population->population_size; i++)
	{
		neurogen_genome_destruct(&population->genome_array[i]);
	}

	free(population->genome_array);
	population->genome_array = NULL;
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

neurogen_population_t neurogen_population_duplicate (const neurogen_population_t* population)
{
	neurogen_population_t output = neurogen_population_construct(population->population_size, population->genome_length, population->mutation_rate, population->crossover_rate,  population->error_function);
	
	output.average_fitness = population->average_fitness;
	output.worst_fitness = population->worst_fitness;
	output.best_fitness = population->best_fitness;
	output.total_fitness = population->total_fitness;
	output.generation = population->generation;
	output.fittest_genome = population->fittest_genome;
	
	for (size_t i = 0; i < output.population_size; i++)
	{
		 neurogen_genome_duplicate(&output.genome_array[i], &population->genome_array[i]);		
	}

	return output;
}

neurogen_population_t neurogen_population_update(neuron_network_t* network, neurogen_population_t* population, const neuron_dataset_t* set)
{	
	neurogen_population_run_network(network, population, set);
	neurogen_population_calculate_statistics(population);
	neurogen_population_t new_pop = neurogen_population_evolve(population);

	return new_pop;
}


void neurogen_population_run_network (neuron_network_t* network, neurogen_population_t* population, const neuron_dataset_t* set)
{
	double fitness;
	for (size_t i = 0; i < population->population_size; i++)
	{
		fitness = 0.0;
		neuron_network_set_weights(network, &population->genome_array[i].array);

		for (int j = 0; j < set->length; j++)
		{		
			neuron_array_t output = neuron_network_update(network, &set->data[j].input);
			double current_fitness = population->error_function(&output, &set->data[j].expected);
			fitness += current_fitness;
		}

		neurogen_genome_set_fitness(&population->genome_array[i], fitness);
	}

}

void neurogen_population_calculate_statistics (neurogen_population_t* population)
{
	double total_fitness = 0.0;
	double current_best = 0.0;
	double current_worst = __DBL_MAX__;

	for (int i = 0; i < population->population_size; i++)
	{
		double val = population->genome_array[i].fitness;
		total_fitness += val;
		
		if (val > current_best)
		{
			
			current_best = val;
			population->fittest_genome = &population->genome_array[i];
		}
		if (val < current_worst) current_worst = val;
	}

	population->average_fitness = total_fitness / (double) population->population_size;
	population->total_fitness = total_fitness;
	population->best_fitness = current_best;
	population->worst_fitness = current_worst;
}

neurogen_genome_t* neurogen_population_roulette_selection(const neurogen_population_t* population)
{
	double total = population->total_fitness;
	double random = prandom_double_range(0.0, total);
	double sum = 0;
	
	for (int i = 0; i < population->population_size; i++)
	{
		double current = population->genome_array[i].fitness;

		// printf("Total: %f, sum: %f, current: %f, random: %f\n", total, sum, current, random);		
		
		if ((current + sum) >= random)
		{
			return &population->genome_array[i];
		}
		else {
			sum += current;
		}
	}
	// If nothing returned
	puts("ERROR: did not return genome");
	exit(1);
}

double neurogen_errorfunction_simple(const neuron_array_t* output, const neuron_array_t* expected)
{
	return neuron_array_difference(output, expected);
}

neurogen_population_t neurogen_population_evolve (const neurogen_population_t* pop)
{
	neurogen_population_t new_pop = neurogen_population_duplicate(pop);

	for (size_t i = 0; i < pop->population_size; i += 2)
	{
		neurogen_genome_t* parent_a = neurogen_population_roulette_selection(pop);
		neurogen_genome_t* parent_b = neurogen_population_roulette_selection(pop);
		neurogen_genome_t child_a;
		neurogen_genome_t child_b;

		neurogen_genome_crossover_random(parent_a, parent_b, &child_a, &child_b, pop->crossover_rate);

		neurogen_genome_mutate(&child_a, pop->mutation_rate);
		neurogen_genome_mutate(&child_b, pop->mutation_rate);

		new_pop.genome_array[i] = child_a;
		new_pop.genome_array[i+1] = child_b;
	}		

	new_pop.generation++;
	return new_pop;
}
