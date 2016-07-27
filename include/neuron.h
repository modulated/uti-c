#ifndef _NEURON_H
#define _NEURON_H

#include <stddef.h> // size_t


// ===========================
// Public Structs
// ===========================

typedef struct
{
	double* array;
	size_t length;
} neuron_array_t;

typedef struct
{
	size_t inputs;
	neuron_array_t weights;
	double bias;
} neuron_unit_t;

typedef struct 
{
	size_t size;
	neuron_unit_t* neurons;
	size_t num_weights;
} neuron_layer_t;

typedef struct 
{
	size_t num_inputs;
	size_t num_outputs;
	size_t num_layers;
	size_t layer_size;
	size_t num_weights;
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
	size_t length;
} neuron_dataset_t;

// ===========================
// Array Functions
// ===========================

// Returns new array of given length.
neuron_array_t neuron_array_construct(size_t length);

// Cleans up array, sets members to NULL and 0. 
void neuron_array_destruct(neuron_array_t* array);

// Returns a new duplicate array.
neuron_array_t neuron_array_duplicate(const neuron_array_t* array);

// Copies *length* values from *src* starting at index_source to *dest* starting at *index_dest*.
void neuron_array_copy(neuron_array_t* dest, size_t index_dest, const neuron_array_t* src, size_t index_source, size_t length);

// Returns array crossed over at *index* with *start* and *end* respectively.
neuron_array_t neuron_array_crossover(const neuron_array_t* start, const neuron_array_t* end, size_t index);

// Returns a new array constructed from a slice of *src* starting at *index* of size *length*.
neuron_array_t neuron_array_slice(const neuron_array_t* src, size_t index, size_t length);

// Calculates the absolute sum of difference between values of two arrays. 
double neuron_array_difference(const neuron_array_t* array_a, const neuron_array_t* array_b);

// Gets a value from an *array* at *index* position. Performs bounds check.
double neuron_array_get(const neuron_array_t* array, size_t index);

// Sets a value of an *array* at *index* position. Performs bounds check.
void neuron_array_set(neuron_array_t* array, size_t index, double value);

// Prints length and values of *array* to stdout.
void neuron_array_print(const neuron_array_t* array);

// Returns result of rectified linear function of *input*, clamped to *max*.  
double neuron_relu (double input, double max);

// Returns result of sigmoid function of *input* with given *response* characteristic.
double neuron_sigmoid(double input, double response);


// ===========================
// Neuron Component Functions
// ===========================

// Returns neuron with *inputs*. Weights are 0.0 by default and bias is -1.0.
neuron_unit_t neuron_unit_construct(size_t inputs);

// Cleans up neuron. Sets inputs to 0. 
void neuron_unit_destruct(neuron_unit_t* neuron);

// Returns neuron layer with *number_of_neurons* and *inputs_per_neuron*.
neuron_layer_t neuron_layer_construct(size_t number_of_neurons, size_t inputs_per_neuron);

// Cleans up neuron layer. Sets members to NULL and 0. 
void neuron_layer_destruct(neuron_layer_t* layer);


// ===========================
// Dataset Functions
// ===========================

// Returns neuron IO pair containing expected *outputs* for given *inputs*. Must match neuron network parameters.
neuron_io_t neuron_io_construct(neuron_array_t* inputs, neuron_array_t* outputs);

// Cleans up neuron IO pair, sets members to NULL.
void neuron_io_destruct(neuron_io_t* io);

// Returns empty neuron IO dataset with expected network parameters.
neuron_dataset_t neuron_dataset_construct(size_t num_inputs, size_t num_outputs);

// Adds IO pair to *set*. Arrays must be of correct length. 
void neuron_dataset_add(neuron_dataset_t* set, double inputs[], double outputs[]);

// Cleans up dataset.
void neuron_dataset_destruct(neuron_dataset_t* set);


// ===========================
// Dataset Functions
// ===========================

// Returns network with *inputs*, outputs, *hidden_layer_num* layers of size *hidden_layer_size*. The default summing function is neuron_array_difference. 
neuron_network_t neuron_network_construct(size_t inputs, size_t outputs, size_t hidden_layer_num, size_t hidden_layer_size);

// Cleans up neuron network, including base components. Sets members to 0.
void neuron_network_destruct(neuron_network_t* network);

// Sets a custom summing function for updating.
void neuron_network_set_summing(neuron_network_t* network, double (*summing_function)(double input, double output));

// Returns array of all neuron weights.
neuron_array_t neuron_network_get_weights(const neuron_network_t* network);

// Sets weights of neuron inputs in *network* to given those in *weights*. Array size must match *network* weight number. 
void neuron_network_set_weights(neuron_network_t* network, const neuron_array_t* weights);

// Returns the number of weights in the *network*.
size_t neuron_network_size_weights(const neuron_network_t* network);

// Returns the number of biases in the *network*.
size_t neuron_network_size_bias(const neuron_network_t* network);

// Returns array of outputs after running network with current weights, biases and inputs. 
neuron_array_t neuron_network_update(const neuron_network_t* network, const neuron_array_t* inputs);


#endif
