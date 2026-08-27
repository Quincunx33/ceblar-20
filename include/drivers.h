#ifndef CEBLAR_DRIVERS_H
#define CEBLAR_DRIVERS_H
#include <stdint.h>
void vga_init(void); void vga_clear(void); void vga_putc(char c); void vga_write(const char *s); void vga_scroll(void); void vga_set_color(uint8_t fg,uint8_t bg); void vga_set_cursor(uint8_t row,uint8_t col);
void serial_init(void); void serial_write(const char *s); void serial_write_n(const char *s,uint32_t n); int serial_read(void);
void gdt_init(void); void tss_init(uint32_t kernel_stack_top); void tss_set_kernel_stack(uint32_t esp); void enter_user_mode(uint32_t entry,uint32_t user_stack);
void idt_init(void); void pit_init(uint32_t hz); void timer_irq(void *registers); uint32_t timer_uptime_ms(void); void ksleep_ms(uint32_t ms);
void keyboard_init(void); void keyboard_irq(void *registers); char keyboard_getc(void);
#endif
