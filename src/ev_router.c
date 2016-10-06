#include "ev.h"

int router_subscribe(router_t* this, entity_t* entity)
{
	return EV_OK;
}

router_t* router_new()
{
	router_t* router = malloc(sizeof(router_t));
	router->mqueue_size = 0;
	router->receiver_size = 0;
	router->subscribe = router_subscribe;
	
	return router;
}

void router_free(router_t** this)
{
	if (*this) free(*this);
	*this = NULL;
}
