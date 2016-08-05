#include <stdlib.h>
#include <stdio.h>
#include "tap.h"
#include "../include/neuron.h"
#include "../include/neurogen.h"

#define dirtycheck(num, equals) (num > equals-0.0000000001 && num < equals+0.0000000001)



void test_neurogen_genome_construct()
{	
	int expected_length = 24;
	neuron_array_t array = neuron_array_construct(expected_length);	
	neurogen_genome_t genome = neurogen_genome_construct(&array);

	ok(
		(genome.array.length = expected_length) &&
		(genome.fitness == 0),
		"neurogen_genome_construct.");

	neurogen_genome_destruct(&genome);
	neuron_array_destruct(&array);
}

void test_neurogen_genome_destruct()
{
	int expected_length = 24;
	neuron_array_t array = neuron_array_construct(expected_length);
	neurogen_genome_t genome = neurogen_genome_construct(&array);
	neurogen_genome_destruct(&genome);
	neuron_array_destruct(&array);

	ok(
		(genome.array.array == NULL) &&
		(genome.array.length == 0) &&
		(genome.fitness == 0),
		"neurogen_genome_destruct.");
}

void test_neurogen_genome_duplicate()
{
	int expected_length = 24;
	neuron_array_t array = neuron_array_construct(expected_length);
	neurogen_genome_t genome = neurogen_genome_construct(&array);
	neurogen_genome_t copy;

	for (size_t i = 0; i < expected_length; i++)
	{
		neuron_array_set(&genome.array, i, 1.0);
	}

	neurogen_genome_duplicate(&copy, &genome);

	size_t errors = 0;
	for (size_t i = 0; i < expected_length; i++)
	{
		if (!dirtycheck(copy.array.array[i], genome.array.array[i])){
			errors++;
			diag("Expected: %f got: %f", genome.array.array[i], copy.array.array[i]);
		}
	}

	ok(
		copy.array.length == expected_length &&
		errors == 0,
		"neurogen_genome_duplicate."
	) ||
	diag("Length: expected %d got %lu", expected_length, copy.array.length);

	neurogen_genome_destruct(&genome);
	neurogen_genome_destruct(&copy);
	neuron_array_destruct(&array);
}

void test_neurogen_genome_set_fitness()
{
	int expected_length = 24;
	double expected_fitness = 99.99;
	neuron_array_t array = neuron_array_construct(expected_length);	
	neurogen_genome_t genome = neurogen_genome_construct(&array);
	neurogen_genome_set_fitness(&genome, expected_fitness);

	ok(
		dirtycheck(genome.fitness, expected_fitness),
		"neurogen_genome_set_fitness");

	neurogen_genome_destruct(&genome);
	neuron_array_destruct(&array);
}

void test_neurogen_genome_compare()
{
	int expected_length = 24;	
	neuron_array_t array1 = neuron_array_construct(expected_length);
	neuron_array_t array2 = neuron_array_construct(expected_length);	
	neurogen_genome_t genome1 = neurogen_genome_construct(&array1);
	neurogen_genome_t genome2 = neurogen_genome_construct(&array2);
	neurogen_genome_set_fitness(&genome1, 1.0);
	neurogen_genome_set_fitness(&genome2, 1.1);

	ok(
		&genome2 == neurogen_genome_compare_fitness(&genome1, &genome2),
		"neurogen_genome_set_fitness");

	neurogen_genome_destruct(&genome1);
	neurogen_genome_destruct(&genome2);
	neuron_array_destruct(&array1);
	neuron_array_destruct(&array2);
}

void test_neurogen_genome_crossover_index()
{
	int expected_length = 24;	
	neuron_array_t array1 = neuron_array_construct(expected_length);
	neuron_array_t array2 = neuron_array_construct(expected_length);
	for (int i = 0; i < expected_length; i++)
	{
		array1.array[i] = 0.5;
		array2.array[i] = 0.2;
	}
	neurogen_genome_t parent1 = neurogen_genome_construct(&array1);
	neurogen_genome_t parent2 = neurogen_genome_construct(&array2);

	neurogen_genome_t child1;
	neurogen_genome_t child2;
	neurogen_genome_crossover_index(&parent1, &parent2, &child1, &child2, expected_length/2);

	// neuron_array_print(&parent1.array);
	// neuron_array_print(&parent2.array);
	// neuron_array_print(&child1.array);
	// neuron_array_print(&child2.array);

	ok(
		(child1.array.array[13] < 0.4) ||
		(child2.array.array[13] < 0.4),
		"neurogen_genome_crossover_index.") ||
	diag("Expected %f, %f got %f, %f", 0.5, 0.3, child1.array.array[expected_length/2], child2.array.array[expected_length/2]);	

	neurogen_genome_destruct(&parent1);
	neurogen_genome_destruct(&parent2);
	neurogen_genome_destruct(&child1);
	neurogen_genome_destruct(&child2);
	neuron_array_destruct(&array1);
	neuron_array_destruct(&array2);
}

void test_neurogen_genome_crossover_random()
{
	int expected_length = 24;	
	neuron_array_t array1 = neuron_array_construct(expected_length);
	neuron_array_t array2 = neuron_array_construct(expected_length);
	neurogen_genome_t parent1 = neurogen_genome_construct(&array1);
	neurogen_genome_t parent2 = neurogen_genome_construct(&array2);

	neurogen_genome_t child1;
	neurogen_genome_t child2;
	neurogen_genome_crossover_random(&parent1, &parent2, &child1, &child2, 1.0 /* always */);

	ok(
		(child1.array.length == child2.array.length) &&
		child1.array.array[0] == parent1.array.array[0] &&
		child1.array.array[expected_length-1] == parent2.array.array[expected_length-1],
		"neurogen_genome_crossover.");

	neurogen_genome_destruct(&parent1);
	neurogen_genome_destruct(&parent2);
	neurogen_genome_destruct(&child1);
	neurogen_genome_destruct(&child2);
	neuron_array_destruct(&array1);
	neuron_array_destruct(&array2);
}

void test_neurogen_genome_mutate()
{
	int expected_length = 24;
	double prob = 0.1;
	neuron_array_t array = neuron_array_construct(expected_length);
	neurogen_genome_t genome = neurogen_genome_construct(&array);
	
	// Unchanged
	neurogen_genome_mutate(&genome, 0.0);

	double unchanged = 0;
	for (int i = 0; i < expected_length; i++)
	{
		unchanged += neuron_array_get(&genome.array, i);
	}

	neurogen_genome_mutate(&genome, prob);

	double sum = 0;
	for (int i = 0; i < expected_length; i++)
	{
		sum += neuron_array_get(&genome.array, i);
	}
	ok(
		(dirtycheck(sum, 0.0) != 1) &&
		dirtycheck(unchanged, 0.0),
		"neurogen_genome_mutate.") ||

	diag("Unchanged: expected %f got %f.", 0.0, unchanged) ||
	diag("Mutated: Did not expect %f got %f.", 0.0, sum);

	neurogen_genome_destruct(&genome);
	neuron_array_destruct(&array);
}

/*
** Population Testing
*/

void test_neurogen_population_construct()
{
	int population_size = 100;
	int genome_length = 10;
	double mutation_rate = 0.05;
	double crossover_rate = 0.5;

	neurogen_population_t population = neurogen_population_construct(population_size, genome_length, mutation_rate, crossover_rate, neurogen_errorfunction_simple);

	ok(
		(population.population_size == population_size) &&
		(population.genome_length == genome_length) &&
		(population.crossover_rate == crossover_rate) &&
		(population.mutation_rate == mutation_rate) &&
		(dirtycheck(population.genome_array[population_size-1].array.array[genome_length-1], 0.0) == 1),
		"neurogen_population_construct");

	neurogen_population_destruct(&population);
}

void test_neurogen_population_destruct()
{
	int population_size = 100;
	int genome_length = 10;
	double mutation_rate = 0.05;
	double crossover_rate = 0.5;

	neurogen_population_t population = neurogen_population_construct(population_size, genome_length, mutation_rate, crossover_rate, neurogen_errorfunction_simple);
	
	neurogen_population_destruct(&population);

	ok(
		(population.genome_array == NULL) &&
		(population.population_size == 0) &&
		(population.genome_length == 0),
		"neurogen_population_construct");
}

void test_neurogen_population_duplicate()
{
	int population_size = 100;
	int genome_length = 100;
	double mutation_rate = 0.05;
	double crossover_rate = 0.5;

	neurogen_population_t test_population = neurogen_population_construct(population_size, genome_length, mutation_rate, crossover_rate, neurogen_errorfunction_simple);
	neurogen_population_t test_copy = neurogen_population_duplicate(&test_population);

	double errors = 0;
	int length = 0;
	for (size_t i = 0; i < population_size; i++)
	{
		errors += neuron_array_difference(&test_population.genome_array[i].array, &test_copy.genome_array[i].array);
		
		if (test_population.genome_array[i].array.length != test_copy.genome_array[i].array.length) length++;
	}
	ok(
		dirtycheck(0.0, errors) &&
		length == 0,
		"neurogen_population_duplicate"
	);

	neurogen_population_destruct(&test_population);
	neurogen_population_destruct(&test_copy);
}

void test_neurogen_population_calculate_statistics()
{
	int population_size = 100;
	int genome_length = 10;
	double mutation_rate = 0.05;
	double crossover_rate = 0.5;
	double max_fitness = 1.0;
	double min_fitness = 0.1;
	double expected_average = 0.501;

	neurogen_population_t population = neurogen_population_construct(population_size, genome_length, mutation_rate, crossover_rate, neurogen_errorfunction_simple);

	neurogen_genome_set_fitness(&population.genome_array[0], min_fitness);
	for (int i = 1; i < population_size-1; i++)
	{
		neurogen_genome_set_fitness(&population.genome_array[i], 0.5);
	}
	neurogen_genome_set_fitness(&population.genome_array[population_size-1], max_fitness);

	neurogen_population_calculate_statistics(&population);

	ok(
		(population.fittest_genome == &population.genome_array[population_size-1]) &&
		dirtycheck(population.best_fitness, max_fitness) == 1 &&
		dirtycheck(population.worst_fitness, min_fitness) &&
		dirtycheck(population.average_fitness, expected_average),
		"neurogen_population_calculate_statistics");

	neurogen_population_destruct(&population);
}

void test_neurogen_population_evolve (void)
{
	int population_size = 50;
	int genome_length = 10;
	double mutation_rate = 0.05;
	double crossover_rate = 0.7;

	neurogen_population_t test_pop = neurogen_population_construct(population_size, genome_length, mutation_rate, crossover_rate, neurogen_errorfunction_simple);

	for (size_t i = 0; i < test_pop.population_size; i++)
	{
		neurogen_genome_set_fitness(&test_pop.genome_array[i], 1.0);
	}

	neurogen_population_t new_pop = neurogen_population_evolve(&test_pop);


	size_t error = 0;
	for (size_t i = 0; i < test_pop.population_size; i++)
	{
		if (test_pop.genome_array[i].array.length != new_pop.genome_array[i].array.length) error++;
	}

	ok(
		error == 0 &&
		new_pop.generation == (test_pop.generation+1) &&
		test_pop.population_size == new_pop.population_size &&
		test_pop.genome_length == new_pop.genome_length,
		"neurogen_population_evolve.");

	neurogen_population_destruct(&new_pop);
	neurogen_population_destruct(&test_pop);
}

void test_neurogen_population_roulette_selection (void)
{
	neurogen_population_t pop = neurogen_population_construct(10, 4, 1, 1, neuron_array_difference);
	
	for (size_t i = 0; i < 10; i++)
	{
		pop.genome_array[i].fitness = i;
	}

	neurogen_population_calculate_statistics(&pop);

	neurogen_genome_t* selected_genome = neurogen_population_roulette_selection(&pop);
	
	ok(selected_genome->fitness > 0.9, "neurogen_population_roulette");
}

void test_neurogen_genome ()
{
	test_neurogen_genome_construct();
	test_neurogen_genome_destruct();
	test_neurogen_genome_duplicate();
	test_neurogen_genome_set_fitness();
	test_neurogen_genome_compare();
	test_neurogen_genome_crossover_index();
	test_neurogen_genome_crossover_random();
	test_neurogen_genome_mutate();
}

void test_neurogen_population ()
{
	test_neurogen_population_construct();
	test_neurogen_population_destruct();
	test_neurogen_population_duplicate();
	test_neurogen_population_evolve();
	test_neurogen_population_calculate_statistics();
	test_neurogen_population_roulette_selection();
}

void test_all() {
	test_neurogen_genome();
	test_neurogen_population();
	// test_neurogen_population_evolve();
}

int main(void) {
	plan();
	test_all();

	return exit_status();
}
