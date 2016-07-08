#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tap.h"
#include "../src/neuron.h"
#include "float.h"

#define abs(a) (a >= 0 ? a : -a)
#define approx(a, b) (abs(a - b) <= FLT_EPSILON)

void test_neuron_construct() 
{
	neuron_t test_neuron = neuron_construct(2);
	
	ok( (test_neuron.inputs == 2), "neuron_create returns expected values.") || 
	diag("Inputs: expected %d, got %d.", 2, test_neuron.inputs);
	
	neuron_destruct(&test_neuron);
}

void test_neuron_destruct() 
{
	neuron_t test_neuron = neuron_construct(10);
	neuron_destruct(&test_neuron);

	ok( (test_neuron.weights == NULL), "neuron_destruct returns expected values.");
}

void test_neuron_layer_construct() 
{
	int expected_size = 4;
	neuron_layer_t test_layer = neuron_layer_construct(expected_size,2);
	
	ok( (test_layer.size == expected_size) && (test_layer.neurons != NULL), "neuron_layer_create returns expected values.") || 
	diag("Inputs: expected %d, got %d.", expected_size, test_layer.size);
	
	neuron_layer_destruct(&test_layer);
}

void test_neuron_layer_destruct() 
{
	neuron_layer_t test_neuron_layer = neuron_layer_construct(4,4);
	neuron_layer_destruct(&test_neuron_layer);

	ok( (test_neuron_layer.neurons == NULL), "neuron_layer_destruct returns expected values.");
}

void test_neuron_network_construct()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,4);

	ok(
		(test_network.net_inputs == 1) &&
		(test_network.net_outputs == 1) &&
		(test_network.hidden_layer_num == 2) &&
		(test_network.hidden_layer_size == 4)
	, "neuron_network_construct returns expected values.");

	neuron_network_destruct(&test_network);
}

void test_neuron_network_destruct()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,4);
	neuron_network_destruct(&test_network);

	ok(test_network.layers == NULL, "neuron_network_destruct returns expected values.");

}

void test_neuron_network_get_num_weights()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,2);
	int num = neuron_network_get_num_weights(&test_network);
	int expected = 8;
	ok(num == expected, "neuron_network_get_num_weights returns expected value.");

	neuron_network_destruct(&test_network);
}

void test_neuron_network_get_weights()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,2);
	int length = neuron_network_get_num_weights(&test_network);
	double expected[length]; 
	double* array = neuron_network_get_weights(&test_network);

	ok(memcmp(expected, array, sizeof(double) * length) == 0, "neuron_network_weights returns expected value.");

	neuron_network_destruct(&test_network);
}

void test_all() {
	srand(1);

	test_neuron_construct();
	test_neuron_destruct();
	test_neuron_layer_construct();
	test_neuron_layer_destruct();
	
}

int main(void) {
	plan();
	test_all();

	return exit_status();
}
