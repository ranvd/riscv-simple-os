#include "heap.h"
#include "node.h"
#include "os.h"

static void _swap(Arr_Heap *heap, int idx_a, int idx_b) {
    void *tmp = heap->nodelist[idx_a].data;
    heap->nodelist[idx_a].data = heap->nodelist[idx_b].data;
    heap->nodelist[idx_b].data = tmp;
}

int heap_push(Arr_Heap *heap, void *data) {
    int idx = heap->blk_top;
    if (idx >= heap->blk_max) {
        return 1;
    }
    heap->blk_top++;
    heap->nodelist[idx].data = data;
    // printf("priority: %d\n", ((context*)heap->nodelist[idx].data)->priority);

    void *parent = heap->nodelist[(idx - 1) / 2].data;
    void *cur = heap->nodelist[idx].data;
    while (idx && heap->compare(cur, parent)) {
        _swap(heap, idx, (idx - 1) / 2);
        idx = (idx - 1) / 2;
        cur = parent;
        parent = heap->nodelist[(idx - 1) / 2].data;
    }
    // printf("priority: \n");
    // for (int i = 0;i < heap->blk_top; i++){
    //     printf("%d ", ((context*)heap->nodelist[i].data)->priority);
    // }
    // printf("\n");

    return 0;
}

int heap_pop(Arr_Heap *heap) {
    /*
     * heap_pop only untrack the data in the node,
     * not delete it.
     */
    if (!heap->blk_top)
        return 1;
    heap->nodelist[0].data = heap->nodelist[--(heap->blk_top)].data;

    int cur_idx = 0;

    while (cur_idx < (int)(heap->blk_top - 1) / 2) {
        // printf("cur_idx: %d ||| blk_top: %d\n", cur_idx, (int) (heap->blk_top - 1) / 2);
        void *cur = heap->nodelist[cur_idx].data;
        void *l_child = heap->nodelist[cur_idx * 2 + 1].data;
        void *r_child = heap->nodelist[cur_idx * 2 + 2].data;
        if (heap->compare(l_child, r_child) && heap->compare(l_child, cur)) {
            _swap(heap, cur_idx, cur_idx * 2 + 1);
            cur_idx = cur_idx * 2 + 1;
        } else if (heap->compare(r_child, l_child) && heap->compare(r_child, cur)) {
            _swap(heap, cur_idx, cur_idx * 2 + 2);
            cur_idx = cur_idx * 2 + 2;
        } else {
            break;
        }
    }

    return 0;
}

void *heap_get(Arr_Heap heap) {
    return heap.nodelist[0].data;
}

void arr_heap_init(Arr_Heap *heap, void *data_pos, uint32_t blk_size, uint32_t blk_num, uint32_t blk_max, int (*compare)(void *, void *)) {
    heap->blk_top = 0;
    heap->blk_size = blk_size;
    heap->blk_max = blk_max;
    heap->compare = compare;
    heap->nodelist = (Node *)malloc(blk_max * blk_size);

    for (int i = 0; i < blk_max; i++) {
        if (i >= blk_num) {
            heap->nodelist[i].data = NULL;
            continue;
        }
        heap_push(heap, data_pos);
        data_pos = (uint8_t *)data_pos + blk_size;
    }
}