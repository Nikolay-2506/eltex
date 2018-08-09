#include "mod_proc.h"
#include "fops_rw.c"         // чтение-запись для /proc/mod_node 

/*static const struct file_operations node_fops = {
   .owner = THIS_MODULE,
   .read  = node_read,
   .write  = node_write
};*/

static const struct file_operations node_fops = {
   .owner = THIS_MODULE,
   .write  = node_write,
   .read  = node_read
   
};

static int __init proc_init( void ) {
   int ret;
   struct proc_dir_entry *own_proc_node;
   /* S_IFREG-> Regular file
    * | S_IRUGO -> (S_IRUSR|S_IRGRP|S_IROTH) -> (read permission, owner| read permission, group | read permission, other)
    * |
    *  create_proc_entry (Имя записи которое хотим создать, Права при создании записи, указать на родительскую директорию)
    * own_proc_node = create_proc_entry( NAME_NODE, S_IFREG | S_IRUGO | S_IWUGO, NULL );
    */
   own_proc_node = proc_create(NAME_NODE, S_IFREG | S_IRUGO | S_IWUGO, NULL, &node_fops);
   if( NULL == own_proc_node ) {
      ret = -ENOENT;
      ERR( "can't create /proc/%s", NAME_NODE );
      goto err_node;
   }
   LOG( "/proc/%s installed", NAME_NODE );
   return 0;
err_node:
   return ret;
}

static void __exit proc_exit( void ) {
   remove_proc_entry( NAME_NODE, NULL );
   LOG( "/proc/%s removed", NAME_NODE );
} 

