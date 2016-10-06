#include "../include/ev.h"



// static void uuid_copy(uuid_t dst, const uuid_t src)
// {

// 	for (int i = 0; i < 16; i++)
// 		dst[i] = src[i];
// }



static void _destruct(message_t* this)
{
	free(this->data);
	uuid_clear(this->sender);
	uuid_clear(this->receiver);
}

char* message_action_parse(message_action_t action)
{
	switch (action) {
		case CREATED: return "CREATED"; break;
		case DELETED: return "DELETED"; break;
		case MOVED: return "MOVED"; break;
		
	}
}

static void _print(message_t* this)
{
	uuid_string_t uuid_sender;
	uuid_string_t uuid_receiver;
	uuid_unparse_upper(this->sender, uuid_sender);
	uuid_unparse_upper(this->receiver, uuid_receiver);

	printf("Sender: %s\nReceiver: %s\nAction: %s\nData: %s\n", uuid_sender, uuid_receiver, message_action_parse(this->action), this->data);
}

message_t message_construct(uuid_t sender, uuid_t receiver, message_action_t action, const char data[])
{
	message_t msg;
	uuid_copy(msg.sender, sender);
	uuid_copy(msg.receiver, receiver);
	msg.action = action;

	msg.print = _print;
	msg.destruct = _destruct;

	return msg;
}
