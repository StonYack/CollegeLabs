//**************************************************
// Linked list implementation
//

#include <stdlib.h>
#include <pthread.h>
#include "list.h"

typedef struct item_s
{
	int64_t data;
	struct item_s *next;
	struct item_s *prev;
} item_t;

typedef struct list_s
{
	pthread_mutex_t lock;
	item_t* head;
	item_t* tail;
	int64_t count;
} list_t;

// Create and initialize a list. 
// Return pointer to list. Return NULL on failure.
linked_list_t Init_List()
{
	list_t* list = (list_t*) malloc(sizeof(list_t));
	pthread_mutex_init(&list->lock, NULL);
	list->count = 0;
	list->head = NULL;
	list->tail = NULL;

	return list;
}

// Delete a list are free all memory used by the list
// It is erroneous to use the list pointer after caling this routine.
// Return zero on success
int Delete_List(linked_list_t list)
{
    int ret = 0;
    list_t * core = (list_t *) list;
    if(core != NULL)
    {
         item_t * travel = (core->head);
         item_t * trail = NULL;
         while(travel != NULL)
         {
         	trail = travel;
                travel = travel->next;
                free(trail);
                trail = NULL;
                core->count--;
         }
         travel = NULL;
         if(core->count == 0)
         {
	        ret = 1;
         }
         free(core);
   }
   return ret;
}
// Returns the number of items in the list
int64_t Count(linked_list_t list)
{
	list_t * core = (list_t *) list;	
	return core->count;
}

// Insert an item at the beginning of the list
// Return zero on success
// Params:
//    list: list to add item to
//    data: Data to be added to the list
int Insert_At_Beginning(linked_list_t list, int64_t data)
{
	int ret = 1;
	list_t * core = (list_t *) list;
	pthread_mutex_lock(&core->lock);
	item_t * travel = core->head;
	item_t * new = malloc(sizeof(item_t));
	new->data = data;
	new->prev = NULL;
	new->next = NULL;
	if(core->head != NULL)
	{
		travel->prev = new;
		core->head = new;
		new->prev = NULL;
		new->next = travel;
		travel = NULL;
		ret = 0;
	}
	else
	{	
		core->tail = new;
		core->head = new;
		ret = 0;
	}
	core->count++; 
	pthread_mutex_unlock(&core->lock);
	return ret;
}

// Insert an item at the end of the list
// Return zero on success
// Params:
//    list: list to add item to
//    data: Data to be added to the list
int Insert_At_End(linked_list_t list, int64_t data)
{
	int ret = 1;
	list_t * core = (list_t *) list;
	pthread_mutex_lock(&core->lock);
	item_t * travel = core->tail;
	item_t * new = malloc(sizeof(item_t));
	new->data = data;
	if(core->tail != NULL)
	{
		travel->next = new;
		core->tail = new;
		new->next = NULL;
		new->prev = travel;
		travel = NULL;
		ret = 0;	
	}
	else 
	{
		core->head = new;
		core->tail = new;
		ret = 0;
	}
	core->count++;
	pthread_mutex_unlock(&core->lock);
	return ret;
}

// Return true (non-zero) if the list is empty
//    list: list to examine
int Empty(linked_list_t list)
{
	int ret = 0;
	list_t * core = (list_t *) list;
	if(core->head == NULL)
	{
		ret = 1;
	}
	return ret;
}

// Remove an item from the beginning of the list 
// Return zero on success
// Params:
//    list: list to remove item from
//    data: pointer to location to store data of removed item
//          if data is NULL, data is not returned
int Remove_From_Beginning(linked_list_t list, int64_t* data)
{
	int ret	= 0;
	list_t * core = (list_t *) list;
	pthread_mutex_lock(&core->lock);
	if(!Empty(list))
	{	
		if (data != NULL)
		{
			*data = core->head->data;
		}
		item_t * temp = (item_t *)core->head; 
		core->head = core->head->next;
		free(temp);
		core->count--;
		if(Count(list) == 0)
		{	
			core->head = NULL;
			core->tail = NULL;
		}
	}

	else
	{
		ret = 1;
	}
	pthread_mutex_unlock(&core->lock);
	return ret;
}

// Remove an item from the end of the list 
// Return zero on success
// Params:
//    list: list to remove item from
//    data: pointer to location to store data of removed item
//          if data is NULL, data is not returned
int Remove_From_End(linked_list_t list, int64_t* data)
{	
	int ret	= 0;
	list_t * core = (list_t *) list;
	pthread_mutex_lock(&core->lock);
	if(!Empty(list))
	{	
		if (data != NULL)
		{
			*data = core->tail->data;
		}
		item_t * temp = (item_t *) core->tail; 
		core->tail = core->tail->prev;
		free(temp);
		core->count--;
		if(Count(list) == 0)
		{	
			core->head = NULL;
			core->tail = NULL;
		}
	}

	else
	{
		ret = 1;
	}
	pthread_mutex_unlock(&core->lock);
	return ret;
}

int Traverse(linked_list_t list, void (*test_action)(int64_t data))
{
	int ret = 0;
	list_t * core = (list_t *) list;
	item_t * current  = core->head;

	if (current != NULL)
	{
		for(int ii = 0; ii < core->count; ii++)
		{	
			test_action(current->data);
			current = current->next;
		}	
	}
	else
	{
		ret = 1;
	}
	return ret;
}

int Insert_In_Order(linked_list_t list, int64_t value)
{
	int ret = 1;
	list_t * core = (list_t *) list;

	pthread_mutex_unlock(&core->lock);
	if(Empty(list))
	{

		Insert_At_Beginning(list, value);
		pthread_mutex_lock(&core->lock);
		ret = 0;
	}
	else if (value < core->head->data)
	{
		Insert_At_Beginning(list, value);
		pthread_mutex_unlock(&core->lock);
		ret = 0;
	}

	else if(value > core->tail->data)
	{
		Insert_At_End(list, value);	
		pthread_mutex_unlock(&core->lock);
		ret = 0;
	}
	else
	{	
		pthread_mutex_lock(&core->lock);
		item_t * new = (item_t *) malloc(sizeof(item_t));
		new->data = value;
		item_t * travel = core->head;
		int ii = 0;
		while (ii < Count(list) && value > travel->data)
		{
			travel = travel->next;
			++ii;
		}
		new->prev = travel->prev;
		new->next = travel;
		travel->prev->next = new;
		travel->prev = new;
		core->count++;
		ret = 0;
	}
	return ret;
}


