## How I got things to build for the RG350


- I built the docker image that had the rg350 buildroot in there, this is where the tools live
- In there we also build a bunch of the dependencies / various libs and stuff which are needed
- It takes a very long time to do it, this thing checks out the code then it chunks along.
- The test-thing I used to build was https://github.com/RafaVico/rg350_test it seemed like a minimal, "hello world" tier thing to build
- I had to modify the Makefile a bit so the path to the linker/compiler would find the files.

For this file, here are the settings:

```
CC    := /root/buildroot/output/host/bin/mipsel-rg350-linux-uclibc-g++
STRIP := /root/buildroot/output/host/bin/mipsel-rg350-linux-uclibc-strip
LIBS  := -L/root/buildroot/output/host/mipsel-rg350-linux-uclibc/sysroot/usr/lib
INCS	:= -I/root/buildroot/output/host/mipsel-rg350-linux-uclibc/sysroot/usr/include
```

- After building it, you build the OPK which is a squashfs image thing
- For fun you can mount it with `mount squashy.opk /mnt/thing -type squashfs`

I had to include some `.so` files in the bundle once when it would fail at runtime. When this happened I also had to changed the `Exec:` line of the manifest of the opk to include `env LD_LIBRARY_PATH=.` in front of the executable (like `./executable.gcw`)

Then I `scp`'d it over onto the device (after plugging into it via USB-C).

     
     
     
     
