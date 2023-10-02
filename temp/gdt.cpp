#include "gdt.h"
#include "types.h"

GlobalDescriptorTable::GlobalDescriptorTable()
 :nullSegmentSelector(0,0,0),
 unusedSegmentSelector(0,0,0),
 codeSegmentSelector(0,64*1024*1024,0x9A),//0x9a 代表的gpt表中的type4位和前面4位的具体权限清楚 通过1001 1010来进行对应位的填充
 dataSegmentSelector(0,64*1024*1024,0x92){

   uint32_t i[2];//x86是小端模式 0x 12 34  会存储成 0x 34 12
   i[1]=(uint32_t)this;
   i[0]=sizeof(GlobalDescriptorTable) << 16;
   asm volatile("lgdt (%0)": :"p"(((uint8_t *) i)+2));//告诉系统使用这个gdt表简单来说，volatile 告诉编译器不要对这个变量进行优化，以确保它在程序执行期间的值是实时和准确的。这样，当变量的值可能被其他代码、硬件或中断处理程序等改变时，我们可以使用 volatile 来确保始终获取最新的值，而不是使用过时的缓存值。

}


GlobalDescriptorTable::~GlobalDescriptorTable(){
}

uint16_t GlobalDescriptorTable::DataSegmentSelector(){

return (uint8_t *) & dataSegmentSelector - (uint8_t *)this;//duan nei pian yi

}
uint16_t GlobalDescriptorTable::CodeSegmentSelector(){


  return (uint8_t *)&codeSegmentSelector -(uint8_t *)this;

}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base,uint32_t limit,uint8_t type){//写入gdp里面的位


    uint8_t * target = (uint8_t *)this;
    if(limit<=65536){

        target[6] = 0x40;//没设d/b位就是 1m*1b = 1m寻址 设d/b位后 然后就变成4kb*1m = 4g寻址  (4KB为单位的段大小 老式16位cpu 20条寻址总线)

    }else{
        /*limit & 0xfff：这是一个按位与操作，用于提取 limit 的低12位。由于GDT中的 limit 字段只有20位，因此我们需要取低12位来判断是否需要进行转换。

limit & 0xfff != 0xfff：这个条件判断语句检查 limit 的低12位是否不全为1。如果低12位不全为1，则说明 limit 的大小在 4095（0xFFF）以下，可以直接使用这个 limit 值。

limit = (limit >> 12) - 1;：如果 limit 的低12位不全为1，则将 limit 向右移动12位（相当于将低12位清零），然后再减去1。这样做的目的是将段的大小减小为原来的 1/4096，使得 limit 的单位变成了4KB。这样，我们可以用更多的位数来表示段的大小，而不是限制在原来的20位。

else 块：如果 limit 的低12位全为1，则说明 limit 的大小在4095以上，直接将 limit 向右移动12位，得到以4KB为单位的段大小。*/
    if((limit & 0xfff != 0xfff)){
        limit =(limit >> 12) -1;//

    }else{
        limit = limit >> 12;

    }
    target[6]=0xC0;
    }
   // Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    // Encode the base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Type
    target[5] = type;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base(){//提取gdp里面的base地址

   uint8_t * target = (uint8_t *)this;
   uint32_t result = target[7];
   result =(result << 8) + target[4];
   result = (result << 8) + target[3];
   result = (result << 8) + target[2];
   return result;


}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit(){//limit 20 wei

  uint8_t* target =(uint8_t*)this;
  uint32_t result = target[6] & 0xf;//取最后四位
  result = (result << 8) + target[1];
  result = (result << 8) + target[0];
 if((target[6] & 0xC0) == 0xC0)
  result ==(result << 12) | 0xfff;
  return result;

}




