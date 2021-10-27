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
	{ 0xb3df6ff9, "module_layout" },
	{ 0xe75107db, "param_ops_ulong" },
	{ 0xd83e24a, "param_ops_int" },
	{ 0x4acd9a7d, "param_array_ops" },
	{ 0x5cc0a8a2, "kmem_cache_alloc_trace" },
	{ 0xc85fd79f, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0x2d5f69b3, "rcu_read_unlock_strict" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xa5d57e43, "current_task" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xb8b9f817, "kmalloc_order_trace" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x56470118, "__warn_printk" },
	{ 0xb08a357c, "pv_ops" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B13F524D12BB2B9008CF3FC");
