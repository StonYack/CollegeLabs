#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <pthread.h>
#include "list.h"
#include "sequence.h"

typedef struct thread_s
{
    pthread_mutex_t lock;
    linked_list_t list;
    int64_t items;
} thread_t;

void test_action(int64_t data)
{
    printf("This item in the list holds the data %ld\n", data);
    return;
}

void * mythread(void * p_data)
{  
    thread_t * t_data = (thread_t *) p_data;
    int64_t data;
    printf("thread data: %ld (%ld)\n", pthread_self(), t_data->items);
    for (int ii = 0; ii < t_data->items; ii++)
    {
	pthread_mutex_lock(&t_data->lock);
	printf("floof\n");
	data = Next_In_Sequence();
       	Insert_In_Order(t_data->list, data);
	pthread_mutex_unlock(&t_data->lock);
	printf("data: %ld\n", data);	
    }
    return p_data;
}
//*************************************************
void Fatal_Error(char *msg)
{
    fprintf(stderr, msg);
    exit(-1);
}
//*************************************************
void Check_Item(int64_t value)
{
    static int64_t last_item = INT64_MIN;

   if (last_item != INT64_MIN && last_item+1 != value)
    {
        printf("Items not in sequence: %ld %ld\n", last_item, value);
    }
    last_item = value;

    // Uncomment the following if you want to see the data
    //printf("Item: %ld\n", value);
}
//*************************************************
int main(int argc, char** argv)
{
    thread_t *t_data = (thread_t *)malloc(sizeof(thread_t));
    int opt;                    // command line arg
    int ii;                     // for index
    int64_t num_to_insert = 20;
    int num_threads = 1;
    void * ptr;
    pthread_mutex_init(&t_data->lock, NULL);
    while ((opt = getopt(argc, argv, "hn:t:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf("%s -h -n <inserts/thread> -t <num threads>\n", argv[0]);
                break;
            case 'n':
                num_to_insert = atol(optarg);
                if (num_to_insert < 0) 
                    Fatal_Error("-n must specify a value > 0");
                break;
            case 't':
                num_threads = atoi(optarg);
                if (num_threads < 1)
                    Fatal_Error("-t must specify a value > 0");
                break;
        }
    }

    printf("Creating sorted list with %ld elements\n", num_to_insert);
    t_data->list = Init_List();
    t_data->items = num_to_insert;
    printf("not failed yet\n");
    pthread_t thread[num_threads];
    for (ii = 0; ii < num_threads; ii++)
    {
    	pthread_create(&thread[ii], NULL, mythread, t_data); 
    }
    for (ii = 0; ii < num_threads; ii++)
    {
 	pthread_join(thread[ii], &ptr);
    }
    
    printf("Checking sorted list\n");
    Traverse(t_data->list, Check_Item);

    if (Count(t_data->list) != num_to_insert * num_threads) printf("List is not tracking size\n");
    Delete_List(t_data->list);
    free(t_data);
    pthread_exit(NULL);
    return 0;
}

