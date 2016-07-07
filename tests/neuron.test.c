#include "tap.h"
#include "../src/neuron.h"
#include "math.h"
#include "float.h"

#define abs(a) a => 0 ? a : -a
#define approx(a, b) (fabs(a - b) <= FLT_EPSILON)

void test_neuron_create() {
	neuron_t test_neuron = neuron_create(1.0);
	ok( approx(test_neuron.bias, 1.0) && approx(test_neuron.output, 0.0), "neuron_create returns expected values.") || 
	diag("Weight expected %f, got %f. Output expected %f, got %f", 1.0, test_neuron.bias, 0.0, test_neuron.output);
}

void test_neuron_set() {
	neuron_t test_neuron = neuron_create(1.0);
	neuron_set(&test_neuron, 1.0, 1.0);
	neuron_set(&test_neuron, 2.0, 1.0);

	ok( approx(test_neuron.bias, 1.0) && approx(test_neuron.input, 3.0), "neuron_set returns expected values.") || 
	diag("Weight expected %f, got %f. Output expected %f, got %f", 1.0, test_neuron.bias, 3.0, test_neuron.output);
}

void test_neuron_get() {
	neuron_t test_neuron = neuron_create(1.0);
	double val = neuron_get(&test_neuron);

	ok(approx(val,0.0), "neuron_get returns expected value.") || 
	diag("Output expected %f, got %f.", 0.0, val);
}

void test_neuron_calc() {
	double bias = 1.0;
	neuron_t test_neuron = neuron_create(bias);
	double val = neuron_calc(&test_neuron);

	ok(approx(val, bias), "neuron_get returns expected value.") || 
	diag("Output expected %f, got %f.", bias, val);
}

void test_all() {
	test_neuron_create();
	test_neuron_set();
	test_neuron_get();
}

int main(void) {
	plan();
	test_all();

	return exit_status();
}
