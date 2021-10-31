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
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x633da5cc, "__register_chrdev" },
	{ 0x977f511b, "__mutex_init" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x2d5f69b3, "rcu_read_unlock_strict" },
	{ 0xa5d57e43, "current_task" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x5cc0a8a2, "kmem_cache_alloc_trace" },
	{ 0xc85fd79f, "kmalloc_caches" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xb8b9f817, "kmalloc_order_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x2ab7989d, "mutex_lock" },
	{ 0xf21017d9, "mutex_trylock" },
	{ 0xc5850110, "printk" },
	{ 0x409bcb62, "mutex_unlock" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x56470118, "__warn_printk" },
	{ 0xb08a357c, "pv_ops" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "9A1ED7373FDCE160F501CA3");
