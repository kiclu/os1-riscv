#include"../h/mem.h"

static void panic(){ for(;;); }

typedef struct _mem_block{
    size_t block_size;
    struct _mem_block* ptr_next;
    struct _mem_block* ptr_prev;
} _mem_block;

// user memory
static _mem_block* mem_free = NULL;
static _mem_block* mem_inuse = NULL;

// kernel memory
static _mem_block* kmem_inuse = NULL;

size_t block_count(size_t size){
    size_t min_size = size + sizeof(_mem_block);
    size_t overhang_size = min_size % MEM_BLOCK_SIZE;
    return (min_size / MEM_BLOCK_SIZE) + (overhang_size > 0);
}

static size_t align(size_t size){ return block_count(size) * MEM_BLOCK_SIZE; }

static void mb_init(_mem_block* mb, size_t size){
    mb->ptr_prev = mb->ptr_next = NULL;
    mb->block_size = size;
}

static void mb_strip(_mem_block* mb, size_t size){
    _mem_block* prev = mb->ptr_prev;
    _mem_block* next = mb->ptr_next;
    if(prev) prev->ptr_next = mb->ptr_next;
    if(next) next->ptr_prev = mb->ptr_prev;
    mb_init(mb, size);

    if((char*)mb + size > (char*)HEAP_END_ADDR)
        mb->block_size = HEAP_END_ADDR - (const void*)mb;
}

static void merge(_mem_block* mb){
    if(mb == NULL) return;
    if(mb->ptr_next == NULL) return;
    if((char*)mb + mb->block_size > (char*)mb->ptr_next) panic();
    if((char*)mb + mb->block_size < (char*)mb->ptr_next) return;
    mb->block_size += mb->ptr_next->block_size;
    mb->ptr_next = mb->ptr_next->ptr_next;
    if(mb->ptr_next) mb->ptr_next->ptr_prev = mb;
}

static void mem_free_insert(_mem_block* mb, size_t size){
    if(size == 0) return;

    mb_init(mb, size);
    if(mem_free == NULL){
        mem_free = mb;
        return;
    }

    if((char*)mb < (char*)mem_free){
        mb->ptr_next = mem_free;
        mem_free->ptr_prev = mb;
        mem_free = mb;
        merge(mb);
        return;
    }

    for(_mem_block* i = mem_free; i; i = i->ptr_next){
        if(i->ptr_next == NULL){
            i->ptr_next = mb;
            mb->ptr_prev = i;
            merge(i);
            return;
        }
        if((char*)i < (char*)mb && (char*)mb < (char*)i->ptr_next){
            mb->ptr_next = i->ptr_next;
            mb->ptr_prev = i;
            i->ptr_next = mb;
            i->ptr_next->ptr_prev = mb;
            merge(mb);
            merge(i);
            return;
        }
    }

    panic();
}

static void mem_free_remove(_mem_block* mb){
    if(mem_free == NULL) panic();

    if(mem_free == mb){
        mem_free = mem_free->ptr_next;
        if(mem_free) mem_free->ptr_prev = NULL;
    }

    mb_strip(mb, mb->block_size);
}

static void mem_inuse_insert(_mem_block* mb, size_t size){
    if(size == 0) return;

    mb_strip(mb, size);
    if(mem_inuse == NULL){
        mem_inuse = mb;
        return;
    }

    if((char*)mb < (char*)mem_inuse){
        mb->ptr_next = mem_inuse;
        mem_inuse->ptr_prev = mb;
        mem_inuse = mb;
        return;
    }

    for(_mem_block* i = mem_inuse; i; i = i->ptr_next){
        if(i->ptr_next == NULL){
            i->ptr_next = mb;
            mb->ptr_prev = i;
            return;
        }
        if((char*)i < (char*)mb && (char*)mb < (char*)i->ptr_next){
            mb->ptr_next = i->ptr_next;
            mb->ptr_prev = i;
            i->ptr_next = mb;
            i->ptr_next->ptr_prev = mb;
            return;
        }
    }

    panic();
}

static void mem_inuse_remove(_mem_block* mb){
    if(mem_inuse == NULL) panic();
    
    if(mem_inuse == mb){
        mem_inuse = mem_inuse->ptr_next;
        if(mem_inuse) mem_inuse->ptr_prev = NULL;
    }

    mb_strip(mb, mb->block_size);
}

void* __mem_alloc(size_t block_count){
    if(mem_free == NULL) return NULL;

    size_t size = block_count * MEM_BLOCK_SIZE;

    _mem_block* mb = NULL;
    for(_mem_block* i = mem_free; i; i = i->ptr_next){
        if(i->block_size >= size){
            if(mb != NULL){
                if(i->block_size < mb->block_size) mb = i;
                continue;
            }
            mb = i;
            if(FIT_ALGORITHM) break;
        }
    }

    if(mb == NULL) return NULL;

    mem_free_remove(mb);
    mem_free_insert((_mem_block*)((char*)mb + size), mb->block_size - size);
    mem_inuse_insert(mb, size);

    return (char*)mb + sizeof(_mem_block);
}

int __mem_free(void* addr){
    if(addr == NULL) return -1;
    if(mem_inuse == NULL) return -1;

    _mem_block* mb = (_mem_block*)((char*)addr - sizeof(_mem_block));
    for(_mem_block* i = mem_inuse; i; i = i->ptr_next){
        if(i == mb){
            mem_inuse_remove(mb);
            mem_free_insert(mb, mb->block_size);
            return 0;
        }
    }

    return -1;
}

static void kmem_inuse_insert(_mem_block* mb, size_t size){
    if(size == 0) return;

    mb_init(mb, size);
    if(kmem_inuse == NULL){
        kmem_inuse = mb;
        return;
    }

    if((char*)mb < (char*)kmem_inuse){
        mb->ptr_next = kmem_inuse;
        kmem_inuse->ptr_prev = mb;
        kmem_inuse = mb;
        return;
    }

    for(_mem_block* i = kmem_inuse; i; i = i->ptr_next){
        if(i->ptr_next == NULL){
            i->ptr_next = mb;
            mb->ptr_prev = i;
            return;
        }
        if((char*)i < (char*)mb && (char*)mb < (char*)i->ptr_next){
            mb->ptr_next = i->ptr_next;
            mb->ptr_prev = i;
            i->ptr_next = mb;
            i->ptr_next->ptr_prev = mb;
            return;
        }
    }

    panic();
}

static void kmem_inuse_remove(_mem_block* mb){
    if(kmem_inuse == NULL) panic();
    
    if(kmem_inuse == mb){
        kmem_inuse = kmem_inuse->ptr_next;
        if(kmem_inuse) kmem_inuse->ptr_prev = NULL;
    }

    mb_strip(mb, mb->block_size);
}

void* __kmem_alloc(size_t size){
    if(mem_free == NULL) return NULL;

    size = align(size);

    _mem_block* mb = NULL;
    for(_mem_block* i = mem_free; i; i = i->ptr_next){
        if(i->block_size >= size){
            if(mb != NULL){
                if(i->block_size < mb->block_size) mb = i;
                continue;
            }
            mb = i;
            if(FIT_ALGORITHM) break;
        }
    }

    if(mb == NULL) return NULL;

    mem_free_remove(mb);
    mem_free_insert((_mem_block*)((char*)mb + size), mb->block_size - size);
    kmem_inuse_insert(mb, size);

    return (void*)((char*)mb + sizeof(_mem_block));
}

int __kmem_free(void* addr){
    if(addr == NULL) return -1;
    if(kmem_inuse == NULL) return -1;

    _mem_block* mb = (_mem_block*)((char*)addr - sizeof(_mem_block));
    for(_mem_block* i = kmem_inuse; i; i = i->ptr_next){
        if(i == mb){
            kmem_inuse_remove(mb);
            mem_free_insert(mb, mb->block_size);
            return 0;
        }
    }

    return -1;
}

void mem_init(){
    mem_free_insert(
        (_mem_block*)HEAP_START_ADDR,
        (size_t)(HEAP_END_ADDR - HEAP_START_ADDR)
    );
}
