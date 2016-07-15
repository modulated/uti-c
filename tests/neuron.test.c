#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tap.h"
#include "../src/neuron.h"
#include "float.h"

// #define abs(a) (a >= 0 ? a : -a)
// #define approx(a, b) (abs(a - b) <= FLT_EPSILON)
#define dirtycheck(num, equals) (num > equals-0.00000001 && num < equals+0.00000001)

void test_neuron_array_construct()
{
	neuron_array_t array = neuron_array_construct(10);
	ok(isnan(array.array[array.length]), "neuron_array_construct.") || 
	diag("array[length] is not NAN.");

	neuron_array_destruct(&array);
}

void test_neuron_unit_construct()
{
	neuron_unit_t test_neuron = neuron_unit_construct(2);
	
	ok( (test_neuron.inputs == 2), "neuron_unit_construct.") || 
	diag("Inputs: expected %d, got %d.", 2, test_neuron.inputs);
	
	neuron_unit_destruct(&test_neuron);
}

void test_neuron_unit_destruct()
{
	neuron_unit_t test_neuron = neuron_unit_construct(10);
	neuron_unit_destruct(&test_neuron);

	ok( (test_neuron.weights.array == NULL && test_neuron.inputs == 0), "neuron_unit_destruct.");
}

void test_neuron_layer_construct()
{
	int expected_size = 4;
	int expected_inputs = 4;
	neuron_layer_t test_layer = neuron_layer_construct(expected_size, expected_inputs);
	
	int err_counter = 0;
	for (int i = 0; i < expected_size; i++)
	{
		neuron_unit_t neuron = test_layer.neurons[i];

		for (int j = 0; j < expected_inputs; j++)
		{
			double weight = neuron_array_get(&neuron.weights, i);
			if (weight > 1.0 || weight < 0.0)
			{
				err_counter++;
			}
		}
	}

	ok( (test_layer.size == expected_size) && (err_counter == 0), "neuron_layer_construct.") || 
	diag("Inputs: expected %d, got %d.", expected_size, test_layer.size) ||
	diag("Neuron: expected weight range 0.0 to 1.0.");
	
	neuron_layer_destruct(&test_layer);
}

void test_neuron_layer_destruct() 
{
	neuron_layer_t test_layer = neuron_layer_construct(4,4);
	neuron_layer_destruct(&test_layer);
	ok((test_layer.neurons == NULL) && (test_layer.size == 0), "neuron_layer_destruct.");
}

void test_neuron_network_construct()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,2, neuron_relu);

	ok(
		(test_network.num_inputs == 1) &&
		(test_network.num_outputs == 1) &&
		(test_network.num_layers == 2) &&
		(test_network.layer_size == 2)
	, "neuron_network_construct.");

	neuron_network_destruct(&test_network);
}

void test_neuron_network_destruct()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,4, neuron_relu);
	neuron_network_destruct(&test_network);

	ok(test_network.layers == NULL, "neuron_network_destruct.");

}

void test_neuron_network_get_num_weights()
{
	neuron_network_t test_network = neuron_network_construct(1,1,3,4, neuron_relu);
	int num = neuron_network_get_num_weights(&test_network);
	int expected = 40;
	ok(num == expected, "neuron_network_get_num_weights.");

	neuron_network_destruct(&test_network);
}

void test_neuron_network_get_weights()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,2, neuron_relu);
	
	neuron_array_t result = neuron_network_get_weights(&test_network);

	int lessthan = 0;
	for (int i = 0; i < result.length; i++) {
		if (result.array[i] > 1.0) lessthan++;
	}

	ok(lessthan == 0, "neuron_network_get_weights.");

	neuron_network_destruct(&test_network);
	neuron_array_destruct(&result);
}

void test_neuron_network_set_weights()
{
	neuron_network_t test_network = neuron_network_construct(1,1,2,2, neuron_relu);	
	
	neuron_array_t array = neuron_array_construct(test_network.num_weights);
	for (int i = 0; i < array.length; i++) 
	{
		neuron_array_set(&array, i, 0.0);
	}
	neuron_network_set_weights(&test_network, &array);

	int check = 0;
	neuron_array_t result = neuron_network_get_weights(&test_network);
	for (int i = 0; i < result.length; i++) 
	{
		if (dirtycheck(result.array[i], 0) == 0) check++;
	}
	ok(check == 0, "neuron_network_set_weights.");

	neuron_array_destruct(&result);
	neuron_array_destruct(&array);
	neuron_network_destruct(&test_network);
}

void test_neuron_network_update() {
	neuron_network_t test_network = neuron_network_construct(2,2,4,4, neuron_relu);	
	neuron_array_t input = neuron_array_construct(2);
	neuron_array_set(&input, 0, 1.0);
	neuron_array_set(&input, 1, 1.0);	

	neuron_array_t output = neuron_network_update(&test_network, &input);


	int err_counter = 0;
	for (int i = 0; i < output.length; i++)
	{
		double weight = neuron_array_get(&output, i);
		if (weight > 1.0 || weight < 0.0) err_counter++;		
	}
	
	
	ok(err_counter == 0, "neuron_network_update.") ||
	diag("output not within range 0.0 to 1.0");
	
	neuron_array_destruct(&output);
	neuron_array_destruct(&input);
	neuron_network_destruct(&test_network);
}
void test_neuron_network_get_sigmoid() {

}

void test_all() {
	srand(0);

	test_neuron_array_construct();
	test_neuron_unit_construct();
	test_neuron_unit_destruct();
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
