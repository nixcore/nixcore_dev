#!/bin/sh
awk '{print ($2*100)/$1 " % idle over "$1/60 " min"}' /proc/uptime
