#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe2a06d27, "module_layout" },
	{ 0xe75107db, "param_ops_ulong" },
	{ 0xd83e24a, "param_ops_int" },
	{ 0x4acd9a7d, "param_array_ops" },
	{ 0x37a0cba, "kfree" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0x6d671fad, "pv_ops" },
	{ 0xb6e1e296, "kmem_cache_alloc_trace" },
	{ 0xb7349dcd, "kmalloc_caches" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "BBAF1D91B7C666FF55EC13E");
