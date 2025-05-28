/*
 *
 *
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/gfp.h>

struct kmem_cache *mycache;
void *object;

int kmem_init(void)
{
	  //SLAB_RED_ZONE for buffer overrun: addtional bytes added
	  //SLAB_POISON to check uninitilized cache read
       mycache = kmem_cache_create("test_cache", 128, 0, SLAB_POISON, NULL); // 128bytes
       if (!mycache)
	    return -ENOMEM;
	
	pr_info("cache size %d\n", kmem_cache_size(mycache));

	object = kmem_cache_alloc(mycache, GFP_KERNEL); //get unused object to mycache
	 if (!object)
                return -ENOMEM;

	pr_info("hexdump after alloc\n");
        print_hex_dump_bytes("kptr: ", DUMP_PREFIX_OFFSET, object, 136);

	return 0;
}

void kmem_exit(void)
{
	kmem_cache_free(mycache, object); //  kmem_cache_alloc() ->  kmem_cache_free()

        print_hex_dump_bytes("kptr: ", DUMP_PREFIX_OFFSET, object, 136);
	kmem_cache_destroy(mycache); //kmem_cache_create() -> kmem_cache_destroy()
}

module_init(kmem_init);
module_exit(kmem_exit);

MODULE_AUTHOR("Raghu Bhardwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("LKD_CW: cache");
MODULE_LICENSE("Dual MIT/GPL");

/*
 *  WITH SLAB_POISON :
 *
 * [175940.246057] cache size 128
[175940.246077] hexdump after alloc
[175940.246081] kptr: 00000000: 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b  kkkkkkkkkkkkkkkk
[175940.246083] kptr: 00000010: 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b  kkkkkkkkkkkkkkkk
[175940.246085] kptr: 00000020: 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b  kkkkkkkkkkkkkkkk
[175940.246086] kptr: 00000030: 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b  kkkkkkkkkkkkkkkk
[175940.246088] kptr: 00000040: 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b  kkkkkkkkkkkkkkkk
[175940.246090] kptr: 00000050: 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b  kkkkkkkkkkkkkkkk
[175940.246091] kptr: 00000060: 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b  kkkkkkkkkkkkkkkk
[175940.246093] kptr: 00000070: 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b 6b a5  kkkkkkkkkkkkkkk.
[175940.246095] kptr: 00000080: 63 19 0a 92 b9 d0 02 b8                          c.......
 *
 */

/*
 *  sudo cat /proc/slabinfo
 *
 *  kmem_cache_node      320    320     64   64    1 : tunables    0    0    0 : slabdata      5      5      0
    kmem_cache           216    216    448   36    4 : tunables    0    0    0 : slabdata      6      6      0
 *
 *  slabinfo - version: 2.1
# name       <active_objs> <num_objs> <objsize> <objperslab> <pagesperslab> : tunables <limit> <batchcount> <sharedfactor> : slabdata <active_slabs> <num_slabs> <sharedavail>
test_cache      1              60         136        60             2        : tunables    0            0         0 :         slabdata      1      1      0
 *
 *
 */
