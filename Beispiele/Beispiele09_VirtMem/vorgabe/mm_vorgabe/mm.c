/* Michael Schoettner, 19.6.2018 */
#include <linux/module.h>		/* printk and module stuff 	*/
#include <linux/fs.h> 			/* file_operations 		*/
#include <linux/cdev.h>			/* new device model stuff 	*/
#include <linux/uaccess.h>		/* access data from user mode   */
#include <linux/mm.h>			/* for 'struct vm_area_struct'  */
#include <linux/sched.h>		/* for 'struct task_struct' 	*/


MODULE_LICENSE("GPL");


#define DEV_NUMBER	255


static struct cdev *driver_object = NULL;
static char kbuf[4096];

ssize_t mm_read( struct file *file, char *buf, size_t len, loff_t *pos );
ssize_t mm_write( struct file *file, const char *buf, size_t len, loff_t *pos );

static struct file_operations fops = {
		write:	mm_write,
		read:	mm_read
};

/* 
   read function 
*/
ssize_t mm_read( struct file *file, char *buf, size_t len, loff_t *pos ) {
	struct task_struct	*tsk = current;
	struct mm_struct	*mm = tsk->mm;
	int			l = 0;
	int 			notcopied, tocopy;

	printk( "mm: read called\n");

	// display title
	l = sprintf( kbuf, "\nMessage from the Memory Management module" );


        /*
          hier muss Code eingefuegt
        */


	tocopy = len;
	if (l<len) tocopy = l;

	notcopied = copy_to_user(buf, kbuf, tocopy);
	printk( "mm: tocopy=%d\n", tocopy);

	return tocopy - notcopied;
}

/*
   write function
*/
ssize_t mm_write( struct file *file, const char *buf, size_t len, loff_t *pos ) {
	printk( "mm: write called\n");
	return	0;
}


/* 
   init module, register device 
*/
static int mm_init(void) {
    dev_t device_number = MKDEV(DEV_NUMBER,0);
    char  *name="mm";
    int count = 1;

    if( register_chrdev_region( device_number, count, name ) ) {
        printk("mm: error, devicenumber 0x%x not available ...\n", device_number );
        return -1;
    }

    driver_object = cdev_alloc();
    if( driver_object==NULL ) {
        printk("mm: error, cdev_alloc failed ...\n");
        unregister_chrdev_region( device_number, count );
        return -1;
    }

    kobject_set_name(&driver_object->kobj, name );
    driver_object->owner = THIS_MODULE;

    cdev_init( driver_object, &fops );
    if( cdev_add( driver_object, device_number, count ) ) {
        printk("mm: error, cdev_add failed ...\n");
    	kobject_put(&driver_object->kobj);
    	driver_object = NULL;
    	unregister_chrdev_region( device_number, count );
        return -1;
    }
    printk("mm: module loaded successfully.\n");

    return 0;
}


/* 
   cleanup module
*/
static void mm_exit(void) {
    dev_t device_number = MKDEV(DEV_NUMBER,0);

    if( driver_object ) {
    printk("mm: del cdev.\n");
        cdev_del( driver_object );
    }
    unregister_chrdev_region( device_number, 1 );
    printk("mm: module unloaded\n");
}


module_init( mm_init );
module_exit( mm_exit );
