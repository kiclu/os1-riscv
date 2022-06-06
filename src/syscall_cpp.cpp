#include"../h/syscall_cpp.hpp"

void* operator new(size_t size){ return mem_alloc(size); }
void operator delete(void* addr){ mem_free(addr); }