/* linker_template.lds -- linker for bootloader component
 * linker script contains 4 main components:
 * |--- memory layout
 * |--- sections
 * |--- options
 * |--- symbols
 *
 * It defines following symbols, which code can use without definition:
 *   __isr_vector_start
 *   __isr_vector_end
 *   __kernel_text_start
 *   __kernel_text_end
 *   __swi_stub_start
 *   __swi_stub_end
 *   __user_text_start
 *   __user_text_end
 *   __text_end
 *   __kernel_rodata_start
 *   __kernel_rodata_end
 *   __user_rodata_start
 *   __user_rodata_end
 *   __exidx_start
 *   __exidx_end
 *   __idata_start
 *   __data_start
 *   __data_end
 *   __kernel_data_start
 *   __kernel_data_end
 *   __user_data_start
 *   __user_data_end
 *   __rtt_start
 *   __rtt_end
 *   __bss_start
 *   __bss_end
 *   __kernel_bss_start
 *   __kernel_bss_end
 *   __user_bss_start
 *   __user_bss_end
 *   __heap_base
 *   __heap_limit
 *   Heap_Size
 *   __stack_base
 *   __stack_limit
 *   Stack_Size
 *
 * do not re-distribute
 */
 
/*OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)*/

MEMORY
{
  flash       (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00100000
  ram         (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00040000
}

Stack_Size = DEFINED(Stack_Size) ? Stack_Size : 0x2000;

SECTIONS
{
/* sections to include:
 * .text for code & constants
 * .bss for unintialized data
 * .heap for our heap
 * .stack for our stack
 * .data for initialized data
 */

  .text :
  {
    /* vector table entries */
    __isr_vector_start = .;
    KEEP(*(.isr_vector))
    __isr_vector_end = .;
    _kernel_text_start = .;
    <K_OBJ_DIR>/*.o (.text*)        /* template */
    _kernel_text_end = .;
  } > flash

  . = ALIGN(4096); /* Align to 4K boundary */

  .app_text :
  {
    /* userspace syscall stubs */
    __swi_stub_start = .; 
    KEEP( *( .swi_stub ) )
    __swi_stub_end = .;
    __user_text_start = .;
    <U_OBJ_DIR>/*.o (.text*)        /* template */
    __user_text_end = .;
  } > flash

  . = ALIGN(4);
  __text_end = .;

  .rodata :
  {
    /* read-only kernel data -- 4kb aligned */
    . = ALIGN(4096);
    __kernel_rodata_start = .;
    <K_OBJ_DIR>/*.o (.rodata*);        /* template */
    __kernel_rodata_end = .;
    /* read-only user data -- 1kb aligned */
    . = ALIGN(1024);
    __user_rodata_start = .;
    <U_OBJ_DIR>/*.o (.rodata*);        /* template */
    *( .rodata );
    __user_rodata_end = .;
  } > flash

  PROVIDE_HIDDEN (__exidx_start = .);
  .ARM.exidx : ALIGN(4)
  {
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > flash
  PROVIDE_HIDDEN (__exidx_end = .);

  __idata_start = .;

  .data : AT (__idata_start)
  {
    /* initialized data sections in ram */
    __data_start = .;
    __rtt_start = .;
    . = . + 168; /* RTT control block is 168 bytes long */
    __rtt_end = .;
    __kernel_data_start = .;
    <K_OBJ_DIR>/*.o (.data*);        /* template */
    __kernel_data_end = .;
    . = ALIGN(1024);
    __user_data_start = .;
    <U_OBJ_DIR>/*.o (.data*);        /* template */
    *( .data );
    __user_data_end = .;
    . = ALIGN(1024);
    __data_end = .;
  } > ram

  .bss :
  {
    /* uninitialized data sections in ram */
    __bss_start = .;
    __kernel_bss_start = .;
    <K_OBJ_DIR>/*.o (.bss*);        /* template */
    <K_OBJ_DIR>/*.o (COMMON*);      /* template */
    __kernel_bss_end = .;
    . = ALIGN(1024);
    __user_bss_start = .;
    <U_OBJ_DIR>/*.o (.bss*);        /* template */
    <U_OBJ_DIR>/*.o (COMMON*);      /* template */
    * ( .bss ) * ( COMMON ) ;
    __user_bss_end = .;
    . = ALIGN(4);
    __bss_end = .;
  } > ram

  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    __heap_base = .;
    __heap_limit = ORIGIN(ram) + LENGTH(ram) - Stack_Size;
    Heap_Size = __heap_limit - __heap_base;
  } > ram

  .stack (NOLOAD) :
  {
    __stack_base = ORIGIN(ram) + LENGTH(ram);
    __stack_limit = __stack_base - Stack_Size;
  } > ram

  . = ALIGN(4);
  _end = .;
}
