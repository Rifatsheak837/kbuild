/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _KDEV_IOCTL_H
#define _KDEV_IOCTL_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define KDEV_PING_REPLY 0x4B444556u /* "KDEV" */

struct kdev_info {
	__u32 version;
	__u32 open_count;
	char  name[16];
};

struct kdev_add {
	__s32 a;
	__s32 b;
	__s64 sum; /* filled by kernel */
};

#define KDEV_MAGIC 'K'
#define KDEV_IOC_PING     _IOR(KDEV_MAGIC, 1, __u32)
#define KDEV_IOC_GET_INFO _IOR(KDEV_MAGIC, 2, struct kdev_info)
#define KDEV_IOC_ADD      _IOWR(KDEV_MAGIC, 3, struct kdev_add)

#endif
