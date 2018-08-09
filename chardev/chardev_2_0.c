#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // для передачи данных из user space в kernel space и обратно
#include <linux/init.h> // для правильной инициализации модуля
#include <linux/slab.h> // слаполокатор (выделение памяти)
#include <linux/cdev.h> // функции для определения символьных девайсов, символьных устройств или модулей


#define MYDEV_NAME "mychardev" // имя символьного устройства, которое видно пользователю
#define KBUF_SIZE (size_t)((10) * PAGE_SIZE)

static char *kbuf;
static dev_t first; // идентификатор первого устройства в цепочке
static unsigned int count = 1; // счетчик устройств
static int char_major = 700, char_minor = 0; // старший и младший номер устройства
static struct cdev *char_cdev; /*
                               **стуркура описывает все операции которые
                               **можем проводить над символьным устройством
                               */

// открытие файла устройства
static int mychardev_open(struct inode *inode, struct file *file)
{
    printk( KERN_INFO "__Opening devise: %s \n\n", MYDEV_NAME);
    return 0;
}

// закрытие файла устройства
static int mychardev_release(struct inode *inode, struct file *file)
{
    printk( KERN_INFO "__Releasing devise: %s \n\n", MYDEV_NAME);
    return 0;
}

static ssize_t mychardev_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
    int nbytes = lbuf - copy_to_user(buf, kbuf + *ppos , lbuf); // подсчитываем колличество байт
    *ppos += nbytes; // подвигаем позицию для чтения

    printk( KERN_INFO "__Read: %s, nbytes = %d, pos = %d \n\n", MYDEV_NAME, nbytes, (int)*ppos);

    return nbytes;
}

static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
    int nbytes = lbuf - copy_from_user(kbuf + *ppos, buf, lbuf); // подсчитываем колличество байт
    *ppos += nbytes; // подвигаем позицию для чтения

    printk( KERN_INFO "__Write: %s, nbytes = %d, pos = %d \n\n", MYDEV_NAME, nbytes, (int)*ppos);

    return nbytes;
}

static const struct file_operations charcdev_fops = {
    .owner = THIS_MODULE,
    .open = mychardev_open,
    .read = mychardev_read,
    .write = mychardev_write,
    .release = mychardev_release,
};

//Инициализация модуля
//(принято в написании модулей, если функция не принимает аргументы использовать 'void')
static int __init init_chardev(void)
{
    printk(KERN_INFO "Loading My-Module");
    kbuf = kmalloc(KBUF_SIZE, GFP_KERNEL); // выделение памяти из стандартого пула ядра 'GFP_KERNEL'

    first = MKDEV(char_major, char_minor); // инициализация первого устройства со старшим и младшим номером

    register_chrdev_region(first, count, MYDEV_NAME); //регистрация устройства
    char_cdev = cdev_alloc();

    cdev_init(char_cdev, &charcdev_fops);
    cdev_add(char_cdev, first, count);
    return 0;
}

//Деинициализация модуля
static void __exit clennup_chardev(void)
{
    printk(KERN_INFO "Leaving My-Module");

    if(char_cdev)
        cdev_del(char_cdev);

    unregister_chrdev_region(first, count);//удаление устройства
}

//Функция отвечающая за загрузку модуля
module_init ( init_chardev );
//Функция отвечающая за выгрузку модуля
module_exit ( clennup_chardev )

//Определение лицензии
MODULE_LICENSE("GLP");
