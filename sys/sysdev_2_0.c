#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

/*
 * This module shows how to create a simple subdirectory in sysfs called
 * /sys/kernel/kobject-example  In that directory, file are created:
 * "foo".
 */

static int foo;
static struct kobject *example_kobj;

/*
 * The "foo" file where a static variable is read from and written to.
 */
static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	int result;
	printk(KERN_INFO "read: %d", foo);
	result = sprintf(buf, "%d\n", foo);
	printk(KERN_INFO "number of characters written: %d, buf = %d",sprintf(buf, "%d\n", foo), *buf);
	return result;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	int ret;
	printk(KERN_INFO "write: %d", *buf);
	ret = kstrtoint(buf, 10, &foo);
	if (ret < 0)
		return ret;
	printk(KERN_INFO "foo = %d", foo);
	return count;
}

/* Sysfs attributes cannot be world-writable. */
static struct kobj_attribute foo_attribute = __ATTR(foo, 0664, foo_show, foo_store);

static int __init example_init(void)
{
	int retval;

	/*
	 * Create a simple kobject with the name of "kobject_example",
	 * located under /sys/kernel/
	 *
	 * As this is a simple directory, no uevent will be sent to
	 * userspace.  That is why this function should not be used for
	 * any type of dynamic kobjects, where the name and number are
	 * not known ahead of time.
	 */
	example_kobj = kobject_create_and_add("kobject_example", kernel_kobj);
	if (!example_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_file(example_kobj, &foo_attribute.attr);
	if (retval)
		kobject_put(example_kobj);

	return retval;
}

static void __exit example_exit(void)
{
	kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL v2");
