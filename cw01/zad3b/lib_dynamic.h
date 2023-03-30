#ifndef LIB_DYNAMIC_H
#define LIB_DYNAMIC_H

#include <dlfcn.h>
#include "lib.h"

extern BlockArray *(*_block_array_init) (int);
extern int (*_count_lines_words) (BlockArray *,const char *);
extern char *(*_get_block_contents) (BlockArray *,int);
extern int (*_delete_block) (BlockArray *, int);
extern int (*_free_blocks_array) (BlockArray *);
extern int init_dynamic_handler(void);
extern void free_handler_memory(void);
#endif  // LIB_DYNAMIC_H
