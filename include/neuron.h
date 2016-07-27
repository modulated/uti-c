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
} neuron_unit_t;

typedef struct 
{
	int size;
	neuron_unit_t* neurons;
	int num_weights;
} neuron_layer_t;

typedef struct 
{
	int num_inputs;
	int num_outputs;
	int num_layers;
	int layer_size;
	int num_weights;
	double (*summing_function)(double, double);
	neuron_layer_t* layers;
} neuron_network_t;

typedef struct
{
	neuron_array_t input;
	neuron_array_t expected;
} neuron_io_t;

typedef struct
{
	neuron_io_t* data;
	int length;
} neuron_dataset_t;




neuron_array_t neuron_array_construct(int length);
void neuron_array_destruct(neuron_array_t* array);
neuron_array_t neuron_array_duplicate(neuron_array_t* array);
void neuron_array_copy(neuron_array_t* dest_array, int index_dest, neuron_array_t* source_array, int index_source, int length);
neuron_array_t neuron_array_crossover(neuron_array_t* array_start, neuron_array_t* array_end, int index);
neuron_array_t neuron_array_slice(neuron_array_t* dest_array, int index, int length);
double neuron_array_difference(neuron_array_t* const array_a, neuron_array_t* const array_b);
double neuron_array_get(neuron_array_t* array, int index);
void neuron_array_set(neuron_array_t* array, int index, double value);
void neuron_array_print(neuron_array_t* array);
double neuron_relu (double input, double max);
double neuron_sigmoid(double input, double response);

neuron_unit_t neuron_unit_construct(int inputs);
void neuron_unit_destruct(neuron_unit_t* neuron);

neuron_layer_t neuron_layer_construct(int number_of_neurons, int inputs_per_neuron);
void neuron_layer_destruct(neuron_layer_t* layer);

neuron_network_t neuron_network_construct(int net_inputs, int net_outputs, int hidden_layer_num, int hidden_layer_size, double (*summing_function)(double, double));
void neuron_network_destruct(neuron_network_t* network);
neuron_array_t neuron_network_get_weights(neuron_network_t* network);
int neuron_network_get_num_weights(neuron_network_t* network);
void neuron_network_set_weights(neuron_network_t* network, neuron_array_t* weights);
neuron_array_t neuron_network_update(neuron_network_t* network, neuron_array_t* inputs);

neuron_io_t neuron_io_construct(neuron_array_t* inputs, neuron_array_t* outputs);
void neuron_io_destruct(neuron_io_t* io);
neuron_dataset_t neuron_dataset_construct();
void neuron_dataset_push(neuron_dataset_t* set, int num_inputs, double inputs[], int num_outputs, double outputs[]);
void neuron_dataset_destruct(neuron_dataset_t* set);

#endif
