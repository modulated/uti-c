#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "json.h"

json_object json_create_obj (int number_elements, json_element** element_array) {
	json_object obj;
	obj.number = number_elements;
	obj.element_array = element_array;
	
	return obj;
}

json_element json_create_element (enum JSON_ELEMENT_TYPE type, union JSON_DATA data, char* name) {
	json_element element;
	
	element.type = type;
	element.data = data;
	element.name = name;
	
	return element;
}

// json_object json_parse (char* json) {
// 	json_object out {10,  };
	
// 	return out;
// }

