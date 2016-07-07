#ifndef _NEURAL_H
#define _NEURAL_H

typedef struct {
	double input;
	double bias;
	double state;
	double output;
} neuron_t;

neuron_t neuron_create(double bias);
void neuron_set(neuron_t* neuron, double input, double weight);
double neuron_get(neuron_t* neuron);
void neuron_clear(neuron_t* neuron);
double neuron_calc(neuron_t* neuron);

#endif
