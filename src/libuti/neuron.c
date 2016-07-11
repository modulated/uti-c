#include "../neuron.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static double neuron_relu_unbound (double input)
{
	return input > 0 ? input : 0;
}

static double neuron_relu_bounded (double input, double max) 
{
	input = input > 0 ? input : 0;
	input = input < max ? input : max;
	
	return input;
}

static double neuron_sigmoid(double input)
{
	return 1.0 / (1.0 + exp(-input * 1));
}

static double neuron_sigmoid_adjusted(double input, double response)
{
	return 1.0 / (1.0 + exp(-input * response));
}

static double neuron_random_weight () // Double between 0.0 and 1.0
{
	return rand() / (RAND_MAX + 1.0);
}

static double neuron_random_range (double min, double max)
{
	return min + (rand() / (RAND_MAX + (max-min)));
}

static neuron_array_t neuron_layer_get_weights(neuron_layer_t* layer)
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

static void neuron_layer_set_weights(neuron_layer_t* layer, neuron_array_t* input_array)
{
	if (input_array->length != layer->num_weights)
	{
		puts("ERROR: input weight length not matching network weight length");
		printf("input: %d, layer: %d\n", input_array->length, layer->num_weights);
		exit(1);
	}

	for (int neuron = 0; neuron < layer->size; neuron++)
	{
		neuron_array_t* neuron_weights =  &layer->neurons[neuron].weights;
		int offset = neuron * neuron_weights->length;
		neuron_array_copy(neuron_weights, 0, input_array, offset, neuron_weights->length);
	}
}


/*
** Public Functions
*/

neuron_array_t neuron_array_construct(int length) 
{
	neuron_array_t array;
	array.length = length;
	array.array = (double*)malloc((length + 1) * sizeof(double));
	array.array[length] = NAN;

	return array;
}

void neuron_array_set(neuron_array_t* array, int index, double value)
{
	if (index >= array->length) {
		puts("ERROR: array write overflow.");
		exit(1);
	}
	array->array[index] = value;
}

double neuron_array_get(neuron_array_t* array, int index)
{
	if (index >= array->length) {
		puts("ERROR: array read overflow.");
		exit(1);
	}
	return array->array[index];
}

void neuron_array_copy(neuron_array_t* dest_array, int index_dest, neuron_array_t* source_array, int index_source, int length)
{
	if (index_source + length > source_array->length) goto error;
	if (index_dest + length > dest_array->length) goto error;
	if (index_dest < 0 || length < 0 || index_source < 0) goto error;

	for (int i = 0; i < length; i++)
	{
		double val = neuron_array_get(source_array, index_source + i);
		neuron_array_set(dest_array, index_dest + i, val);
	}

	return;

	error:
		puts("ERROR: array copy bounds check.");
		exit(1);
}

neuron_array_t neuron_array_slice(neuron_array_t* dest_array, int index, int length)
{
	if (index + length > dest_array->length) goto error;	
	if (index < 0 || length < 0) goto error;

	neuron_array_t output_array = neuron_array_construct(length);

	for (int i = 0; i < length; i++)
	{
		double val = neuron_array_get(dest_array, index + i);
		neuron_array_set(&output_array, i, val);
	}

	return output_array;

	error:
		puts("ERROR: array copy bounds check.");
		exit(1);
}

void neuron_array_destruct(neuron_array_t* array)
{
	free(array->array);
	array->array = NULL;
	array->length = 0;
}

neuron_unit_t neuron_construct (int inputs)
{
	neuron_unit_t neuron;
	neuron.inputs = inputs;
	neuron.weights = neuron_array_construct(inputs);
	
	for (int i = 0; i < inputs; i++) // Last weight is Bias. Bias = -1 x Threshold 
	{
		neuron_array_set(&neuron.weights, i, neuron_random_weight());
		
	}

	neuron.bias = neuron_random_weight() * -1;

	return neuron;
}

void neuron_destruct (neuron_unit_t* neuron)
{
	neuron_array_destruct(&neuron->weights);
	neuron->inputs = 0;
}

neuron_layer_t neuron_layer_construct(int number_of_neurons, int inputs_per_neuron)
{
	neuron_layer_t layer;
	layer.size = number_of_neurons;
	layer.num_weights = number_of_neurons * inputs_per_neuron;
	layer.neurons = malloc(number_of_neurons * sizeof(neuron_unit_t));
	 
	for (int i = 0; i < number_of_neurons; i++)
	{
		layer.neurons[i] = neuron_construct(inputs_per_neuron);
	}

	return layer;
}

void neuron_layer_destruct(neuron_layer_t* layer) 
{
	for (int i = 0; i < layer->size; i++)
	{
		neuron_destruct(&layer->neurons[i]);
	}

	free(layer->neurons);
	layer->neurons = NULL;
	layer->num_weights = 0;
	layer->size = 0;
}

neuron_network_t neuron_network_construct(int num_inputs, int num_outputs, int num_layers, int layer_size)
{
	neuron_network_t network;
	network.num_inputs = num_inputs;
	network.num_outputs = num_outputs;
	network.num_layers = num_layers;
	network.layer_size = layer_size;

	network.layers = malloc((num_layers + 1) * sizeof (neuron_layer_t)); // Last = output row
	
	network.layers[0] = neuron_layer_construct(layer_size, num_inputs); // First layer recieves from inputs 

	for (int i = 1; i < num_layers; i++)
	{
		network.layers[i] = neuron_layer_construct(layer_size, layer_size);
	}

	network.layers[num_layers] = neuron_layer_construct(num_outputs, layer_size); // Last layer is output
	
	network.num_weights = neuron_network_get_num_weights(&network);

	return network;
}

void neuron_network_destruct(neuron_network_t* network) 
{
	for (int i = 0; i < network->num_layers; i++)
	{
		neuron_layer_destruct(&network->layers[i]);
	}

	free(network->layers);
	network->layers = NULL;
}

neuron_array_t neuron_network_get_weights(neuron_network_t* network) 
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
	
	for (int mid_layer = 0; mid_layer < layer_num-2; mid_layer++)
	{	
		int offset_mid_layer = first_layer_length + (mid_layer) * mid_layer_length;
		neuron_array_t mid_layer_weights = neuron_layer_get_weights(&network->layers[mid_layer + 1]);
		neuron_array_copy(&output_array, offset_mid_layer, &mid_layer_weights, 0, mid_layer_length);
	}
	
	neuron_array_t last_layer_weights = neuron_layer_get_weights(&network->layers[layer_num - 1]);
	int offset_last_layer = num_weights - last_layer_length;	
	neuron_array_copy(&output_array, offset_last_layer, &last_layer_weights, 0, last_layer_length);

	return output_array;
}

int neuron_network_get_num_weights(neuron_network_t* network)
{
	// int num = (network->num_inputs + network->num_outputs + (network->num_layers -1) * network->layer_size) * network->layer_size; // (in + out + num - 1 * size) * size

	int num_weights = 0;
	for (int i = 0; i < network->num_layers + 1; i++) {
		num_weights += network->layers[i].num_weights;
	}

	return num_weights;
}

void neuron_network_set_weights(neuron_network_t* network, neuron_array_t* weights)
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

neuron_array_t neuron_network_update(neuron_network_t* network, neuron_array_t* inputs)
{
	neuron_array_t outputs; // Stores outputs from each layer
	
	// For each layer
	for (int i = 0; i < network->num_layers + 1; i++)
	{
		int inputs_size;
		if (i > 0) 
		{
			inputs = &outputs; //save previous layers output;
			neuron_array_destruct(&outputs);
			inputs_size = network->layer_size;
		}
		else
		{
			 inputs_size = network->num_inputs;
		}

		// Initiate output for each layer
		int outputs_size = network->layers[i].size;
		outputs = neuron_array_construct(outputs_size);

		printf("Layer: %d Inputs size: %d Outputs size: %d\n", i, inputs_size, outputs_size);
		printf("Inputs: ");
		for (int k = 0; k < inputs->length; k++)
		{
			printf("%d: %f ", k, neuron_array_get(inputs, k));
		}
		puts("");


		for (int j = 0; j < outputs_size; j++)
		{
			double sum_inputs = 0;

			neuron_unit_t current_neuron = network->layers[i].neurons[j];

			if (network->layers[i].num_weights != inputs->length) 
			{
				puts("ERROR: inputs not equal to layer weights.");
				exit(1);
			}

			// Sum each weight * input
			for (int k = 0; k < current_neuron.weights.length; k++)
			{
				double current_input = neuron_array_get(inputs, k);
				double current_weight = neuron_array_get(&current_neuron.weights, k);
				sum_inputs += current_input * current_weight;
			}

			// Add in the bias
			sum_inputs += network->layers[i].neurons[j].bias;

			// Sigmoid for each neuron total
			 double sigmoid_output = neuron_sigmoid(sum_inputs);
			 neuron_array_set(&outputs, j, sigmoid_output);

			 printf("Out: %f -> Sigmoid: %f\n", sum_inputs, sigmoid_output);
		}
	}
	
	return outputs;
}

double neuron_network_get_sigmoid(neuron_network_t* network, double activation, double response);
