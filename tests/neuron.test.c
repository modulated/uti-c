#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tap.h"
#include "../src/neuron.h"
#include "float.h"

// #define abs(a) (a >= 0 ? a : -a)
// #define approx(a, b) (abs(a - b) <= FLT_EPSILON)
#define dirtycheck(num, equals) (num > equals-0.00000001 && num < equals+0.00000001)
void test_neuron_construct() 
{
	neuron_t test_neuron = neuron_construct(2);
	
	ok( (test_neuron.inputs == 2), "neuron_create.") || 
	diag("Inputs: expected %d, got %d.", 2, test_neuron.inputs);
	
	neuron_destruct(&test_neuron);
}

void test_neuron_destruct() 
{
	neuron_t test_neuron = neuron_construct(10);
	neuron_destruct(&test_neuron);

	ok( (test_neuron.weights == NULL && test_neuron.inputs == 10), "neuron_destruct.");
}

void test_neuron_layer_construct() 
{
	int expected_size = 4;
	int expected_inputs = 4;
	neuron_layer_t test_layer = neuron_layer_construct(expected_size, expected_inputs);
	
	ok( (test_layer.size == expected_size) && (test_layer.neurons[expected_size-1].weights[expected_inputs] <= 1.0), "neuron_layer_construct.") || 
	diag("Inputs: expected %d, got %d.", expected_size, test_layer.size) ||
	diag("Neuron: expected %f <= 1.0", test_layer.neurons[expected_size-1].weights[expected_inputs]);
	
	neuron_layer_destruct(&test_layer);
}

void test_neuron_layer_destruct() 
{
	neuron_layer_t test_neuron_layer = neuron_layer_construct(4,4);
	neuron_layer_destruct(&test_neuron_layer);

	ok( (test_neuron_layer.neurons == NULL), "neuron_layer_destruct.");
}

void test_neuron_network_construct()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,2);

	ok(
		(test_network.net_inputs == 1) &&
		(test_network.net_outputs == 1) &&
		(test_network.hidden_layer_num == 2) &&
		(test_network.hidden_layer_size == 2)
	, "neuron_network_construct.");

	neuron_network_destruct(&test_network);
}

void test_neuron_network_destruct()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,4);
	neuron_network_destruct(&test_network);

	ok(test_network.layers == NULL, "neuron_network_destruct.");

}

void test_neuron_network_get_num_weights()
{
	neuron_network_t test_network = neuron_network_construct(1,1,3,4);
	int num = neuron_network_get_num_weights(&test_network);
	int expected = 40;
	ok(num == expected, "neuron_network_get_num_weights.");

	neuron_network_destruct(&test_network);
}

void test_neuron_network_get_weights()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,2);
	int length = neuron_network_get_num_weights(&test_network);	
	
	double* result = neuron_network_get_weights(&test_network);

	int lessthan = 0;
	for (int i = 0; i < length; i++) {
		if (result[i] > 1.0) lessthan++;
	}

	ok(lessthan == 0, "neuron_network_get_weights.");

	neuron_network_destruct(&test_network);
	free(result);
}

void test_neuron_network_set_weights()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,2);
	int length = neuron_network_get_num_weights(&test_network);
	
	double array[length];
	for (int i = 0; i < length; i++) 
	{
		array[i] = 0.0;
	}
	neuron_network_set_weights(&test_network, array);

	int check = 0;
	double* result = neuron_network_get_weights(&test_network);
	for (int i = 0; i < length; i++) 
	{
		if (dirtycheck(result[i], 0) == 0) check++;
	}
	ok(check == 0, "neuron_network_set_weights.");

	free(result);
	neuron_network_destruct(&test_network);
}

void test_neuron_network_update() {
	neuron_network_t test_network = neuron_network_construct(2,2,4,4);	
	double input[2] = {0.0, 0.0};
	
	double* output = neuron_network_update(&test_network, input);
	printf("Out: %f\n", output[0]);
	
	ok(output[0] > 0.0 && output[0] < 1.0, "neuron_network_update.");
	
	free(output);
	neuron_network_destruct(&test_network);
}
void test_neuron_network_get_sigmoid() {

}

void test_all() {
	srand(1);

	test_neuron_construct();
	test_neuron_destruct();
	test_neuron_layer_construct();
	test_neuron_layer_destruct();
	test_neuron_network_construct();
	test_neuron_network_destruct();
	test_neuron_network_get_num_weights();
	test_neuron_network_get_weights();
	test_neuron_network_set_weights();
	test_neuron_network_update();
}

int main(void) {
	plan();
	test_all();

	return exit_status();
}
