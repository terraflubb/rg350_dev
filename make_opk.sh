#!/bin/sh

# It feels like this could be cleaned up with a tool and
# config file. Run this after compiling to stuff all the
# things together.
OPK_NAME=rg350test_banana.opk

echo ${OPK_NAME}

# The manifest is called "default.gcw0.desktop"
# fill it in here.

cat > default.gcw0.desktop <<EOF
[Desktop Entry]
Name=RG350 test banana
Comment=Test your banana
Exec=env LD_LIBRARY_PATH=. ./rg350test.gcw
Terminal=false
Type=Application
StartupNotify=true
Icon=rg350test
Categories=applications;
EOF
# Note:
# I had problems executing it when it couldn't
# find some .so files (or the version, it was
# libxxx.so.2 it couldn't find) so the exec
# is beefed up with the LD_LIBRARY_PATH.
# I also saw somewhere where Exec just had
# some script `./foo.sh` and all the stuff
# was stuffed in there, and the file was included
# in the bundle.


# These are the files/dirs to feed into the bundle
FLIST="media"
FLIST="${FLIST} rg350test.gcw"
FLIST="${FLIST} rg350test.png"
FLIST="${FLIST} default.gcw0.desktop"

# This is the lost shared-object thing for libshake I needed for something.
FLIST="${FLIST} libshake.so.2"
cp /root/buildroot/output/host/mipsel-rg350-linux-uclibc/sysroot/usr/lib/libshake.so libshake.so.2

# Here is where the magic happens, delete the old one, create a new one
rm -f ${OPK_NAME}
mksquashfs ${FLIST} ${OPK_NAME} -all-root -no-xattrs -noappend -no-exports

# Dump the manifest for giggles, then clean up
cat default.gcw0.desktop
rm -f default.gcw0.desktop
rm libshake.so.2
