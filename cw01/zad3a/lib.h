#ifndef LIB_H
#define LIB_H

typedef struct {
    char **blocks;
    int size;
    int used;
} BlockArray;

BlockArray *block_array_init(int size);
int count_lines_words(BlockArray *blocks_array,const char *filename);
char *get_block_contents(BlockArray *blocks_array,int index);
int delete_block(BlockArray *blocks_array,int index);
int free_blocks_array(BlockArray *blocks_array);
#endif /* LIB_H */