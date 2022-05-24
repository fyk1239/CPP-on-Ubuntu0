#include <stdio.h>

#define MEM_SIZE 32
#define RAM_SIZE 16
#define SWAP_SIZE 32

struct page_item // 页表项
{
    int ram_addr;    // 内存中物理地址
    int swap_addr;   // 外存地址
    unsigned access; // 访问字段
    bool present;    // 状态位
    bool modified;   // 修改位
};
static char ram[RAM_SIZE];                    // 内存
static char swap[SWAP_SIZE];                  // 外存
static struct page_item page_table[MEM_SIZE]; // 页表
void mem_init()
{
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
        ram[i] = i;
    }
}
// 换页算法，选出内存中访问次数最少的页换出。
// LFU算法，选出内存中一段时间内被访问次数最少的页换出。
// 若有多个页帧访问次数相同且均为最少，则选择其中下标最小的页换出。
static int swap_algorithm()
{
    int min_access = -1;
    int min_access_page = -1;
    for (int i = 0; i < MEM_SIZE; i++)
    {
        if (page_table[i].present &&
            (min_access == -1 || page_table[i].access < min_access))
        {
            min_access = page_table[i].access;
            min_access_page = i;
        }
    }
    return min_access_page;
}
// 逻辑地址(页表地址)logic_addr转换为物理地址(内存地址)physic_addr
static int mem_access(int logic_addr)
{
    // 在内存中时，直接返回内存地址
    if (page_table[logic_addr].present)
    {
        page_table[logic_addr].access++;
        return page_table[logic_addr].ram_addr;
    }
    // 缺页处理，根据特定算法选择被换出的页
    int page = swap_algorithm();
    // 换出
    if (page_table[page].modified) // 写回外存
    {
        int data = ram[page_table[page].ram_addr];
        swap[page_table[page].swap_addr] = data;
    }
    page_table[page].present = false;
    // 替换
    int physic_addr = page_table[page].ram_addr;
    page_table[logic_addr].ram_addr = physic_addr;

    ram[physic_addr] = swap[page_table[logic_addr].swap_addr];

    page_table[logic_addr].modified = false;
    page_table[logic_addr].present = true;
    page_table[logic_addr].access = 1;

    return physic_addr;
}
// 读取
int mem_read(int addr, char *buf)
{
    if (addr < 0 || addr > MEM_SIZE)
        return -1;

    int physic_addr = mem_access(addr);
    *buf = ram[physic_addr];

    return 0;
}
// 写入
int mem_write(int addr, char buf)
{
    if (addr < 0 || addr > MEM_SIZE)
        return -1;

    int paddr = mem_access(addr);
    ram[paddr] = buf;
    page_table[addr].modified = true;

    return 0;
}
// 打印状态
void mem_state()
{
    printf("RAM state:\t");
    for (int i = 0; i < RAM_SIZE; i++)
        printf("%02x ", ram[i]);
    printf("\nswap state:\t");
    for (int i = 0; i < SWAP_SIZE; i++)
        printf("%02x ", swap[i]);
    printf("\n\n");
}
// 要编写的测试函数
void mem_test()
{
    char *ptr = new char();
    mem_state();

    mem_read(RAM_SIZE + 2, ptr); // 测试换页功能，换掉第一个页ram[0]
    printf("%x \n", *ptr);
    mem_state();
    mem_read(1, ptr); // 读一遍ram[1]
    mem_read(RAM_SIZE + 5, ptr); // 测试，可以看到替换掉的不是ram[1]是ram[2]，测试正确
    printf("%x \n", *ptr);
    mem_state();
    mem_write(3, 0x22);          // 此时ram[3]已经成为下一个可能被换出的页，现在修改它的值，刷新位置
    mem_state();                 // ram[3]里面的22写回到了swap里面
    mem_read(RAM_SIZE + 7, ptr); // 这个时候替换的是ram[4]
    printf("%x \n", *ptr);
    mem_state();
    // 刷新一次，把之前修改过的ram[3]轮换到下一个要被替换的位置
    // 可以看到此时下一个就要换ram[3]，我们测试修改后的写回功能
    mem_read(RAM_SIZE + 8, ptr);
    printf("%x \n", *ptr);
    mem_state();
}

int main(int argc, const char *argv[])
{
    mem_init();
    mem_test();
    return 0;
}