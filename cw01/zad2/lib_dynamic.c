#include <stdio.h>
#include <stdlib.h>
#include "lib_dynamic.h"

#define SUCCESS  0
#define FAILURE  1

static void *handle = NULL;

BlockArray *(*_block_array_init) (int);

int (*_count_lines_words) (BlockArray *,const char *);

char *(*_get_block_contents) (BlockArray *, int);

int (*_delete_block) (BlockArray *,int);

int (*_free_blocks_array) (BlockArray *);

BlockArray *block_array_init(int size_of_array)
{
    return (*_block_array_init)(size_of_array);
}

int count_lines_words(BlockArray *array,const char *filename)
{
    return (*_count_lines_words)(array, filename);
}

char *get_block_contents(BlockArray* arr,int index)
{
    return (*_get_block_contents)(arr, index);
}

int delete_block(BlockArray *arr,int index)
{
    return (*_delete_block)(arr, index);
}

int free_blocks_array(BlockArray *blocks_array)
{
    return (*_free_blocks_array)(blocks_array);
}

/**************************************************************/

int init_dynamic_handler(void)
{
    handle = dlopen("./libblockarray.so", RTLD_LAZY);

    if (handle == NULL)
    {
        fprintf(stderr, "ERROR WHILE LOADING DYNAMIC LIBRARY\n");
        return FAILURE;
    }

    printf("SUCCESSFUL LOADING OF DYNAMIC LIBRARY\n");

    *((void **) &(_block_array_init)) = dlsym(handle, "block_array_init");

    if (dlerror() != NULL)
    {
        fprintf(stderr, "COULD NOT LOAD block_array_init METHOD\n");

        free_handler_memory();
        return FAILURE;
    }

    *((void **) &(_count_lines_words)) = dlsym(handle, "count_lines_words");

    if (dlerror() != NULL)
    {
        fprintf(stderr, "COULD NOT LOAD count_lines_words METHOD\n");

        free_handler_memory();
        return FAILURE;
    }

    *((void **) &(_get_block_contents)) = dlsym(handle, "get_block_contents");

    if (dlerror() != NULL)
    {
        fprintf(stderr, "COULD NOT LOAD get_block_contents METHOD\n");

        free_handler_memory();
        return FAILURE;
    }

    *((void **) &(_delete_block)) = dlsym(handle, "delete_block");

    if (dlerror() != NULL)
    {
        fprintf(stderr, "COULD NOT LOAD delete_block METHOD\n");

        free_handler_memory();
        return FAILURE;
    }

    *((void **) &(_free_blocks_array)) = dlsym(handle, "free_blocks_array");

    if (dlerror() != NULL)
    {
        fprintf(stderr, "COULD NOT LOAD free_blocks_array METHOD\n");

        free_handler_memory();
        return FAILURE;
    }

    return SUCCESS;
}

void free_handler_memory(void)
{
    dlclose(handle);
}

