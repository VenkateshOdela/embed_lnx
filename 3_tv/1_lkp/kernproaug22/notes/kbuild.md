## Download and build Linux kernel source 

####   1.1 Download tools needed

​	

**Ubuntu machines:** 

```shell
   $ sudo apt-get install libncurses5-dev libssl-dev git exuberant-ctags cscope build-essentials
```

**Fedora machines:**

```shell
   $ sudo yum install git ctags ncurses-devel openssl-devel
```

  **Clone kernel source** ****

```shell
$ mkdir ~/sources
$ cd ~/sources
$ git clone git://git.kernel.org/pub/scm/linux/kernel/git/stable/
		 linux-stable.git
$ cd linux-stable
```


  Find the latest stable kernel by looking for the largest vX.Y.Z values,tags that end with -rcX are beta/test kernels.

```shell
 $ git checkout -b stable ver.tag
```

  	 Note:ver.tag is the latest vX.Y.Z tag you found. 



#### 2.0 Kernel build

1. choose kernel configuration

2. initiate compile & build

```shell
	$ make -j4

		1. open .config
		2. identify source file(s) for each service selected 
	       by stepping into appropriate branch
	
		3. Compile identified service code into relocatables using kbuild 
	       script
	
		4. Integrate all relocatables of the options chosen statically 
	       to generate ELF(raw)kernel image (vmlinux image)
	
		5. invoke target arch specific build script(Arch Makefile) to generate loadable kernel image.
	   		Arch Makefile script does the following:
					- Appends arch specific boot-strap 
						- enable CPU's MMU
						- set up page tables
						- invokes start function of kernel proper
					- packages kernel image into loadable format as required by the bootloader
	              		(Appends Boot-loader specific header defining image type, load and entry 
	               		 addresses.)
	
	 6. Build module object files(.ko) from relocatables of "=m" entries 
	        from .config file
	
	 In summary following files are generated
	     
	   1. Raw(ELF) kernel image with symbol tables: vmlinux (located in source root)
	   2. Composite kernel image: vmlinuz (located in arch/$ARCH/boot folder)
	   3. Loadable kernel image: bzImage|uImage|zImage (located in arch/$ARCH/boot folder)
	   4. Dynamically loadable kernel modules: .ko files(spread through the source tree)
```



3. Move Module images onto Root file system(RFS)

```shell
  $ make modules_install 
```

Move module images onto root file-system's in-tree modules folder (/lib/modules/(kernel_version_string)/)

Folder is created matching kernels version tag. This folder is also populated with kernel-headers for supporting out-of-tree module build.

4. Install kernel Image & Configure boot loader 

```shell
$ make install 

	 1. copies loadable kernel image and other relavent 
	    files into /boot folder of root file system.
	 2. updates bootloader with menu entry for new kernel
```



> #### Integrating New services into kernel source tree

1. ​	step 1: Move source file into appropriate branch of source tree

2. ​	step 2: Add new listing into kernel configuration Menu

   ​		Append new config entry describing new service and its dependencies into Kconfig file of the local branch

3. ​	step 3: Modify Kbuild scripts to compile and build new service as per chosen method

   ​			Edit local Kbuild Makefile to have compile target for new source





