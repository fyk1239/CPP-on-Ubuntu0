#include <stdio.h>

#define MEM_SIZE 32

#define RAM_SIZE 16
#define SWAP_SIZE 32

struct page_item
{ // 页表项
    int ram_addr;
    int swap_addr;
    unsigned access;
    bool present;
    bool modified;
};

static char ram[RAM_SIZE];
static char swap[SWAP_SIZE];
static struct page_item page_table[MEM_SIZE];

struct node
{
    int ram_addr;
    node *next;
};
node *head = new node();

static int lru_swap(void)
{
    return head->next->ram_addr;
}

// select the pre node* of the target node
static node *select(int target_addr)
{
    node *now = head;
    while (now->next->ram_addr != target_addr)
    {
        now = now->next;
    }
    // now the nowptr is pre of the target
    return now;
}

static node *tailnode(void)
{
    node *cur = head;
    while (cur->next != nullptr)
    {
        cur = cur->next;
    }
    return cur;
}

// 逻辑地址laddr转换为物理地址paddr，laddr指paper table里面的地址，paddr指ram里的地址
static int mem_access(int laddr)
{
    if (page_table[laddr].present)
    { // 如果这页在ram里
        page_table[laddr].access++;
        //
        node *pre = select(page_table[laddr].ram_addr);
        node *cur = pre->next;
        pre->next = cur->next;
        tailnode()->next = cur;
        cur->next = nullptr;
        //
        return page_table[laddr].ram_addr; // 返回这页的ram地址
    }

    // 缺页处理

    // 根据特定算法选择被换出的页
    int page = lru_swap();
    // int page = algo_swap();

    // 如果换出的页被修改了要写回
    if (page_table[page].modified)
    { // 写回
        int data = ram[page_table[page].ram_addr];
        swap[page_table[page].swap_addr] = data; // 写回swap里面
    }
    page_table[page].present = false;

    // 替换
    int paddr = page_table[page].ram_addr; // 被换出的页的ram里面的地址
    page_table[laddr].ram_addr = paddr;

    ram[paddr] = swap[page_table[laddr].swap_addr];

    page_table[laddr].modified = false;
    page_table[laddr].present = true;
    page_table[laddr].access = 1;

    node *pre = select(paddr);
    node *cur = pre->next;
    pre->next = cur->next;
    delete (cur);
    node *now = new node();
    now->ram_addr = paddr;
    now->next = nullptr;
    tailnode()->next = now;

    return paddr;
}

int mem_read(int addr, char *buf)
{
    if (addr < 0 || addr > MEM_SIZE)
        return -1;

    int paddr = mem_access(addr);
    *buf = ram[paddr];

    return 0;
}

int mem_write(int addr, char buf)
{
    if (addr < 0 || addr > MEM_SIZE)
        return -1;

    int paddr = mem_access(addr);
    ram[paddr] = buf;
    page_table[addr].modified = true;

    return 0;
}

void mem_init(void)
{
    node *pre = head;
    // 全部加载到交换空间
    for (int i = 0; i < MEM_SIZE; i++)
    {
        page_table[i].swap_addr = i;
        page_table[i].present = false;
        page_table[i].modified = false;
        swap[i] = i;
    }
    // 部分加载到内存
    for (int i = 0; i < RAM_SIZE; i++)
    {
        page_table[i].ram_addr = i;
        page_table[i].present = true;
        page_table[i].access = 0;
        // 新添代码
        node *p = new node();
        p->ram_addr = i;
        pre->next = p;
        pre = p;
        ram[i] = i;
    }
}

void mem_state(void)
{
    printf("ram state:\t");
    for (int i = 0; i < RAM_SIZE; i++)
    {
        printf("%02x ", ram[i]);
    }
    printf("\nswap state:\t");
    for (int i = 0; i < SWAP_SIZE; i++)
    {
        printf("%02x ", swap[i]);
    }

    printf("\nlru linklist:\t");
    node *p = head->next;
    while (p != nullptr)
    {
        printf("%d - ", p->ram_addr);
        p = p->next;
    }
    printf("\n\n");
}
// mem_init里面已经将ram和swap都装上page_table里面的值了，只不过现在由于所有值都是0，所以完全看不出来有什么差别
// 为了让差别可视化，首先我们给他们赋值，我们默认page_table[i]=i，这样ram里面应该是00~0f,swap里面是00~1f
void mem_test(void)
{
    char *ptr = new char();
    mem_state();

    mem_read(RAM_SIZE + 2, ptr); // 读swap里面的，换掉lru linklist的第一个也就是ram[0]
    printf("%x \n", *ptr);
    mem_state();
    mem_read(1, ptr); // 按道理来说此时ram[1]到了lru linklist的第一个，下次换就是换它，我特意读一遍ram[1]以刷新，这样ram[1]就到了lru linklist最后一个
    printf("%x \n", *ptr);
    mem_state();
    mem_read(RAM_SIZE + 5, ptr); // 测试，可以看到替换掉的不是ram[1]是ram[2]，测试正确
    printf("%x \n", *ptr);
    mem_state();
    mem_write(3, 0x22); // 此时ram[3]已经到了lru linklist第一个，我修改它的值，刷新位置
    mem_state();
    mem_read(RAM_SIZE + 7, ptr); // 果然这个时候替换的是ram[4]
    printf("%x \n", *ptr);
    mem_state();
    // 刷新一轮，把之前改过的ram[3]轮换到下一个要被替换的位置
    int k = 5;
    while (k != 3)
    {
        mem_read(k, ptr);
        k++;
        if (k > 15)
            k = 0;
    }
    mem_state(); // 可以看到此时下一个就要换ram[3]，我们测试修改后的写回功能
    mem_read(RAM_SIZE + 8, ptr);
    printf("%x \n", *ptr);
    mem_state(); // 果然ram[3]里面的22写回到了swap里面
}

int main(int argc, const char *argv[])
{
    mem_init();

    mem_test();

    return 0;
}