#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

#define SUCCESS  0
#define FAILURE  1

BlockArray* block_array_init(int size){
    BlockArray* blocks_array = (BlockArray *) calloc(1, sizeof(BlockArray));

    if (NULL == blocks_array)
    {
        fprintf(stderr, "Error: could not allocate memory for the array\n");
        return NULL;
    }

    blocks_array->blocks = (char **) calloc(size, sizeof(char *));

    if (NULL == blocks_array->blocks)
    {
        fprintf(stderr, "Error: could not allocate memory for the blocks in array\n");

        free(blocks_array);
        return NULL;
    }

    blocks_array->size = size;
    blocks_array->used = 0;

    return blocks_array;
}

int count_lines_words(BlockArray *blocks_array, const char *filename){
    // Czy na pewno można dodać następny blok?

    if ((blocks_array->used + 1) > blocks_array->size)
    {
        fprintf(stderr, "Error: no space left to put new blocks\n");
        return FAILURE;
    }

    // Tworzenie pliku tymczasowego

    char nameBuff[17];

    char *pont = strncpy( nameBuff, "/tmp/tmpF-XXXXXX", sizeof(nameBuff));

    nameBuff[sizeof(nameBuff) - 1] = '\0';

    if (pont == NULL)
    {
        fprintf(stderr, "Error: error in creating tmp file\n");
        return FAILURE;
    }

    int mks = mkstemp(nameBuff);

    if (mks < 0)
    {
        fprintf(stderr, "Error: error in adjusting name of tmp file to be uniqe\n");
        return FAILURE;
    }

    // Wywoływanie polecenia "wc", zapisanie wyniku w pliku tymczasowym

    char cmd[256];

    snprintf(cmd, sizeof(cmd),"wc '%s' > '%s'",filename,nameBuff);

    cmd[sizeof(cmd) - 1] = '\0';

    int ret = system(cmd);

    if (ret != 0)
    {
        fprintf(stderr, "Error: error in executing wc command\n");
        return -1;
    }

    // Odczyt danych (liczba wierszy i liczba słów) z pliku tymczasowego

    FILE *fp = fopen(nameBuff, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Error: error trying to open file\n");
        return -1;
    }

    int lines, words;
    int no = fscanf(fp, "%d %d", &lines, &words);
    fclose(fp);

    if (no != 2)
    {
        fprintf(stderr, "Error: not enough data in tmp file");
        return -1;
    }

    // Zapis danych do dynamicznego bloku pamięci

    int answer_len = snprintf(NULL,0,"L: %d, W: %d\n",lines,words);

    char *buffer = calloc(answer_len + 1, sizeof(char));

    snprintf(buffer,answer_len + 1,"L: %d, W: %d\n",lines, words);

    // Inkrementacja licznika ilośći zapisanych bloków

    blocks_array->blocks[blocks_array->used] = buffer;
    blocks_array->used += 1;

    // Usuniętie pliku tymczasowego

    remove(nameBuff);

    return SUCCESS;
}


char *get_block_contents(BlockArray *blocks_array,int index){
    if (NULL == blocks_array)
    {
        fprintf(stderr, "Error: array is null\n");
        return NULL;
    }

    if (index < 0 || index >= blocks_array->used)
    {
        fprintf(stderr, "Error: index out of range when accesing block contents\n");
        return NULL;
    }

    return blocks_array->blocks[index];
}


int delete_block(BlockArray *blocks_array, int index){
    if (blocks_array == NULL)
    {
        fprintf(stderr, "Error: array is null\n");
        return FAILURE;
    }

    if (index < 0 || index >= blocks_array->used)
    {
        fprintf(stderr, "Error: index out of range when deleting block\n");
        return FAILURE;
    }

    free(blocks_array->blocks[index]);

    for (int i = index + 1; i < blocks_array->used; i++)
    {
        blocks_array->blocks[i - 1] = blocks_array->blocks[i];
    }

    blocks_array->used -= 1;

    return SUCCESS;
}


int free_blocks_array(BlockArray *blocks_array){
    if (NULL == blocks_array)
    {
        fprintf(stderr, "Error: array is null\n");
        return FAILURE;
    }

    if (NULL != blocks_array->blocks)
    {
        for (int i = 0; i < blocks_array->used; i++)
        {
            if (NULL != blocks_array->blocks[i])
            {
                free(blocks_array->blocks[i]);
            }
        }

        free(blocks_array->blocks);
    }

    blocks_array->size = 0;
    blocks_array->used = 0;

    free(blocks_array);

    return SUCCESS;
}
