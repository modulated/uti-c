#include <stdio.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include <string.h>
#include "hashmap.h"

#define MAX_QUEUE_SIZE 16
#define MAX_RECEIVER_SIZE 32

// RETURN CODES
#define EV_OK 0
#define EV_MEM -1
#define EV_MIS -2


typedef enum
{
	CREATED,
	DELETED,
	MOVED
} message_action_t;


typedef struct message_s message_t;
typedef struct router_s router_t;


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
	int id;
	router_t* router;

} entity_t;


struct router_s
{	
	message_t mqueue[MAX_QUEUE_SIZE];
	size_t mqueue_size;

	map_t receiver_map[MAX_RECEIVER_SIZE];
	size_t receiver_size;

	int (*subscribe)(router_t* this, entity_t* entity);

	
} ;

message_t message_new(uuid_t sender, uuid_t receiver, message_action_t action, const char data[]);

router_t* router_new();
void router_free();
