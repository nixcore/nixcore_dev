#!/bin/bash

# This points to where OpenWRT is
OPENWRT_DIR="../openwrt"
# This points to where OpenWRT wants our RFS files to be
OPENWRT_FILES_LNK="$OPENWRT_DIR/files"

# This points to where our modified OpenWRT files are
MY_FILES_DIR="./"
# This is the folder our RFS files are in within the modified
# files directory
RFS_FILES="rfs_files"

# Now, put everything in the right place

if [ ! -e $OPENWRT_FILES_LNK ] ; then
    echo "Files directory doesn't exist, creating symlink"
    ln -s $PWD/$MYFILES_DIR/$RFS_FILES/ $OPENWRT_FILES_LNK
fi

# Copy the packages
echo "Copying packages..."

echo "libconfuse..."
cp $MY_FILES_DIR/confuse $OPENWRT_DIR/package/libs/ -rf

echo "Nixcore Kernel modules..."
cp $MY_FILES_DIR/nixcore $OPENWRT_DIR/package/kernel/ -rf

echo "NixCore X1 profile..."
cp $MY_FILES_DIR/NIXCORE-X1.dts $OPENWRT_DIR/target/linux/ramips/dts/
cp $MY_FILES_DIR/nixcore.mk $OPENWRT_DIR/target/linux/ramips/rt305x/profiles/
# Should we copy this over?
cp $MY_FILES_DIR/Makefile $OPENWRT_DIR/target/linux/ramips/image/

echo "NixCore Version file..."
cp $RFS_FILES/etc/nixcore_version $OPENWRT_DIR/version.txt
