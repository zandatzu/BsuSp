#include <linux/module.h>		     /* printk and module stuff 	*/
#include <linux/fs.h> 		     	 /* file_operations 		    */
#include <linux/cdev.h>			     /* device model stuff 	        */
#include <linux/uaccess.h>           /* copy_to_user, copy_from_user*/


#define NR_OF_MINOR_NUMBERS   1
#define MEM_SIZE              4096   /* nr. of bytes for storage    */

static char storage[MEM_SIZE];       /* string storage              */

static struct cdev *driver_object = NULL;

static dev_t  device_number;         /* dynamically allocated       */
static char *device_name = "BScdev"; /* name appearing in           */ 
                                     /* /proc/devices and /sys      */

/* function declarations */
ssize_t my_cdev_read( struct file *file, char *buf, size_t len, loff_t *pos );
ssize_t my_cdev_write( struct file *file, const char *buf, size_t len, loff_t *pos );


/* my implemented file ops */
static struct file_operations fops = {
	write:	my_cdev_write,
	read:	my_cdev_read
};


static int my_cdev_init(void) {
    printk("my_cdev: init function called.\n");

    if ( alloc_chrdev_region( &device_number, 0, NR_OF_MINOR_NUMBERS, device_name ) ) {
        printk("my_cdev: error, could not alloc device number.\n");
        return -1;
    }

    driver_object = cdev_alloc();
    if( driver_object == NULL ) {
        printk("my_cdev: error, could not alloc driver object.\n");
        unregister_chrdev_region( device_number, NR_OF_MINOR_NUMBERS );
        return -1;
    }

    kobject_set_name(&driver_object->kobj, device_name );
    driver_object->owner = THIS_MODULE;

    cdev_init( driver_object, &fops );
    if( cdev_add( driver_object, device_number, NR_OF_MINOR_NUMBERS ) ) {
        printk("my_cdev: error, could not register device object.\n");
    	kobject_put(&driver_object->kobj);
    	driver_object = NULL;
    	unregister_chrdev_region( device_number, NR_OF_MINOR_NUMBERS );
        return -1;
    }

    /* init storage array */    
    storage[0] = '\0';

    printk("my_cdev: device driver initialized successfully.\n");
    return 0;
}


static void my_cdev_exit(void) {
    if( driver_object )
        cdev_del( driver_object );
    unregister_chrdev_region( device_number, NR_OF_MINOR_NUMBERS );
    printk("my_cdev: module unloaded\n");
}


ssize_t my_cdev_read( struct file *file, char *buf, size_t len, loff_t *pos ) {
    unsigned long slen;
    unsigned long not_copied;

	printk( "my_cdev: read called ('pos' ignored)\n");

    /* read string always from beginning of storage */
    slen = strlen(storage);
    if (len > slen)
      len = slen;

    /* returns number of not copied bytes */
    not_copied = copy_to_user(buf, storage, len);

	return len - not_copied;  /* number of bytes read */
}


ssize_t my_cdev_write( struct file *file, const char *buf, size_t len, loff_t *pos ) {
    unsigned long not_copied;

	printk( "my_cdev: write called ('pos' ignored)\n");

    /* write string always from beginning of storage */
    if ( len > MEM_SIZE-1 ) 
       len = MEM_SIZE-1;

    /* returns number of not copied bytes */
    not_copied = copy_from_user(storage, buf, len);
    storage[len+1] = '\0';

	return len - not_copied;  /* number of bytes written */
}


module_init( my_cdev_init );
module_exit( my_cdev_exit );
MODULE_LICENSE("GPL"); 

