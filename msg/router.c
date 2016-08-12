#include "../include/message.h"

router_t* router_new()
{
	router_t* router = malloc(sizeof(router_t));

	
	return router;
}

void router_free(router_t* this)
{
	if (this) free(this);
	this = NULL;
}