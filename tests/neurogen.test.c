#include <stdlib.h>
#include <stdio.h>
#include "tap.h"
#include "../src/neuron.h"
#include "../src/neurogen.h"


#define dirtycheck(num, equals) (num > equals-0.0000000001 && num < equals+0.0000000001)

void test_neurogen_genome_construct()
{
	int expected_length = 24;
	neuron_array_t array = neuron_array_construct(expected_length);	
	neurogen_genome_t genome = neurogen_genome_construct(&array);

	ok(
		(genome.chromosome.length = expected_length) &&
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
		(genome.chromosome.array == NULL) &&
		(genome.chromosome.length == 0) &&
		(genome.fitness == 0),
		"neurogen_genome_destruct.");
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
		&genome2 == neurogen_genome_compare(&genome1, &genome2),
		"neurogen_genome_set_fitness");

	neurogen_genome_destruct(&genome1);
	neurogen_genome_destruct(&genome2);
	neuron_array_destruct(&array1);
	neuron_array_destruct(&array2);
}

void test_neurogen_genome_crossover()
{
	int expected_length = 24;	
	neuron_array_t array1 = neuron_array_construct(expected_length);
	neuron_array_t array2 = neuron_array_construct(expected_length);
	neurogen_genome_t parent1 = neurogen_genome_construct(&array1);
	neurogen_genome_t parent2 = neurogen_genome_construct(&array2);

	neurogen_genome_t child1;
	neurogen_genome_t child2;
	neurogen_genome_crossover(&parent1, &parent2, &child1, &child2);

	ok(
		(child1.chromosome.length == child2.chromosome.length),
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
	double prob = 0.05;
	neuron_array_t array = neuron_array_construct(expected_length);
	neurogen_genome_t genome = neurogen_genome_construct(&array);
	
	// Unchanged
	neurogen_genome_mutate(&genome, 0.0);

	double unchanged = 0;
	for (int i = 0; i < expected_length; i++)
	{
		unchanged += neuron_array_get(&genome.chromosome, i);
	}

	neurogen_genome_mutate(&genome, prob);

	double sum = 0;
	for (int i = 0; i < expected_length; i++)
	{
		sum += neuron_array_get(&genome.chromosome, i);
	}
	ok(
		(dirtycheck(sum, 0.0) != 1) &&
		(dirtycheck(unchanged, 0.0) == 1),
		"neurogen_genome_mutate.") ||

	diag("Unchanged: expected %f got %f\n.", 0.0, unchanged) ||
	diag("Mutated: Did not expect %f got %f\n.", 0.0, sum);

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

	neurogen_population_t population = neurogen_population_construct(population_size, genome_length, mutation_rate, crossover_rate);

	ok(
		(population.population_size == population_size) &&
		(population.genome_length == genome_length) &&
		(population.crossover_rate == crossover_rate) &&
		(population.mutation_rate == mutation_rate) &&
		(dirtycheck(population.genomes[population_size-1].chromosome.array[genome_length-1], 0.0) == 1),
		"neurogen_population_construct");

	neurogen_population_destruct(&population);
}

void test_neurogen_population_destruct()
{
	int population_size = 100;
	int genome_length = 10;
	double mutation_rate = 0.05;
	double crossover_rate = 0.5;

	neurogen_population_t population = neurogen_population_construct(population_size, genome_length, mutation_rate, crossover_rate);
	
	neurogen_population_destruct(&population);

	ok(
		(population.genomes == NULL) &&
		(population.population_size == 0) &&
		(population.genome_length == 0),
		"neurogen_population_construct");
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

	neurogen_population_t population = neurogen_population_construct(population_size, genome_length, mutation_rate, crossover_rate);

	neurogen_genome_set_fitness(&population.genomes[0], min_fitness);
	for (int i = 1; i < population_size-1; i++)
	{
		neurogen_genome_set_fitness(&population.genomes[i], 0.5);
	}
	neurogen_genome_set_fitness(&population.genomes[population_size-1], max_fitness);

	neurogen_population_calculate_statistics(&population);

	ok(
		(population.fittest_genome == &population.genomes[population_size-1]) &&
		dirtycheck(population.best_fitness, max_fitness) == 1 &&
		dirtycheck(population.worst_fitness, min_fitness) &&
		dirtycheck(population.average_fitness, expected_average),
		"neurogen_population_calculate_statistics");

	neurogen_population_destruct(&population);
}

void test_all() {
	test_neurogen_genome_construct();
	test_neurogen_genome_destruct();
	test_neurogen_genome_set_fitness();
	test_neurogen_genome_compare();
	test_neurogen_genome_crossover();
	test_neurogen_genome_mutate();

	test_neurogen_population_construct();
	test_neurogen_population_destruct();
	test_neurogen_population_calculate_statistics();
}

int main(void) {
	plan();
	test_all();

	return exit_status();
}
