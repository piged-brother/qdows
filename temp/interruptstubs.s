 
.set IRQ_BASE, 0x20
.section .text
.extern _ZN16InterruptManager15handleInterruptEhj /*/sheng ming interrupt han shu*/

  #sheng ming hong  "name manging" ming ming fang fa
.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev/*sheng ming */
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev: /*dingyi*/
       movb $\num + IRQ_BASE , (interruptnumber)
       jmp int_bottom
.endm

.macro HandleException num
.global _ZN16InterruptManager19HandleException\num\()Ev   /*sheng ming */
  _ZN16InterruptManager19HandleException\num\()Ev:        /*dingyi*/
      movb $\num , (interruptnumber)
      jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02
HandleInterruptRequest 0x03
HandleInterruptRequest 0x04
HandleInterruptRequest 0x05
HandleInterruptRequest 0x06
HandleInterruptRequest 0x07
HandleInterruptRequest 0x08
HandleInterruptRequest 0x09
HandleInterruptRequest 0x0A
HandleInterruptRequest 0x0B
HandleInterruptRequest 0x0C
HandleInterruptRequest 0x0D
HandleInterruptRequest 0x0E
HandleInterruptRequest 0x0F
HandleInterruptRequest 0x31


HandleException 0x00
HandleException 0x01
HandleException 0x02
HandleException 0x03
HandleException 0x04
HandleException 0x05
HandleException 0x06
HandleException 0x07
HandleException 0x08
HandleException 0x09
HandleException 0x0A
HandleException 0x0B
HandleException 0x0C
HandleException 0x0D
HandleException 0x0E
HandleException 0x0F
HandleException 0x10
HandleException 0x11
HandleException 0x12
HandleException 0x13


int_bottom:
   #ruzhan
         pusha
         push %ds
         push %es
         push %fs
         push %gs

         push %esp
         push (interruptnumber)
         call _ZN16InterruptManager15handleInterruptEhj
         add %esp, 6
         mov %eax,%esp

         #chuzhan
         pop %gs
         pop %fs
         pop %es
         pop %ds
         popa

.global _ZN16InterruptManager15InterruptIgnoreEv
_ZN16InterruptManager15InterruptIgnoreEv:


         iret
         #IRET是中断返回指令，它是中断处理程序的结束指令，其作用是使PLC返回到被中断时的下一条指令继续执行。 若没有中断处理子程序，虽然触发了中断，但没有处理任何事情。

 .data
    interruptnumber: .byte 0
