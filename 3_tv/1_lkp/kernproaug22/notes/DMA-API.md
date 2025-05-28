> ## Kernel DMA Mapping Interface

The DMA API describes two types of memory architecture, called "consistent" and "non-consistent"(or sometimes called 
"coherent" and "non-coherent"). 

With consistent-memory architectures, changes to memory contents (even when done by a device) will cause any cached data 
to be updated or invalidated. As a result, a device or CPU can read memory immediately after a device or CPU writes to it
without having to worry about caching effects (though the DMA API notes that the CPU cache may need to be flushed before 
devices can read). 

Much of the x86 world deals with consistent memory (with some exceptions, usually dealing with GPUs), however in the Arm 
world, we see many devices that are not coherent with the CPU and are thus non-consistent-memory architectures. 
Arm64 devices gain functionality like PCIe, there can often be a mix of coherent and non-coherent devices on a system.

> #### large DMA-coherent Allocations

Consistent memory is memory for which a write by either the device or the processor can immediately be read by the processor or device without having to worry about caching effects.

```C
void *dma_alloc_coherent(struct device *dev, size_t size,
			     dma_addr_t *dma_handle, gfp_t flag)
```

This routine allocates a region of <size> bytes of consistent memory. It returns a pointer to the allocated region (in the kernel linear address space) or NULL if the allocation failed.

It also returns a <dma_handle> which can be given to the device as the DMA address base of the region.

```C
void *dma_zalloc_coherent(struct device *dev, size_t size,
			     dma_addr_t *dma_handle, gfp_t flag)
```

Wraps dma_alloc_coherent() and also zeroes the returned memory if the allocation attempt succeeded.

```c
void dma_free_coherent(struct device *dev, size_t size, void *cpu_addr,
			   dma_addr_t dma_handle)
```

Free a region of consistent memory previously allocated. dev, size and dma_handle must all be the same as those passed into
dma_alloc_coherent().  cpu_addr must be the linear address returned by the dma_alloc_coherent().



> #### small DMA-coherent buffers

Many drivers need lots of small DMA-coherent memory regions for DMA descriptors or I/O buffers.  Rather than allocating in units of a page or more using dma_alloc_coherent(), you can use DMA pools.  These work much like a struct kmem_cache, except that they use the DMA-coherent allocator, not __get_free_pages().


```c
struct dma_pool *dma_pool_create(const char *name, struct device *dev,
		size_t size, size_t align, size_t alloc);
```

dma_pool_create() initializes a pool of DMA-coherent buffers for use with a given device.  It must be called in a context which
can sleep. The "name" is for diagnostics (like a struct kmem_cache name); dev and size are like what you'd pass to dma_alloc_coherent().  The device's hardware alignment requirement for this type of data is "align" (which is expressed in bytes, and must be a power of two).  If your device has no boundary crossing restrictions, pass 0 for alloc; passing 4096 says memory allocated from this pool must not cross 4KByte boundaries.


```C
void *dma_pool_zalloc(struct dma_pool *pool, gfp_t mem_flags, dma_addr_t *handle)
```

Wraps dma_pool_alloc() and also zeroes the returned memory if the allocation attempt succeeded.


```C
void *dma_pool_alloc(struct dma_pool *pool, gfp_t gfp_flags, dma_addr_t *dma_handle);
```

This allocates memory from the pool; the returned memory will meet the size and alignment requirements specified at creation time.


```C
void dma_pool_free(struct dma_pool *pool, void *vaddr, dma_addr_t addr);
```

This puts memory back into the pool.  The pool is what was passed to dma_pool_alloc(); the CPU (vaddr) and DMA addresses are what were returned when that routine allocated the memory being freed.


```C
void dma_pool_destroy(struct dma_pool *pool);
```

dma_pool_destroy() frees the resources of the pool.  It must be called in a context which can sleep.  Make sure you've freed all allocated memory back to the pool before you destroy it.

>
> #### Streaming DMA mappings
>

```C
dma_addr_t
dma_map_single(struct device *dev, void *cpu_addr, size_t size,
		      enum dma_data_direction direction)
```

Maps a piece of processor linear memory so it can be accessed by the device and returns the DMA address of the memory.

The direction can be any of the following:

| DMA_NONE          | No direction (used for debugging)            |
| ----------------- | -------------------------------------------- |
| DMA_TO_DEVICE     | Data is moving from memory to device         |
| DMA_FROM_DEVICE   | Data is coming from the device to the memory |
| DMA_BIDIRECTIONAL | data flow is bidirectional                   |

DMA_TO_DEVICE synchronisation is done after the last modification of the memory region by the software and before it is handed off to the device.  Once this primitive is used, memory covered by this primitive should be treated as read-only by the device.  

DMA_FROM_DEVICE synchronisation is done before the driver accesses data that may be changed by the device.  This memory should be treated as read-only by the driver.

```C
void
dma_unmap_single(struct device *dev, dma_addr_t dma_addr, size_t size,
		 enum dma_data_direction direction)
```

Unmaps the region previously mapped.  All the parameters passed in must be identical to those passed in (and returned) by the mapping API.

```C
dma_addr_t
dma_map_page(struct device *dev, struct page *page,
		    unsigned long offset, size_t size,
		    enum dma_data_direction direction)
void
dma_unmap_page(struct device *dev, dma_addr_t dma_address, size_t size,
	       enum dma_data_direction direction)
```

API for mapping and un-mapping for pages.  All the notes and warnings for the other mapping APIs apply here.  Also, although the <offset> and <size> parameters are provided to do partial page mapping, it is recommended that you never use these unless you really know what the cache width is.

--------------------------------------------------------------------------



```C
dma_addr_t
dma_map_resource(struct device *dev, phys_addr_t phys_addr, size_t size,
		 enum dma_data_direction dir, unsigned long attrs)

void
dma_unmap_resource(struct device *dev, dma_addr_t addr, size_t size,
		   enum dma_data_direction dir, unsigned long attrs)
```

API for mapping and unmapping for MMIO resources. All the notes and warnings for the other mapping APIs apply here. The API should only be used to map device MMIO resources, mapping of RAM is not permitted.

```C
int
dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
```

In some circumstances dma_map_single(), dma_map_page() and dma_map_resource() will fail to create a mapping. A driver can check for these errors by testing the returned DMA address with dma_mapping_error(). A non-zero return value
means the mapping could not be created and the driver should take appropriate action (e.g. reduce current DMA mapping usage or delay and try again later).


```c
int
dma_map_sg(struct device *dev, struct scatterlist *sg,
	int nents, enum dma_data_direction direction)
```

Returns: the number of DMA address segments mapped (this may be shorter than <nents> passed in if some elements of the scatter/gather list are physically or linearly adjacent and an IOMMU maps them with a single entry).

Please note that the sg cannot be mapped again if it has been mapped once. The mapping process is allowed to destroy information in the sg.

As with the other mapping interfaces, dma_map_sg() can fail. When it does, 0 is returned and a driver must take appropriate action. It is critical that the driver do something, in the case of a block driver aborting the request or even oopsing is better than doing nothing and corrupting the filesystem.

With scatterlists, you use the resulting mapping like this:

```c
int i, count = dma_map_sg(dev, sglist, nents, direction);
struct scatterlist *sg;

for_each_sg(sglist, sg, count, i) {
	hw_address[i] = sg_dma_address(sg);
	hw_len[i] = sg_dma_len(sg);
}
```

where nents is the number of entries in the sglist.

The implementation is free to merge several consecutive sglist entries into one (e.g. with an IOMMU, or if several pages just happen to be physically contiguous) and returns the actual number of sg entries it mapped them to. On failure 0, is returned.

Then you should loop count times (note: this can be less than nents times) and use sg_dma_address() and sg_dma_len() macros where you previously accessed sg->address and sg->length as shown above.

```c
void
dma_unmap_sg(struct device *dev, struct scatterlist *sg,
	int nents, enum dma_data_direction direction)
```

Unmap the previously mapped scatter/gather list.  All the parameters must be the same as those and passed in to the scatter/gather mapping API.

Note: <nents> must be the number you passed in, *not* the number of DMA address entries returned.