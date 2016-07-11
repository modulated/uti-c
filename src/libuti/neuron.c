#include "../neuron.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../data.h"

#define BIAS -1
#define SIG_RES 0.5

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
	return 1.0 / (1.0 + exp(-input * SIG_RES));
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

static void neuron_network_weight_iterator_read(double* input_array, neuron_network_t* network, int offset, int layer, int num_neurons, int num_inputs)
{
	for (int neuron = 0; neuron < num_neurons; neuron++)
	{
		for (int input = 0; input < num_inputs; input++)
		{
			// printf("neuron: %d input: %d x: %f\n", neuron, input, network->layers[layer].neurons[neuron].weights[input]);
			input_array[offset + (neuron * num_inputs) + input] = network->layers[layer].neurons[neuron].weights[input];
		}
	}
}

static void neuron_network_weight_iterator_write(double* input_array, neuron_network_t* network, int offset, int layer, int num_neurons, int num_inputs)
{
	for (int neuron = 0; neuron < num_neurons; neuron++)
	{
		for (int input = 0; input < num_inputs; input++)
		{
			network->layers[layer].neurons[neuron].weights[input] = input_array[offset + (neuron * num_inputs) + input]; 
			// printf("neuron: %d input: %d x: %f\n", neuron, input, network->layers[layer].neurons[neuron].weights[input]);
		}
	}
}


/*
** Public Functions
*/

neuron_t neuron_construct (int inputs)
{
	neuron_t neuron;
	neuron.inputs = inputs;
	neuron.weights = malloc((inputs + 1) * sizeof(double));
	
	for (int i = 0; i < inputs + 1; i++) // Last weight is Bias. Bias = -1 x Threshold 
	{
		neuron.weights[i] = neuron_random_weight();
		// printf("Weight %d: %f\n", i, neuron.weights[i]);
	}

	return neuron;
}

void neuron_destruct (neuron_t* neuron)
{
	free(neuron->weights);
	neuron->weights = NULL;
}

neuron_layer_t neuron_layer_construct(int number_of_neurons, int inputs_per_neuron)
{
	neuron_layer_t layer;
	layer.size = number_of_neurons;
	layer.neurons = malloc(number_of_neurons * sizeof(neuron_t));

	for (int i = 0; i < number_of_neurons; i++)
	{
		layer.neurons[i] = neuron_construct(inputs_per_neuron);
	}

	return layer;
}

void neuron_layer_destruct(neuron_layer_t* neuron_layer) 
{
	for (int i = 0; i < neuron_layer->size; i++)
	{
		neuron_destruct(&neuron_layer->neurons[i]);
	}

	free(neuron_layer->neurons);
	neuron_layer->neurons = NULL;
}

neuron_network_t neuron_network_construct(int net_inputs, int net_outputs, int hidden_layer_num, int hidden_layer_size)
{
	neuron_network_t network;
	network.net_inputs = net_inputs;
	network.net_outputs = net_outputs;
	network.hidden_layer_num = hidden_layer_num;
	network.hidden_layer_size = hidden_layer_size;

	network.layers = malloc((hidden_layer_num + 1) * sizeof (neuron_layer_t)); // Last = output row
	
	network.layers[0] = neuron_layer_construct(hidden_layer_size, net_inputs); // First layer recieves from inputs 

	for (int i = 1; i < hidden_layer_num; i++)
	{
		network.layers[i] = neuron_layer_construct(hidden_layer_size, hidden_layer_size);
	}

	network.layers[hidden_layer_num] = neuron_layer_construct(net_outputs, hidden_layer_size); // Last layer is output
	
	return network;
}

void neuron_network_destruct(neuron_network_t* network) 
{
	for (int i = 0; i < network->hidden_layer_num; i++)
	{
		neuron_layer_destruct(&network->layers[i]);
	}

	free(network->layers);
	network->layers = NULL;
}

double* neuron_network_get_weights(neuron_network_t* network) 
{
	int num_weights = neuron_network_get_num_weights(network);
	double* array = malloc(num_weights * sizeof(double));
	int inputs = network->net_inputs;
	int outputs = network->net_outputs;
	int layer_num = network->hidden_layer_num + 1; // Add output layer
	int layer_size = network->hidden_layer_size;

	int first_layer_length = inputs * layer_size;
	int middle_layer_length = layer_size * layer_size;
	int last_layer_length = outputs * layer_size;
	
	neuron_network_weight_iterator_read(array, network, 0, 0, layer_size, inputs); // First layer
	
	for (int mid_layer = 0; mid_layer < layer_num-2; mid_layer++) // Middle layers
	{	
		int offset_mid_layer = first_layer_length + (mid_layer) * middle_layer_length;
		// printf("Mid layer: %d, offset: %d\n", mid_layer+1, offset_mid_layer);
		neuron_network_weight_iterator_read(array, network, offset_mid_layer, mid_layer+1, layer_size, layer_size);
	}
	// printf("Last layer: %d, offset: %d\n", layer_num-1, num_weights - last_layer_length);
	neuron_network_weight_iterator_read(array, network, num_weights - last_layer_length, layer_num-1, outputs, layer_size); //Last layer

	return array;
}

int neuron_network_get_num_weights(neuron_network_t* network)
{
	int num = (network->net_inputs + network->net_outputs + (network->hidden_layer_num -1) * network->hidden_layer_size) * network->hidden_layer_size; // (in + out + num - 1 * size) * size

	return num;
}

void neuron_network_set_weights(neuron_network_t* network, double* weights)
{
	int num_weights = neuron_network_get_num_weights(network);
	for (int i = 0; i < num_weights; i++)
	{
		if (weights[i] > 1.0 || weights[i] < 0.0) 
		{
			puts("ERRROR: illegal weight.");
			exit(1);
		}
	}

	int inputs = network->net_inputs;
	int outputs = network->net_outputs;
	int layer_num = network->hidden_layer_num + 1; // Add output layer
	int layer_size = network->hidden_layer_size;

	int first_layer_length = inputs * layer_size;
	int middle_layer_length = layer_size * layer_size;
	int last_layer_length = outputs * layer_size;
	
	neuron_network_weight_iterator_write(weights, network, 0, 0, layer_size, inputs); // First layer
	
	for (int mid_layer = 0; mid_layer < layer_num-2; mid_layer++) // Middle layers
	{	
		int offset_mid_layer = first_layer_length + (mid_layer) * middle_layer_length;
		// printf("Mid layer: %d, offset: %d\n", mid_layer+1, offset_mid_layer);
		neuron_network_weight_iterator_write(weights, network, offset_mid_layer, mid_layer+1, layer_size, layer_size);
	}
	// printf("Last layer: %d, offset: %d\n", layer_num-1, num_weights - last_layer_length);
	neuron_network_weight_iterator_write(weights, network, num_weights - last_layer_length, layer_num-1, outputs, layer_size); //Last layer

}

double* neuron_network_update(neuron_network_t* network, double* inputs)
{
	double* outputs; // Stores outputs from each layer
	
	// For each layer
	for (int i = 0; i < network->hidden_layer_num + 1; i++)
	{
		int inputs_size;
		if (i > 0) 
		{
			inputs = outputs; //save previous layers output;
			free(outputs);
			inputs_size = network->hidden_layer_size;
		}
		else
		{
			 inputs_size = network->net_inputs;
		}

		// For each neuron
		int outputs_size = network->layers[i].size;
		
		outputs = malloc(outputs_size * sizeof(double));

		printf("Layer: %d Inputs size: %d Outputs size: %d\n", i, inputs_size, outputs_size);
		printf("Inputs: ");
		for (int k = 0; k < inputs_size; k++)
		{
			printf("%d: %f ", k, inputs[k]);
		}
		puts("");


		for (int j = 0; j < outputs_size; j++)
		{
			double netinput = 0;
			int num_inputs = network->layers[i].neurons[j].inputs;

			// Sum each weight * input
			for (int k = 0; k < num_inputs; k++)
			{
				netinput += network->layers[i].neurons[j].weights[k] * inputs[k];
			}

			// Add in the bias
			netinput += network->layers[i].neurons[j].weights[num_inputs] * BIAS;

			// Sigmoid for each neuron total
			outputs[j] = neuron_sigmoid(netinput);
		}
	}
	
	return outputs;
}

double neuron_network_get_sigmoid(neuron_network_t* network, double activation, double response);
