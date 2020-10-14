#!/usr/bin/env bash
set -e
cd "$(dirname "${BASH_SOURCE[0]}")"

# Name of the result opk file
OPK_NAME='hello_triangle.opk'

# These are the files/dirs to feed into the bundle
FLIST="media"
FLIST="${FLIST} program.gcw"
FLIST="${FLIST} icon.png"
FLIST="${FLIST} default.gcw0.desktop"

# Launch this script from inside of the rg350_build container
if [[ "$1" != "INSIDE_CONTAINER" ]]; then
    docker run -it -v $(pwd):/root/source rg350_build /root/source/build.sh INSIDE_CONTAINER

    # Install if run with --install arg
    if [[ "$1" == "--install" ]]; then
        scp "./$OPK_NAME" "root@10.1.1.2:/media/data/apps/$OPK_NAME"
    fi

    exit 0
fi

# Build the latest binary
make

# The manifest is called "default.gcw0.desktop", fill it in here.
cat > default.gcw0.desktop <<EOF
[Desktop Entry]
Name=Hello OpenGL
Comment=Test Opengl
Exec=env LD_LIBRARY_PATH=. ./program.gcw
Terminal=false
Type=Application
StartupNotify=true
Icon=icon
Categories=applications;
EOF

# Here is where the magic happens, delete the old one, create a new one
rm -f ${OPK_NAME}
mksquashfs ${FLIST} ${OPK_NAME} -all-root -no-xattrs -noappend -no-exports

# Clean up
rm -f default.gcw0.desktop
rm -f program.gcw
