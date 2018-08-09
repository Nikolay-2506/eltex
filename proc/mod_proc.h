#include <linux/module.h>
#include <linux/stat.h>
#include <linux/uaccess.h>
#include "common.h"
#include <linux/proc_fs.h>

MODULE_LICENSE( "GPL" );

static int  __init proc_init( void );
static void __exit proc_exit( void );

module_init( proc_init );
module_exit( proc_exit ); 

