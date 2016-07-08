#include "../neuron.h"

static double neuron_relu(double input) {
	return input > 0 ? input : 0;
}

neuron_t neuron_create(double thresh) {
	neuron_t neuron;
	neuron.thresh = thresh;
	return neuron;
}

void neuron_set(neuron_t* neuron, double input, double weight) {
	neuron->input += input;
}

double neuron_get(neuron_t* neuron) {
	return neuron->output;
}

void neuron_clear(neuron_t* neuron) {
	neuron->input = 0;
	neuron->output = 0;
}

double neuron_calc(neuron_t* neuron) {
	neuron->output = neuron_relu(neuron->input + neuron->thresh);
	return neuron->output;
}

