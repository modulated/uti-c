#ifndef _NEURON_H
#define _NEURON_H

typedef struct {
	double input;
	double thresh;
	double state;
	double output;
} neuron_t;

typedef struct {
	neuron_t* origin;
	neuron_t* destination;
	neuron_t* weighting;
} axon_t;

neuron_t neuron_create(double thresh);
void neuron_set(neuron_t* neuron, double input, double weight);
double neuron_get(neuron_t* neuron);
void neuron_clear(neuron_t* neuron);
double neuron_calc(neuron_t* neuron);

#endif
