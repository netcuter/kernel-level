#ifdef CONFIG_IA32_EMULATION
#include "unistd_32.h"
#endif

void release_virtual_mapping(const void * addr)
{
	vunmap((void*)((unsigned long)addr & PAGE_MASK));
}

#ifdef __i386__
struct idt_descriptor {
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_flags;
    unsigned short offset_high;
} __attribute__ ((packed));
#elif defined(CONFIG_IA32_EMULATION)
struct idt_descriptor {
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero1;
    unsigned char type_flags;
    unsigned short offset_middle;
    unsigned int offset_high;
    unsigned int zero2;
} __attribute__ ((packed));
#endif

struct idtr {
    unsigned short limit;
    void *base;
} __attribute__ ((packed));

#ifdef CONFIG_IA32_EMULATION
void **ia32_sys_call_table;
#endif


#if defined(__i386__) || defined(CONFIG_IA32_EMULATION)
#ifdef __i386__
void *get_sys_call_table(void) {
#elif defined(__x86_64__)
void *get_ia32_sys_call_table(void) {
#endif
    struct idtr idtr;
    struct idt_descriptor idtd;
    void *system_call;
    unsigned char *ptr;
    int i;

    asm volatile("sidt %0" : "=m"(idtr));

    memcpy(&idtd, idtr.base + 0x80*sizeof(idtd), sizeof(idtd));

#ifdef __i386__
    system_call = (void*)((idtd.offset_high<<16) | idtd.offset_low);
    //printk("modul i386");
#elif defined(__x86_64__)
    system_call = (void*)(((long)idtd.offset_high<<32) |
                        (idtd.offset_middle<<16) | idtd.offset_low);
    //printk("modul x86_64");
#endif

    for (ptr=system_call, i=0; i<500; i++) {
#ifdef __i386__
        if (ptr[0] == 0xff && ptr[1] == 0x14 && ptr[2] == 0x85)
            return *((void**)(ptr+3));
#elif defined(__x86_64__)
        if (ptr[0] == 0xff && ptr[1] == 0x14 && ptr[2] == 0xc5)
            return (void*) (0xffffffff00000000 | *((unsigned int*)(ptr+3)));
#endif
        ptr++;
    }

    return NULL;
}
#endif


#ifdef __x86_64__
#define IA32_LSTAR 0xc0000082



void *get_sys_call_table(void) {
    void *system_call;
    unsigned char *ptr;
    int i, low, high;

    asm volatile("rdmsr" : "=a" (low), "=d" (high) : "c" (IA32_LSTAR));

    system_call = (void*)(((long)high<<32) | low);


    for (ptr=system_call, i=0; i<500; i++) {
        if (ptr[0] == 0xff && ptr[1] == 0x14 && ptr[2] == 0xc5)
            return (void*)(0xffffffff00000000 | *((unsigned int*)(ptr+3)));
        ptr++;
    }

    return NULL;
}
#endif


void *get_writable_sct(void *sct_addr) {
    struct page *p[2];
    void *sct;
    unsigned long addr = (unsigned long)sct_addr & PAGE_MASK;

    if (sct_addr == NULL)
        return NULL;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22) && defined(__x86_64__)
    p[0] = pfn_to_page(__pa_symbol(addr) >> PAGE_SHIFT);
    p[1] = pfn_to_page(__pa_symbol(addr + PAGE_SIZE) >> PAGE_SHIFT);
#else
    p[0] = virt_to_page(addr);
    p[1] = virt_to_page(addr + PAGE_SIZE);
#endif
    sct = vmap(p, 2, VM_MAP, PAGE_KERNEL);
    if (sct == NULL)
        return NULL;
    return sct + offset_in_page(sct_addr);
}

