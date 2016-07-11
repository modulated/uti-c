#ifndef _NEURON_H
#define _NEURON_H

typedef struct
{
	double* array;
	int length;
} neuron_array_t;

typedef struct 
{
	int inputs;
	neuron_array_t weights;
	double bias;
} neuron_t;

typedef struct 
{
	int size;
	neuron_t* neurons;
} neuron_layer_t;

typedef struct 
{
	int num_inputs;
	int num_outputs;
	int num_layers;
	int hidden_layer_size;
	neuron_layer_t* layers;
	int num_weights;
} neuron_network_t;




neuron_array_t neuron_array_construct(int length);
void neuron_array_destruct(neuron_array_t* array);
void neuron_array_set(neuron_array_t* array, int index, double value);
double neuron_array_get(neuron_array_t* array, int index);

neuron_t neuron_construct(int inputs);
void neuron_destruct(neuron_t* neuron);

neuron_layer_t neuron_layer_construct(int number_of_neurons, int inputs_per_neuron);
void neuron_layer_destruct(neuron_layer_t* layer);

neuron_network_t neuron_network_construct(int net_inputs, int net_outputs, int hidden_layer_num, int hidden_layer_size);
void neuron_network_destruct(neuron_network_t* network);
neuron_array_t neuron_network_get_weights(neuron_network_t* network);
int neuron_network_get_num_weights(neuron_network_t* network);
void neuron_network_set_weights(neuron_network_t* network, neuron_array_t* weights);
neuron_array_t neuron_network_update(neuron_network_t* network, neuron_array_t* inputs);

#endif
