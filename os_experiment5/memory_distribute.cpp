#include <stdlib.h>
#include <stdio.h>

#define MEM_SIZE 128
static char mem[MEM_SIZE];
#define MINBLK 16
struct block
{
    struct block *next;
    size_t length;
    char data[0];
};
static struct block *free_list = NULL;

void malloc_init()
{
    free_list = (struct block *)mem;
    free_list->next = nullptr;
    free_list->length = MEM_SIZE - sizeof(struct block);
}
// 返回值是data区的首地址
void *malloc(size_t size)
{
    size = (size + 7) & ~7;
    struct block *prev = (struct block *)&free_list;
    struct block *cur = free_list;
    while (cur != nullptr)
    {
        if (cur->length >= size)
            break;
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr)
        return nullptr;

    if ((cur->length - size) >= MINBLK)
    {
        struct block *temp = (struct block *)(cur->data + size);
        temp->next = cur->next;
        temp->length = cur->length - size - sizeof(struct block);
        prev->next = temp;
        cur->length = size;
    }
    else
    {
        prev->next = cur->next;
    }
    return cur->data;
}

void list_state(void)
{
    struct block *cur = free_list;
    // 显示free_list中含有的free block情况
    printf("free blocks:\n");
    while (cur != nullptr)
    {
        printf("%p:", cur);
        for (int i = 0; i < cur->length; i++)
            printf("=");
        printf(" ");
        cur = cur->next;
    }
    printf("\n");
    return;
}
// ptr传入的是data区的首地址
void free(void *ptr)
{
    if (ptr == nullptr)
        return;
    // temp是ptr所在块的头指针
    struct block *temp = (struct block *)((char *)ptr - sizeof(struct block));
    // temp_next是和tmp物理地址相邻的下一个block的首地址
    char *temp_next = (char *)ptr + temp->length;
    struct block *prev = (struct block *)&free_list;
    struct block *cur = free_list;
    char *cur_next; // 和cur物理地址相邻的下一个block的首地址
    // 检查free的块的位置，看是在free_list首地址的左边还是右边
    // 在free_list首地址左边,检查右边是否有相邻的free_list块
    if (temp < cur) 
    {
        // 右边有free_list的块
        if ((char *)cur == temp_next)
        {
            temp->length = temp->length + cur->length + sizeof(struct block);
            temp->next = cur->next;
        }
        // 右边没有free_list的块
        else
            temp->next = cur;
        prev->next = temp;
        return;
    }
    // 在free_list首地址右边
    else 
    {
        while (cur->next < temp)
        {
            prev = cur;
            cur = cur->next; // cur是free_list里左边离temp最近的块
        }
        cur_next = (char *)cur->data + cur->length; // 和cur物理地址相邻的下一个block的首地址
        // 检查将要free的块的左右是否有free_list的块。
        // 左边有右边没有
        if (cur_next == (char *)temp && temp_next != (char *)cur->next)
            cur->length = cur->length + temp->length + sizeof(struct block);
        // 右边有左边没有
        else if (cur_next != (char *)temp && temp_next == (char *)cur->next)
        {
            temp->length = temp->length + cur->length + sizeof(struct block);
            temp->next = cur->next->next;
            cur->next = temp;
        }
        // 左右均有
        else if (cur_next == (char *)temp && temp_next == (char *)cur->next)
        {
            cur->length = cur->length + cur->next->length + sizeof(struct block) * 2;
            cur->next = cur->next->next;
        }
        // 左右均没有
        else
        {
            temp->next = cur->next;
            cur->next = temp;
        }
    }
}

void malloc_test()
{
    list_state();
    void *ptr1 = malloc(sizeof(char));
    list_state();
    void *ptr2 = malloc(sizeof(int));
    list_state();
    void *ptr3 = malloc(sizeof(long));
    list_state();
    void *ptr4 = malloc(sizeof(double));
    list_state();
    printf("\n\n开始测试free函数。\n\n");
    free(ptr2);
    list_state();
    free(ptr3);
    list_state();
    free(ptr1);
    list_state();
    free(ptr4);
    list_state();
    return;
}

int main()
{
    printf("初始化free_list。\n");
    malloc_init();
    printf("开始测试。\n");
    malloc_test();
    return 0;
}