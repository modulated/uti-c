#include "neuron.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h> // exp(), fabs()


// ===========================
// Utility Functions
// ===========================

double neuron_relu (double input, double max) 
{
	input = input > 0 ? input : 0;
	input = input < max ? input : max;
	
	return input;
}

double neuron_sigmoid(double input, double response)
{
	return 1.0 / (1.0 + exp(-input * response));
}


// ===========================
// Array Functions
// ===========================

neuron_array_t neuron_array_construct(size_t length) 
{
	neuron_array_t array;
	array.length = length;
	array.array = malloc((length + 1) * sizeof(double));
	array.array[length] = NAN;

	for (size_t i = 0; i < length; i++)
	{
		array.array[i] = 0.0;
	}
	return array;
}

neuron_array_t neuron_array_duplicate(const neuron_array_t* array)
{
	neuron_array_t output = neuron_array_construct(array->length);
	for (int i = 0; i < array->length; i++)
	{
		output.array[i] = array->array[i];
	}
	return output;
}

void neuron_array_set(neuron_array_t* array, size_t index, double value)
{
	if (index >= array->length) {
		puts("ERROR: array write overflow.");
		exit(1);
	}
	array->array[index] = value;
}

double neuron_array_get(const neuron_array_t* array, size_t index)
{
	if (index >= array->length) {
		puts("ERROR: array read overflow.");
		exit(1);
	}
	return array->array[index];
}

void neuron_array_print(const neuron_array_t* array)
{
	printf("Array length: %lu\n", array->length);
	for (int i = 0; i < array->length; i++)
	{
		printf("%.2f ", array->array[i]);
	}
	puts("");
}

void neuron_array_copy(neuron_array_t* dest, size_t index_dest, const neuron_array_t* src, size_t index_source, size_t length)
{
	if (index_source + length > src->length) goto error;
	if (index_dest + length > dest->length) goto error;

	for (int i = 0; i < length; i++)
	{
		double val = neuron_array_get(src, index_source + i);
		neuron_array_set(dest, index_dest + i, val);
	}
	return;

	error:
		puts("ERROR: array copy bounds check.");
		exit(1);
}

neuron_array_t neuron_array_crossover(const neuron_array_t* array_start, const neuron_array_t* array_end, size_t index)
{
	if (array_start->length != array_end->length)
	{
		puts("ERROR: arrays not same length.");
		exit(1);
	}

	neuron_array_t output = neuron_array_construct(array_start->length);
	for (int i = 0; i < index; i++)
	{
		output.array[i] = array_start->array[i];
	}
	for (int i = index; i < array_start->length; i++)
	{
		output.array[i] = array_end->array[i];
	}
	return output;
}

neuron_array_t neuron_array_slice(const neuron_array_t* dest, size_t index, size_t length)
{
	if (index + length > dest->length) {
		puts("ERROR: array copy bounds check.");
		exit(1);
	}

	neuron_array_t output_array = neuron_array_construct(length);

	for (int i = 0; i < length; i++)
	{
		double val = neuron_array_get(dest, index + i);
		neuron_array_set(&output_array, i, val);
	}

	return output_array;
}

double neuron_array_difference(const neuron_array_t* array_a, const neuron_array_t* array_b)
{
	if (array_a->length != array_b->length)
	{
		puts("ERROR: arrays are not equal length, cannot compare.");
		exit(1);
	}

	double difference = 0.0;

	for (int i = 0; i < array_a->length; i++)
	{
		difference += fabs(neuron_array_get(array_a, i) - neuron_array_get(array_b, i));
	}

	return difference;
}

void neuron_array_destruct(neuron_array_t* array)
{
	free(array->array);
	array->array = NULL;
	array->length = 0;
}


// ===========================
// Dataset Functions
// ===========================

neuron_io_t neuron_io_construct(neuron_array_t* input, neuron_array_t* expected)
{
	neuron_io_t io;
	io.input = neuron_array_duplicate(input);
	io.expected = neuron_array_duplicate(expected);

	return io;
}

void neuron_io_destruct(neuron_io_t* io)
{
	neuron_array_destruct(&io->input);
	neuron_array_destruct(&io->expected);
}

neuron_dataset_t neuron_dataset_construct(size_t inputs, size_t outputs)
{ 
	neuron_dataset_t set;
	set.length = 0;
    set.block = 32;
	set.inputs = inputs;
	set.outputs = outputs;
	
	return set;
}

void neuron_dataset_add(neuron_dataset_t* set, double inputs[], double outputs[])
{
	if (set->length % set->block == 0) 
	{
		set->data = realloc(set->data, (set->block * ((set->length + 32) / set->block)) * sizeof(neuron_io_t)); 
        printf("realloc %lu\n", set->block * ((set->length + 32) / set->block));
	}

	neuron_array_t array_in = neuron_array_construct(set->inputs);

	for (int i = 0; i < set->inputs; i++)
	{
		neuron_array_set(&array_in, i, inputs[i]);
	}

	set->data[set->length].input = array_in;

	neuron_array_t array_out = neuron_array_construct(set->outputs);

	for (int i = 0; i < set->outputs; i++)
	{
		neuron_array_set(&array_out, i, outputs[i]);
	}

	set->data[set->length].expected = array_out;

	set->length++;
}

void neuron_dataset_destruct(neuron_dataset_t* set)
{
	for (int i = 0; i < set->length; i++)
	{
		neuron_array_destruct(&set->data[i].input);
		neuron_array_destruct(&set->data[i].expected);
	}
	free(set->data);
	set->data = NULL;
	set->length = 0;
}

// ===========================
// Neuron Component Functions
// ===========================

neuron_unit_t neuron_unit_construct (size_t inputs)
{
	neuron_unit_t neuron;
	neuron.inputs = inputs;
	neuron.weights = neuron_array_construct(inputs);
	
	for (size_t i = 0; i < inputs; i++)
	{
		neuron_array_set(&neuron.weights, i, 0.0);
	}
	neuron.bias = -1.0;

	return neuron;
}

void neuron_unit_destruct (neuron_unit_t* neuron)
{
	neuron_array_destruct(&neuron->weights);
	neuron->inputs = 0;
}

neuron_layer_t neuron_layer_construct(size_t number_of_neurons, size_t inputs_per_neuron)
{
	neuron_layer_t layer;
	layer.size = number_of_neurons;
	layer.num_weights = number_of_neurons * inputs_per_neuron;
	layer.neurons = malloc(number_of_neurons * sizeof(neuron_unit_t));
	 
	for (size_t i = 0; i < number_of_neurons; i++)
	{
		layer.neurons[i] = neuron_unit_construct(inputs_per_neuron);
	}

	return layer;
}

void neuron_layer_destruct(neuron_layer_t* layer) 
{
	for (int i = 0; i < layer->size; i++)
	{
		neuron_unit_destruct(&layer->neurons[i]);
	}

	free(layer->neurons);
	layer->neurons = NULL;
	layer->num_weights = 0;
	layer->size = 0;
}

static neuron_array_t neuron_layer_get_weights(const neuron_layer_t* layer)
{
	int num_inputs = layer->neurons->inputs;
	int num_neurons = layer->size;

	neuron_array_t output_array = neuron_array_construct(num_inputs * num_neurons);

	for (int neuron = 0; neuron < layer->size; neuron++)
	{
		neuron_array_t* neuron_weights =  &layer->neurons[neuron].weights;

		for (int input = 0; input < num_inputs; input++)
		{
			double neuron_weight = neuron_array_get(neuron_weights, input);
			
			int output_index = (neuron * num_inputs) + input;
			neuron_array_set(&output_array, output_index, neuron_weight);
		}
	}

	return output_array;
}

static void neuron_layer_set_weights(neuron_layer_t* layer, const neuron_array_t* input_array)
{
	if (input_array->length != layer->num_weights)
	{
		puts("ERROR: input weight length not matching network weight length");
		printf("input: %lu, layer: %lu\n", input_array->length, layer->num_weights);
		exit(1);
	}

	for (int neuron = 0; neuron < layer->size; neuron++)
	{
		neuron_array_t* neuron_weights =  &layer->neurons[neuron].weights;
		size_t offset = neuron * neuron_weights->length;
		neuron_array_copy(neuron_weights, 0, input_array, offset, neuron_weights->length);
	}
}

neuron_network_t neuron_network_construct(size_t num_inputs, size_t num_outputs, size_t num_layers, size_t layer_size)
{
	neuron_network_t network;
	network.num_inputs = num_inputs;
	network.num_outputs = num_outputs;
	network.num_layers = num_layers;
	network.layer_size = layer_size;
	network.summing_function = neuron_relu;

	network.layers = malloc((num_layers + 1) * sizeof (neuron_layer_t)); // Last = output row
	
	network.layers[0] = neuron_layer_construct(layer_size, num_inputs); // First layer recieves from inputs 

	for (size_t i = 1; i < num_layers; i++)
	{
		network.layers[i] = neuron_layer_construct(layer_size, layer_size);
	}

	network.layers[num_layers] = neuron_layer_construct(num_outputs, layer_size); // Last layer is output
	
	network.num_weights = neuron_network_size_weights(&network);

	return network;
}

void neuron_network_destruct(neuron_network_t* network) 
{
	for (int i = 0; i < network->num_layers + 1; i++)
	{
		neuron_layer_destruct(&network->layers[i]);
	}

	free(network->layers);
	network->layers = NULL;
}

void neuron_network_set_summing(neuron_network_t* network, double (*summing_function)(double input, double param))
{
	network->summing_function = summing_function;
}

neuron_array_t neuron_network_get_weights(const neuron_network_t* network) 
{
	int num_weights = network->num_weights;
	neuron_array_t output_array = neuron_array_construct(num_weights);
	int inputs = network->num_inputs;
	int outputs = network->num_outputs;
	int layer_num = network->num_layers + 1; // Add output layer
	int layer_size = network->layer_size;

	int first_layer_length = inputs * layer_size;
	int mid_layer_length = layer_size * layer_size;
	int last_layer_length = outputs * layer_size;
	

	neuron_array_t first_layer_weights = neuron_layer_get_weights(&network->layers[0]);	
	neuron_array_copy(&output_array, 0, &first_layer_weights, 0, first_layer_length);
	neuron_array_destruct(&first_layer_weights);
	
	
	for (int mid_layer = 0; mid_layer < layer_num-2; mid_layer++)
	{	
		int offset_mid_layer = first_layer_length + (mid_layer) * mid_layer_length;
		neuron_array_t mid_layer_weights = neuron_layer_get_weights(&network->layers[mid_layer + 1]);
		neuron_array_copy(&output_array, offset_mid_layer, &mid_layer_weights, 0, mid_layer_length);
		neuron_array_destruct(&mid_layer_weights);
	}
	
	
	neuron_array_t last_layer_weights = neuron_layer_get_weights(&network->layers[layer_num - 1]);
	int offset_last_layer = num_weights - last_layer_length;	
	neuron_array_copy(&output_array, offset_last_layer, &last_layer_weights, 0, last_layer_length);
	neuron_array_destruct(&last_layer_weights);


	return output_array;
}

size_t neuron_network_size_weights(const neuron_network_t* network)
{
	// int num = (network->num_inputs + network->num_outputs + (network->num_layers -1) * network->layer_size) * network->layer_size; // (in + out + num - 1 * size) * size

	int num_weights = 0;
	for (int i = 0; i < network->num_layers + 1; i++) {
		num_weights += network->layers[i].num_weights;
	}

	return num_weights;
}

void neuron_network_set_weights(neuron_network_t* network, const neuron_array_t* weights)
{
	int num_weights = network->num_weights;
	if (num_weights != weights->length)
	{
		puts("ERROR: number of weights do not match network.");
		exit(1);
	}
	for (int i = 0; i < num_weights; i++)
	{
		if (weights->array[i] > 1.0 || weights->array[i] < 0.0) 
		{
			puts("ERRROR: illegal weight.");
			exit(1);
		}
	}

	// int outputs = network->num_outputs;
	int total_layers = network->num_layers + 1; // Add output layer
	// int layer_size = network->layer_size;

	int first_layer_length = network->layers[0].num_weights;
	int mid_layer_length = network->layers[1].num_weights;
	int last_layer_length = network->layers[total_layers - 1].num_weights;
	neuron_array_t first_array = neuron_array_slice(weights, 0, first_layer_length);	

	neuron_layer_set_weights(&network->layers[0], &first_array);
	neuron_array_destruct(&first_array);
	
	for (int mid_layer = 1; mid_layer < total_layers - 1; mid_layer++)
	{	
		int offset_mid_layer = first_layer_length + (mid_layer-1) * mid_layer_length;
		neuron_array_t mid_array = neuron_array_slice(weights, offset_mid_layer, mid_layer_length);

		neuron_layer_set_weights(&network->layers[mid_layer], &mid_array);
		neuron_array_destruct(&mid_array);
	}

	neuron_array_t last_array = neuron_array_slice(weights, 0, last_layer_length);
	neuron_layer_set_weights(&network->layers[total_layers - 1], &last_array);
	neuron_array_destruct(&last_array);
}

neuron_array_t neuron_network_update(const neuron_network_t* network, const neuron_array_t* in)
{
	neuron_array_t outputs; // Stores outputs from each layer
	neuron_array_t inputs = neuron_array_duplicate(in);
	
	for (int i = 0; i < network->num_layers + 1; i++) // For each layer (+1 for outputs)
	{
		if (i > 0) 
		{
			neuron_array_destruct(&inputs);
			inputs = neuron_array_duplicate(&outputs); //save previous layers output;
			neuron_array_destruct(&outputs);
		}

		// Initiate output for each layer
		neuron_layer_t* current_layer = &network->layers[i];
		outputs = neuron_array_construct(network->layers[i].size);

		// Debugging
		// printf("Input size %d\n", inputs->length);
		// printf("Layer: %d Inputs size: %d Outputs size: %d\n", i, inputs->length, outputs.length);
		// printf("Inputs: ");
		// for (int k = 0; k < inputs->length; k++)
		// {
		// 	printf("%d: %f ", k, neuron_array_get(inputs, k));
		// }
		// puts("");
		// Debugging

		for (int j = 0; j < outputs.length; j++)
		{
			double sum_inputs = 0;

			neuron_unit_t current_neuron = current_layer->neurons[j];

			if (network->layers[i].neurons->weights.length != inputs.length) 
			{
				puts("ERROR: inputs not equal to layer weights.");
				printf("inputs: %lu, weights: %lu\n", network->layers[i].neurons->inputs, inputs.length);
				exit(1);
			}

			// Sum each weight * input
			for (int k = 0; k < current_neuron.weights.length; k++)
			{
				double current_input = neuron_array_get(&inputs, k);
				double current_weight = neuron_array_get(&current_neuron.weights, k);
				sum_inputs += current_input * current_weight;
			}

			// Add in the bias
			sum_inputs += network->layers[i].neurons[j].bias;

			// Sigmoid for each neuron total	 
			 double sigmoid_output = network->summing_function(sum_inputs, 1);
			 neuron_array_set(&outputs, j, sigmoid_output);

			//  printf("Output size: %d, Sig %f -> %f\n", outputs.length, sum_inputs, sigmoid_output);
		}

	}
	
	return outputs;
}
