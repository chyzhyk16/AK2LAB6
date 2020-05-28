
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint howmany = 1;
module_param(howmany, uint, S_IRUGO);
MODULE_PARM_DESC(howmany, "Unsigned int, determines how many lines will be printed");

struct my_data {
	struct list_head list_node;
	ktime_t time_;
};
static LIST_HEAD(my_list_head);

static int __init hello_init(void)
{
	int err = -EINVAL;
	uint i;
	ktime_t time_;
	struct my_data *ptr, *temp;

	BUG_ON(howmany > 10);
	if (howmany == 0 || (howmany >= 5 && howmany <= 10))
		pr_info("Warning! 5 <= argc <= 10\n");
	BUG_ON(!list_empty(&my_list_head));
	for (i = 0; i < howmany; i++) {
		time_ = ktime_get();
		pr_info("Hello, world!\n");
		ptr = (struct my_data *) kmalloc(sizeof(struct my_data), GFP_KERNEL);
		if (i == 4)
			ptr = NULL;
		*ptr = (struct my_data) {
			(struct list_head) {NULL, NULL}, time_
		};
		list_add_tail(&ptr->list_node, &my_list_head);
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct my_data *ptr, *temp;
	list_for_each_entry_safe(ptr, temp, &my_list_head, list_node)
	{
		printk(KERN_INFO "time=%lld\n", ptr->time_);
		list_del(&ptr->list_node);
		kfree(ptr);
	}
	printk(KERN_EMERG "Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);
