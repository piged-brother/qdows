#include "types.h"
#include "interrupts.h"
#include "gdt.h"
void printf(const char* str)
{
    static unsigned short* VideoMemory = (uint16_t*)0xb8000;


    static uint8_t x = 0,y = 0;
    for(int i = 0; str[i] != '\0'; ++i){//这里x和y写的是 一般我们的黑体框是80*25  x代表行 y代表列  所以80*i+x代表基于魔法数字后内存位置的写入信息

            switch(str[i]){
                case '\n': y++;x=0;break;

                default:
                     VideoMemory[80 * y +x] = (VideoMemory[80 * y +x] & 0xFF00) |//VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];: 在循环体内，这行代码执行了以下操作：VideoMemory[i] & 0xFF00: 这是一个位与（AND）操作，用于保留 VideoMemory[i] 中的高字节（高8位），将低字节（低8位）清零。| str[i]: 这是一个位或（OR）操作，将 str[i] 中的字符内容写入低字节部分。结果是将字符的颜色和属性保留在原来的 VideoMemory[i] 中，而将字符本身的ASCII码写入其中，从而实现在屏幕上显示带有颜色的文本。 str[i];
                    x++;

            }




    if(x >= 80) {y++; x=0;}
    if(y >= 25) {

        for(y = 0; y < 25; y++){
            for(x = 0; x < 80; x++){
                VideoMemory[80 * y +x] = (VideoMemory[80 * y +x] & 0xFF00) | ' ';


            }


        }
      x= 0; y= 0;
    }

    }
}



typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();//打印函数  就是在内存空间中的star_clor和end clor里面有函数地址 但是并不确定在哪 所以我们要遍历迭代的进行调用确保函数生效 确保构造函数初始化
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("qdows to be no.1\n");
    printf("made from wq\n");
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);
    interrupts.Activate();

    while(1);
}
