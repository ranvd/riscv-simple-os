#ifndef __HEAP_H__
#define __HEAP_H__

#include "types.h"
#include "node.h"

typedef struct Arr_Heap
{
    Node *nodelist;
    uint32_t blk_size;
    uint32_t blk_top;
    uint32_t blk_max;
    int (*compare)(void*, void*);
}Arr_Heap;


extern void arr_heap_init(Arr_Heap *heap, void *data_pos, uint32_t blk_size, uint32_t blk_num, uint32_t blk_max, int (*compare)(void*, void*));
extern int heap_pop(Arr_Heap *heap);
extern int heap_push(Arr_Heap *heap, void *data);
extern void *heap_get(Arr_Heap heap);

#endif