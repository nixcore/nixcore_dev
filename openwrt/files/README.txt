NixCore OpenWRT Files 1.0
=========================

We are working to get NixCore support integrated into the main branch
of OpenWRT.  There are a number of steps we need to go through to get
approval from the main OpenWRT developers.

Until we get full trunk support in OpenWRT we are providing all files
needed to get a OpenWRT build for NixCore X1.

Install:
========

We have provided a setup.sh bash script which will copy all packages
and NixCore X1 target files to the correct place within a local copy
of the OpenWRT branch.  There should only be a single variable within
the setup file that needs to be modified for any system.

Modify the OPENWRT_DIR variable at the top of the script to point to
the location of the OpenWRT branch. By default the script assumes that
the source is in a directory named "openwrt" one level up from where
it is executed from.

Custom location example:
OPENWRT_DIR="/home/USER/openwrt_branch_XXX"

Execute:
========
Ensure the setup.sh script is executable (chmod 755 setup.sh) and then
execute from within the directory:  ./setup.sh

About Packages:
===============
NixCore has two packages that need to be added to the OpenWRT trunk,
libconfuse and a package for a custom NixCore kernel module.  T

About Target:
============= 
Obviously we need to add a NixCore X1 target to the OpenWRT source so
the correct values will be set for the build.  We provide all the files
to add this target to the OpenWRT branch
