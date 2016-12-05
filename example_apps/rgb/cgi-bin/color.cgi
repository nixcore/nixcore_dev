#!/bin/sh

# Include Nixcore functions
. /usr/sbin/nixcore.sh


# Found at
# http://stackoverflow.com/questions/3919755/how-to-parse-query-string-from-a-bash-cgi-script
if [ "$REQUEST_METHOD" = POST ]; then
    read -n $CONTENT_LENGTH query
    #logger "query  [$query]"
    #echo "printing vals"
    saveIFS=$IFS
    IFS='&'
    eval $query
    IFS=$saveIFS
    #echo "${colorval}"
    colorval=${colorval:3}
    #http://stackoverflow.com/questions/13055889/sed-with-literal-string-not-input-file
    device=`echo $device|sed 's/%2F/\//g'`
fi

r=${colorval:0:2}
g=${colorval:2:2}
b=${colorval:4:2}

# http://stackoverflow.com/questions/13280131/hexadecimal-to-decimal-in-shell-script
r=$((0x${r}))
g=$((0x${g}))
b=$((0x${b}))

/root/rgb2.bin $r $g $b $device $num > /dev/null

#echo "Content-type: text/html"
#echo
#echo "<html><head>"
#echo "<META http-equiv='refresh' content='0;URL=../color.html?colorval=${colorval}?device=${device}?num=${num}'>"
#echo "</head><body>"
#echo "red: $r<br />"
#echo "green: $g<br />"
#echo "blue: $b<br />"
#echo "device: $device<br />"
#echo "num: $num<br />"
#echo "</body></html>"
