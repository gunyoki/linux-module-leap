#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define PROC_NAME "driver/utime"

static int pid;
module_param(pid, int, S_IRUGO | S_IWUSR);

static int proc_read(char *page, char **start, off_t offset, int count, int *eof, void *data)
{
	unsigned long outbyte;
	struct task_struct *process;
	if (offset > 0) {
		*eof = 1;
		return 0;
	}
	process = pid_task(find_vpid(pid), PIDTYPE_PID);
	if (!process) { 
		printk("pid error: %d\n", pid);
		*eof = 1;
		return 0;
	}
	outbyte = sprintf(page, "%ld", process->utime);
	printk("proc_read len = %lu\n", outbyte);
	*eof = 1;
	return outbyte;
}

static int proc_write(struct file *page, const char *buf, unsigned long len, void *data)
{
	char tmp[64];
	long utime;
	struct task_struct *process;
	if (copy_from_user(tmp, buf, len))
		return -EFAULT;
	utime = simple_strtol(tmp, NULL, 0);
	process = pid_task(find_vpid(pid), PIDTYPE_PID);
	if (!process) { 
		printk("pid error: %d\n", pid);
		return -EFAULT;
	}
	printk("utime: %ld -> %ld\n", process->utime, utime);
	process->utime = utime;
	return len;
}

int init_module(void)
{
	struct proc_dir_entry *entry;
	entry = create_proc_entry(PROC_NAME, 0666, NULL);
	if (entry == 0)
		return -EINVAL;
	entry->read_proc = proc_read;
	entry->write_proc = proc_write;
	return 0;
}

void cleanup_module(void)
{
	remove_proc_entry(PROC_NAME, NULL);
}

MODULE_LICENSE("GPL");
