#include <stdio.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include <string.h>
#include "hashmap.h"

#define MAX_QUEUE_SIZE 16
#define MAX_RECEIVER_SIZE 32

typedef void* entity_t;

typedef enum
{
	CREATED,
	DELETED,
	MOVED
} message_action_t;

typedef struct message_s message_t;

struct message_s
{
	uuid_t sender;
	uuid_t receiver;
	message_action_t action;
	char* data;
	void (*print)(message_t* msg);
	void (*destruct)(message_t* msg);
};

typedef struct
{	
	message_t mqueue[MAX_QUEUE_SIZE];
	size_t mqueue_size;

	map_t receiver_map[MAX_RECEIVER_SIZE];
	size_t receiver_size;

	
} router_t;

message_t message_construct(uuid_t sender, uuid_t receiver, message_action_t action, const char data[]);
router_t* router_construct();