// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/atomic.h>
#include <linux/string.h>
#include <linux/printk.h>
#include "kdev_ioctl.h"

#define KDEV_VERSION 1

static atomic_t kdev_opens = ATOMIC_INIT(0);

static int kdev_open(struct inode *inode, struct file *file)
{
	atomic_inc(&kdev_opens);
	return 0;
}

static long kdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *uarg = (void __user *)arg;

	switch (cmd) {
	case KDEV_IOC_PING: {
		__u32 v = KDEV_PING_REPLY;

		if (copy_to_user(uarg, &v, sizeof(v)))
			return -EFAULT;
		return 0;
	}
	case KDEV_IOC_GET_INFO: {
		struct kdev_info info;

		memset(&info, 0, sizeof(info));
		info.version = KDEV_VERSION;
		info.open_count = (__u32)atomic_read(&kdev_opens);
		strscpy(info.name, "kdev", sizeof(info.name));
		if (copy_to_user(uarg, &info, sizeof(info)))
			return -EFAULT;
		return 0;
	}
	case KDEV_IOC_ADD: {
		struct kdev_add req;

		if (copy_from_user(&req, uarg, sizeof(req)))
			return -EFAULT;
		req.sum = (__s64)req.a + (__s64)req.b;
		if (copy_to_user(uarg, &req, sizeof(req)))
			return -EFAULT;
		return 0;
	}
	default:
		return -ENOTTY;
	}
}

static const struct file_operations kdev_fops = {
	.owner          = THIS_MODULE,
	.open           = kdev_open,
	.unlocked_ioctl = kdev_ioctl,
	.compat_ioctl   = compat_ptr_ioctl,
	.llseek         = noop_llseek,
};

static struct miscdevice kdev_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "kdev",
	.fops  = &kdev_fops,
	.mode  = 0600,
};

static int __init kdev_init(void)
{
	int ret = misc_register(&kdev_misc);

	if (ret)
		pr_err("kdev: misc_register failed: %d\n", ret);
	else
		pr_info("kdev: registered /dev/kdev\n");
	return ret;
}

static void __exit kdev_exit(void)
{
	misc_deregister(&kdev_misc);
}

module_init(kdev_init);
module_exit(kdev_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Custom kdev misc device");
