#include "../neuron.h"
#include <stdlib.h>
#include <stdio.h>

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

static double neuron_sigmoid(double in)
{
    double output;
       
    output = 1.0;
    return output;
}

static double neuron_sigmoid_adjusted(double in);

static double neuron_random_weight () // Double between 0.0 and 1.0
{
	return rand() / (RAND_MAX + 1.0);
}

static double neuron_random_range (double min, double max)
{
	return min + (rand() / (RAND_MAX + (max-min)));
}

static void neuron_network_weight_iterator(double* input_array, int offset, neuron_network_t* network);

/*
** Public Functions
*/

neuron_t neuron_construct (int inputs)
{
	neuron_t neuron;
	neuron.inputs = inputs;
	neuron.weights = malloc((inputs + 1) * sizeof(double));
	
	for (int i = 0; i < inputs + 1; i++) // First = Bias. Bias = -1 x Threshold 
	{
		neuron.weights[i] = neuron_random_weight();
		// printf("%d: %f\n", i, neuron.weights[i]);
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

	for (int i = 1; i < hidden_layer_num - 1; i++)
	{
		network.layers[i] = neuron_layer_construct(hidden_layer_size, hidden_layer_size);
	}

	network.layers[hidden_layer_num-1] = neuron_layer_construct(net_outputs, hidden_layer_size); // Last layer is output
	
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
    int layer_num = network->hidden_layer_num;
    int layer_size = network->hidden_layer_size;

    for (int i = 0; i < layer_size; i++)
    {
    //    for (int j = 0; j < inputs; j++)
    //    {
        int j = 0;
           array[i + j * inputs] = network->layers[0].neurons[i].weights[j];
            printf("i: %d j: %d x: %f\n", i, j, array[i + j * inputs]); 
    //    }
    }


    for (int k = 0; k < outputs; k++)
    {
        for (int l = 0; l < layer_size; l++)
        {
            array[k + l * layer_size] = network->layers[layer_num].neurons[k].weights[l];
            printf("k: %d l: %d x: %f\n", k, l, array[k + l * layer_size]);
        }
    }

    return array;
}

static void neuron_network_weight_iterator(double* input_array, int offset, neuron_network_t* network)
{
    for (int i = 0; i < n
}

int neuron_network_get_num_weights(neuron_network_t* network)
{
	int num = (network->net_inputs + network->net_outputs + (network->hidden_layer_num -1) * network->hidden_layer_size) * network->hidden_layer_size; // (in + out + num - 1) * size

	return num;
}

void neuron_network_set_weights(neuron_network_t* network, double* weights);
double* neuron_network_update(neuron_network_t* network, double* inputs);
double neuron_network_get_sigmoid(neuron_network_t* network, double activation, double response);
