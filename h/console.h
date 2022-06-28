#ifndef _OS1_RISCV_CONSOLE_H
#define _OS1_RISCV_CONSOLE_H

#define UART_BUFFER_SIZE 65536

#ifdef  __cplusplus
extern "C"{
#endif//__cplusplus

void __putc(char);
char __getc();

void console_handler();

void console_init();
void console_shutdown();

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//_OS1_RISCV_CONSOLE_H