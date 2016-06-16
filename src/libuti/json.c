#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"


enum JSON_ELEMENT_TYPE {
	JSON_NUMBER,
	JSON_STRING,
	JSON_BOOL,
	JSON_ARRAY,
	JSON_OBJECT
};

union JSON_DATA {
	char* string;
	int* num;
	char* bool;
	struct json_object* obj;
	struct json_array_container* array;
	
};

struct json_object;

typedef struct json_element {
	enum JSON_ELEMENT_TYPE type;
	char* name;
	union JSON_DATA data;
} json_element;

typedef struct json_object {
	int number;
	struct json_element** element_array; 
} json_object;

struct json_array_container;

typedef struct json_array_element {
	enum JSON_ELEMENT_TYPE type;
	union JSON_DATA data;
} json_array_element;

typedef struct json_array_container {
	int number;
	struct json_array_element* element;
} json_array_contaner;

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
