#!/bin/sh
cat /proc/uptime /tmp/uptime|tr '\n' ' '|awk '{A=$1-$3;B=$2-$4;print (B*100)/A " % idle over "A/60 " min"}'
