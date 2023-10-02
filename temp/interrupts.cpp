
#include "interrupts.h"
#include "gdt.h"
void printf(const char*);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

void InterruptManager::SetInterruptDescriptorTableEntry(
 uint8_t interruptNumber,
   uint16_t codeSegmentSelectorOffset,
   void(*handler)(),
   uint8_t DescriptorPrivilegelLevel,
   uint8_t DescriptorType
){

  const uint8_t IDT_DESC_PRESENT = 0x80;// fang wen quan xian
  interruptDescriptorTable[interruptNumber].handlerAddressLowBits=((uint32_t)handler) & 0xffff;
  interruptDescriptorTable[interruptNumber].handlerAddressHighBits=((uint32_t)handler >> 16)&0xffff;

  interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;

   interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | ((DescriptorPrivilegelLevel & 3 )<< 5) | DescriptorType;// dpl zhi qu liang wei

   interruptDescriptorTable[interruptNumber].reserved = 0;

}

InterruptManager::InterruptManager(uint16_t  hardwareInterruptOffset,GlobalDescriptorTable * gdt):          programmableInterruptControllerMasterCommandPort(0x20),
     programmableInterruptControllerMasterDataPort(0x21),
      programmableInterruptControllerSlaveCommandPort(0xA0),
      programmableInterruptControllerSlaveDataPort(0xA1){// chu shi hua  zhong duan men  8259 芯片 主中断地址是0x20 和0x21 8 ge port , 副中断是0xA0 0xA1 8 ge port

    this->hardwareInterruptOffset = hardwareInterruptOffset;
    uint16_t CodeSegment = gdt -> CodeSegmentSelector();

  const uint8_t IDT_INTERRUPT_GATE = 0xe;
  for(uint16_t i = 0; i < 256 ; i++){
      SetInterruptDescriptorTableEntry(i,CodeSegment,&InterruptIgnore,0, IDT_INTERRUPT_GATE);

      }


      SetInterruptDescriptorTableEntry(0x00,CodeSegment,&HandleException0x00, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x01,CodeSegment,&HandleException0x01, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x02,CodeSegment,&HandleException0x02, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x03,CodeSegment,&HandleException0x03, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x04,CodeSegment,&HandleException0x04, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x05,CodeSegment,&HandleException0x05, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x06,CodeSegment,&HandleException0x06, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x07,CodeSegment,&HandleException0x07, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x08,CodeSegment,&HandleException0x08, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x09,CodeSegment,&HandleException0x09, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x0A,CodeSegment,&HandleException0x0A, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x0B,CodeSegment,&HandleException0x0B, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x0C,CodeSegment,&HandleException0x0C, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x0D,CodeSegment,&HandleException0x0D, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x0E,CodeSegment,&HandleException0x0E, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x0F,CodeSegment,&HandleException0x0F, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x10,CodeSegment,&HandleException0x10, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x11,CodeSegment,&HandleException0x11, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x12,CodeSegment,&HandleException0x12, 0,IDT_INTERRUPT_GATE);
      SetInterruptDescriptorTableEntry(0x13,CodeSegment,&HandleException0x13, 0,IDT_INTERRUPT_GATE);

// 这里加hardwareInterruptOffset 是因为异常处理和中断处理都是从0x00开始的所以会有内存偏移量相加才不会导致冲突
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00,CodeSegment,&HandleInterruptRequest0x00, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01,CodeSegment,&HandleInterruptRequest0x01, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x02,CodeSegment,&HandleInterruptRequest0x02, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x03,CodeSegment,&HandleInterruptRequest0x03, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x04,CodeSegment,&HandleInterruptRequest0x04, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x05,CodeSegment,&HandleInterruptRequest0x05, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x06,CodeSegment,&HandleInterruptRequest0x06, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x07,CodeSegment,&HandleInterruptRequest0x07, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x08,CodeSegment,&HandleInterruptRequest0x08, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x09,CodeSegment,&HandleInterruptRequest0x09, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0A,CodeSegment,&HandleInterruptRequest0x0A, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0B,CodeSegment,&HandleInterruptRequest0x0B, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0C,CodeSegment,&HandleInterruptRequest0x0C, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0D,CodeSegment,&HandleInterruptRequest0x0D, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0E,CodeSegment,&HandleInterruptRequest0x0E, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0F,CodeSegment,&HandleInterruptRequest0x0F, 0 ,IDT_INTERRUPT_GATE);
   SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x31,CodeSegment,&HandleInterruptRequest0x31, 0 ,IDT_INTERRUPT_GATE);



    programmableInterruptControllerMasterCommandPort.Write(0x11);
    programmableInterruptControllerSlaveCommandPort.Write(0x11);

    // remap
    programmableInterruptControllerMasterDataPort.Write(hardwareInterruptOffset);
    programmableInterruptControllerSlaveDataPort.Write(hardwareInterruptOffset+8);

    programmableInterruptControllerMasterDataPort.Write(0x04);
    programmableInterruptControllerSlaveDataPort.Write(0x02);

    programmableInterruptControllerMasterDataPort.Write(0x01);
    programmableInterruptControllerSlaveDataPort.Write(0x01);

    programmableInterruptControllerMasterDataPort.Write(0x00);
    programmableInterruptControllerSlaveDataPort.Write(0x00);

    InterruptDescriptorTablePointer idt_pointer;
    idt_pointer.size  = 256*sizeof(GateDescriptor) - 1;
    idt_pointer.base  = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt_pointer));



}
uint16_t InterruptManager::HardwareInterruptOffset()
{
    return hardwareInterruptOffset;
}

void InterruptManager::Activate()
{
    //if(ActiveInterruptManager == 0)

        //ActiveInterruptManager = this;
        asm("sti");

}

void InterruptManager::Deactivate()
{
    /*if(ActiveInterruptManager == this)
    {
        ActiveInterruptManager = 0;
        */
        //asm("cli");
        /*
    }*/
        asm("cli");
}

uint32_t  InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp){

    printf("interrupts");
    return esp;

}

InterruptManager::~InterruptManager(){}
