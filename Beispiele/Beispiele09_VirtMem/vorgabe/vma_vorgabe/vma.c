/* Michael Schoettner, 13.6.2018 */
#include <linux/module.h>		/* printk and module stuff 	*/
#include <linux/fs.h> 			/* file_operations 		*/
#include <linux/cdev.h>			/* new device model stuff 	*/
#include <linux/uaccess.h>		/* access data from user mode   */
#include <linux/mm.h>			/* for 'struct vm_area_struct'  */
#include <linux/sched.h>		/* for 'struct task_struct' 	*/

MODULE_LICENSE("GPL");


#define DEV_NUMBER	256


static struct cdev *driver_object = NULL;
static char kbuf[4096];

ssize_t vma_read( struct file *file, char *buf, size_t len, loff_t *pos );
ssize_t vma_write( struct file *file, const char *buf, size_t len, loff_t *pos );

static struct file_operations fops = {
		write:	vma_write,
		read:	vma_read
};

/* 
   read function 
*/
ssize_t vma_read( struct file *file, char *buf, size_t len, loff_t *pos ) {
	struct task_struct	*tsk = current;
	struct vm_area_struct	*vma;
	int			i = 0, l = 0;
	int 			notcopied, tocopy;

	printk( "vma: read called\n");

	// display title
	l += sprintf( kbuf+l, "Message from the Virtual Memory Area module " );


       /*
          hier muss Code eingefuegt werden
       */


	tocopy = len;
	if (l<len) tocopy = l;

	notcopied = copy_to_user(buf, kbuf, tocopy);
	printk( "vma: tocopy=%d\n", tocopy);

	return tocopy - notcopied;
}

/* 
   write function 
*/
ssize_t vma_write( struct file *file, const char *buf, size_t len, loff_t *pos ) {
	printk( "vma: write called\n");
	return	0;
}


/* 
   init module, register device 
*/
static int vma_initf(void) {
    dev_t device_number = MKDEV(DEV_NUMBER,0);
    char  *name="vma";
    int count = 1;

    if( register_chrdev_region( device_number, count, name ) ) {
        printk("vma: error, devicenumber 0x%x not available ...\n", device_number );
        return -1;
    }

    driver_object = cdev_alloc();
    if( driver_object==NULL ) {
        printk("vma: error, cdev_alloc failed ...\n");
        unregister_chrdev_region( device_number, count );
        return -1;
    }

    kobject_set_name(&driver_object->kobj, name );
    driver_object->owner = THIS_MODULE;

    cdev_init( driver_object, &fops );
    if( cdev_add( driver_object, device_number, count ) ) {
        printk("vma: error, cdev_add failed ...\n");
    	kobject_put(&driver_object->kobj);
    	driver_object = NULL;
    	unregister_chrdev_region( device_number, count );
        return -1;
    }
    printk("vma: module loaded successfully.\n");

    return 0;
}


/* 
   cleanup module
*/
static void vma_exit(void) {
    dev_t device_number = MKDEV(DEV_NUMBER,0);

    if( driver_object ) {
    printk("vma: del cdev.\n");
        cdev_del( driver_object );
    }
    unregister_chrdev_region( device_number, 1 );
    printk("vma: module unloaded\n");
}


module_init( vma_initf );
module_exit( vma_exit );
