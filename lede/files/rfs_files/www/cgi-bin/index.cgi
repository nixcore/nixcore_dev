#!/bin/sh

# Include Nixcore functions
. /usr/sbin/nixcore.sh


echo "Content-type: text/html"
echo
echo "<html><head>"
echo "<title>NixCore</title>"
echo "<link rel='stylesheet' type='text/css' href='../main.css'>"
echo "</head><body>"
echo "<div id='doc'>"
    echo "<div id='allcontent' style='border-bottom:1px solid #000000;background:#EEEEEE'>"
        echo "<div id='headerpic'>"
            echo "<img src='../header_new.jpg'>"
        echo "</div>"
        echo "<div style='padding:10px'>"
        echo "<a href='../index.html'>Home</a><br /><hr/><br />"
# Boilerplate
################################

echo "<h2>Info about system</h2><hr /><br />"
echo "<h3>Hostname</h3>"
uci get system.@system[0].hostname
echo "<h3>OpenWRT Info</h3><pre>"
cat /etc/openwrt_release
echo "</pre>"
echo "<h3>Date</h3>"
date
echo "<h3>System load</h3>"
idle.sh
echo "<h3>Processes</h3><pre>"
ps
echo "</pre>"
echo "<h3>Network</h3><pre>"
ifconfig
echo "</pre>"
echo "<h3>Test</h3>"
nixtest


################################
# Boilerplate
        echo "</div>"
    echo "</div>"
    echo "</div>"
    echo "<div id='footer'>"
echo "Nixcore"
    echo "</div>"
echo "</div>"
echo "</div></body></html>"
