#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xad12b0d5, "module_layout" },
	{ 0xfc43f5ac, "per_cpu__current_task" },
	{ 0x390b9990, "mem_map" },
	{ 0x18c55c40, "commit_creds" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xb72397d5, "printk" },
	{ 0x8dd219ef, "vmap" },
	{ 0xb4390f9a, "mcount" },
	{ 0x533c8c53, "prepare_creds" },
	{ 0x61651be, "strcat" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x94961283, "vunmap" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "F4CEE77A5F1A759D52F776C");
