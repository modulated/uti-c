#ifndef _JSON_H
#define _JSON_H

/*
** Data types
*/ 

enum JSON_ELEMENT_TYPE {
	JSON_NUMBER,
	JSON_STRING,
	JSON_BOOL,
	JSON_ARRAY,
	JSON_OBJECT
};

union JSON_DATA {
	char* string;
	int num;
	char bool;
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

json_object json_create_obj (int number_elements, json_element** element_array);

json_element json_create_element (enum JSON_ELEMENT_TYPE type, union JSON_DATA data, char* name);

#endif
