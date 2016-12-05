#!/bin/sh

# This script blinks gpio27 at 2Hz for testing

LOOPS=1000

echo "out" > /sys/class/gpio/gpio27/direction
until [  $LOOPS -lt 11 ]; do
    #echo COUNTER $COUNTER
    echo "1" > /sys/class/gpio/gpio27/value
    sleep 1
    echo "0" > /sys/class/gpio/gpio27/value
    sleep 1
    let LOOPS-=1
done
