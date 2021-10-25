#include <stdio.h>
#include <stdlib.h>

#include "list.h"

//******************************************
//

void test_action(int data)
{
    printf("This item in the list holds the data %d\n", data);
    return;	
}
int main(void)
{
    linked_list_t list;
    int ii;
    int value;
    int error;

    printf("Checking stack behavior\n");
    list = Init_List();
    if (list == NULL)
    {
        printf("Unable to initialize list\n");
        return 1;
    }

   for (ii=0; ii<10; ii++)
    {
        if (Insert_At_End(list, ii) != 0)
        {
            printf("Error inserting %d\n", ii);
        }
    }
    for (ii=0; ii<10; ii++)
    {
	if(Remove_From_Beginning(list, &ii) != 0)
	{
		printf("Error Removing %d\n", ii);
		error = 1;
	}
    }
    if(error != 0)
    {
	printf("Stack Operation Verified\n");
    }

    for (ii = 0; ii <= 20; ii++)
    {
	int data = rand() % 1000;
   	if(Insert_In_Order(list, data) == 0)
	{
		printf("%d is now inserted into the list\n", data);	
        }
	else
	{
		printf("%d did not get inserted\n", data);
	}
    }
    Traverse(list, test_action); 


    for (ii=Count(list) - 1; ii>=0; ii--)
    {
        if (Remove_From_Beginning(list, &value) != 0)
        {
            printf("Failed to remove item\n");
        }
    }

    if (!Empty(list)) 
    {
        printf("List is not empty after removes\n");
    }

    Delete_List(list);

    printf("Tests complete\n");

    return 0;
}

    























