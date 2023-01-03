typedef struct sll
{
    struct sll* next;
}sll;

int sll_insert(sll *head, int idx, int (*compare)(void*, void*));
int sll_delete(sll *head, int idx);
