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
	{ 0x3e0fecb8, "module_layout" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xa58b498f, "class_destroy" },
	{ 0xce789fbc, "device_destroy" },
	{ 0x687e9f04, "cdev_del" },
	{ 0xe0ac035a, "device_create" },
	{ 0xa4e8f334, "__class_create" },
	{ 0x911cfbec, "cdev_add" },
	{ 0xd7b314c, "cdev_init" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x50eedeb8, "printk" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "DEDEE5C2C8E66E79C79C4FA");
