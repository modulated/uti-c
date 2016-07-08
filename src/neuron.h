#ifndef _NEURON_H
#define _NEURON_H

typedef struct 
{
	int inputs;
	double* weights;
} neuron_t;



typedef struct 
{
	int size;
	neuron_t* neurons;
} neuron_layer_t;

typedef struct 
{
	int net_inputs;
	int net_outputs;
	int hidden_layer_num;
	int hidden_layer_size;
	neuron_layer_t* layers;
} neuron_network_t;



neuron_t neuron_construct(int inputs);
void neuron_destruct(neuron_t* neuron);

neuron_layer_t neuron_layer_construct(int number_of_neurons, int inputs_per_neuron);
void neuron_layer_destruct(neuron_layer_t* layer);

neuron_network_t neuron_network_construct(int net_inputs, int net_outputs, int hidden_layer_num, int hidden_layer_size);
void neuron_network_destruct(neuron_network_t* network);
double* neuron_network_get_weights(neuron_network_t* network);
int neuron_network_get_num_weights(neuron_network_t* network);
void neuron_network_set_weights(neuron_network_t* network, double* weights);
double* neuron_network_update(neuron_network_t* network, double* inputs);
double neuron_network_get_sigmoid(neuron_network_t* network, double activation, double response);

#endif
