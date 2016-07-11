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

static void neuron_network_weight_iterator_read(neuron_array_t* output_array, neuron_network_t* network, int offset, int layer, int num_neurons, int num_inputs)
{
	if (output_array->length != network->layers[layer].size * network->layers[layer].neurons->inputs)
	{
		puts("ERROR: input weight length not matching network weight length");
		exit(1);
	}

	for (int neuron = 0; neuron < num_neurons; neuron++)
	{
		neuron_array_t* neuron_weights =  &network->layers[layer].neurons[neuron].weights;

		for (int input = 0; input < num_inputs; input++)
		{
			// printf("neuron: %d input: %d x: %f\n", neuron, input, network->layers[layer].neurons[neuron].weights[input]);
			int index = offset + (neuron * num_inputs) + input;
			double neuron_weight = neuron_array_get(neuron_weights, index);
			neuron_array_set(output_array, index, neuron_weight); 
			
		}
	}
}

static void neuron_network_weight_iterator_write(neuron_array_t* input_array, neuron_network_t* network, int offset, int layer, int num_neurons, int num_inputs)
{
	if (input_array->length != network->num_weights)
	{
		puts("ERROR: input weight length not matching network weight length");
		exit(1);
	}

	for (int neuron = 0; neuron < num_neurons; neuron++)
	{
		neuron_array_t* neuron_weights =  &network->layers[layer].neurons[neuron].weights;

		for (int input = 0; input < num_inputs; input++)
		{
			int index = offset + (neuron * num_inputs) + input;
			double input_weight = neuron_array_get(input_array, index);
			neuron_array_set(neuron_weights,index, input_weight); 
			// printf("neuron: %d input: %d x: %f\n", neuron, input, network->layers[layer].neurons[neuron].weights[input]);
		}
	}
}


/*
** Public Functions
*/

neuron_array_t neuron_array_construct(int length) 
{
	neuron_array_t array;
			
	array.length = length;

	array.array = malloc(length + 1);
	array.array[length] = NAN;

	return array;
}

void neuron_array_set(neuron_array_t* array, int index, double value)
{
	if (index >= array->length) {
		puts("ERROR: array overflow.");
		exit(1);
	}
	array->array[index] = value;
}

double neuron_array_get(neuron_array_t* array, int index)
{
	if (index >= array->length) {
		puts("ERROR: array overflow.");
		exit(1);
	}
	return array->array[index];
}

void neuron_array_destruct(neuron_array_t* array)
{
	free(array->array);
	array->array = NULL;
	array->length = 0;
}

neuron_t neuron_construct (int inputs)
{
	neuron_t neuron;
	neuron.inputs = inputs;
	neuron.weights = neuron_array_construct(inputs);
	
	for (int i = 0; i < inputs; i++) // Last weight is Bias. Bias = -1 x Threshold 
	{
		neuron_array_set(&neuron.weights, i, neuron_random_weight());
		// printf("Weight %d: %f\n", i, neuron.weights[i]);
	}

	neuron.bias = neuron_random_weight() * -1;

	return neuron;
}

void neuron_destruct (neuron_t* neuron)
{
	neuron_array_destruct(&neuron->weights);	
	neuron->inputs = 0;
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

void neuron_layer_destruct(neuron_layer_t* layer) 
{
	for (int i = 0; i < layer->size; i++)
	{
		neuron_destruct(&layer->neurons[i]);
	}

	free(layer->neurons);
	layer->neurons = NULL;
}

neuron_network_t neuron_network_construct(int num_inputs, int num_outputs, int num_layers, int hidden_layer_size)
{
	neuron_network_t network;
	network.num_inputs = num_inputs;
	network.num_outputs = num_outputs;
	network.num_layers = num_layers;
	network.hidden_layer_size = hidden_layer_size;

	network.layers = malloc((num_layers + 1) * sizeof (neuron_layer_t)); // Last = output row
	
	network.layers[0] = neuron_layer_construct(hidden_layer_size, num_inputs); // First layer recieves from inputs 

	for (int i = 1; i < num_layers; i++)
	{
		network.layers[i] = neuron_layer_construct(hidden_layer_size, hidden_layer_size);
	}

	network.layers[num_layers] = neuron_layer_construct(num_outputs, hidden_layer_size); // Last layer is output
	
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
	int layer_size = network->hidden_layer_size;

	int first_layer_length = inputs * layer_size;
	int middle_layer_length = layer_size * layer_size;
	int last_layer_length = outputs * layer_size;
	
	neuron_network_weight_iterator_read(&output_array, network, 0, 0, layer_size, inputs); // First layer
	
	for (int mid_layer = 0; mid_layer < layer_num-2; mid_layer++) // Middle layers
	{	
		int offset_mid_layer = first_layer_length + (mid_layer) * middle_layer_length;
		// printf("Mid layer: %d, offset: %d\n", mid_layer+1, offset_mid_layer);
		neuron_network_weight_iterator_read(&output_array, network, offset_mid_layer, mid_layer+1, layer_size, layer_size);
	}
	// printf("Last layer: %d, offset: %d\n", layer_num-1, num_weights - last_layer_length);
	neuron_network_weight_iterator_read(&output_array, network, num_weights - last_layer_length, layer_num-1, outputs, layer_size); //Last layer

	return output_array;
}

int neuron_network_get_num_weights(neuron_network_t* network)
{
	int num = (network->num_inputs + network->num_outputs + (network->num_layers -1) * network->hidden_layer_size) * network->hidden_layer_size; // (in + out + num - 1 * size) * size

	return num;
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

	int inputs = network->num_inputs;
	int outputs = network->num_outputs;
	int layer_num = network->num_layers + 1; // Add output layer
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
			inputs_size = network->hidden_layer_size;
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
		for (int k = 0; k < inputs_size; k++)
		{
			printf("%d: %f ", k, inputs->array[k]);
		}
		puts("");


		for (int j = 0; j < outputs_size; j++)
		{
			double sum_inputs = 0;
			int num_inputs = network->layers[i].neurons[j].inputs;

			neuron_t current_neuron = network->layers[i].neurons[j];
			// Sum each weight * input
			for (int k = 0; k < num_inputs; k++)
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

			 printf("Sum: %f -> Sigmoid: %f\n", sum_inputs, sigmoid_output);
		}
	}
	
	return outputs;
}

double neuron_network_get_sigmoid(neuron_network_t* network, double activation, double response);
