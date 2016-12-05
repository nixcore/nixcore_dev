#!/bin/sh

# Debugging
#UBUNTU=1
#SAVE_OLD_FILES=1

# Network variables
NETWORK_CONFIG_FILE="/etc/config/network"
NETWORK_INIT_SCRIPT="/etc/init.d/network"

# Wireless variables
WIRELESS_CONFIG_FILE="/etc/config/wireless"
WIRELESS_HWMODE="11g"
WIRELESS_DRIVER="mac80211"
WIRELESS_CHAN="6"
WIRELESS_PATH="10180000.wmac"
WIRELESS_HTMODE="HT20"
WIRELESS_LOG_LEVEL="0"
WIRELESS_COUNTRY="US"
WIRELESS_SSID="SomeNet"
WIRELESS_ENCRYPT="psk2"
WIRELESS_KEY="[MYKEYHERE]"
WIRELESS_TYPE="sta"
WIRELESS_NETWORK="wwan"

# Network functions
##########################################
lan_reload()
{
    if [ -n "$UBUNTU" ]; then
        echo "Reload network"
    else
        `$NETWORK_INIT_SCRIPT reload`
    fi
    return
}
lan_restart()
{
    if [ -n "$UBUNTU" ]; then
        echo "Restart network"
    else
        `$NETWORK_INIT_SCRIPT restart`
    fi
    return
}
lan_static ()
{
    return
}
lan_dhcp ()
{
    return
}

# Wireless functions
##########################################

# Turn off and then on the wifi hardware to load a new config
wifi_reload()
{
    if [ -n "$UBUNTU" ]; then
        echo "wifi down;wifi up"
    else
        wifi down;wifi up
    fi
    return
}

# Scan for all SSIDs that we can see
wifi_scan() {
    RESULTS=`iw dev wlan0 scan | grep SSID|sed "s|^.*SSID: ||g"`
    return "$RESULTS"
}

# TODO: Return info about a given SSID
wifi_get_ssid_info()
{
    return
}

# Save the wireless config.  Don't call this outside of this
# script since it will just write default values.  Always call
# wifi_sta_connect or wifi_ap_create.
wifi_write_config()
{
    DATE="` date +"%s"`"
    if [ -n "$SAVE_OLD_FILES" ]; then
        mv $WIRELESS_CONFIG_FILE "$WIRELESS_CONFIG_FILE.$DATE.old"
    else
        mv $WIRELESS_CONFIG_FILE "$WIRELESS_CONFIG_FILE.old"
    fi
    echo "config wifi-device  radio0" >> $WIRELESS_CONFIG_FILE
    echo "    option type             '$WIRELESS_DRIVER'" >> $WIRELESS_CONFIG_FILE
    echo "    option channel          '$WIRELESS_CHAN'" >> $WIRELESS_CONFIG_FILE
    echo "    option hwmode           '$WIRELESS_HWMODE'" >> $WIRELESS_CONFIG_FILE
    echo "    option path             '$WIRELESS_PATH'" >> $WIRELESS_CONFIG_FILE
    echo "    option htmode           '$WIRELESS_HTMODE'" >> $WIRELESS_CONFIG_FILE
    echo "    option log_level        '$WIRELESS_LOG_LEVEL'" >> $WIRELESS_CONFIG_FILE
    echo "    option country          '$WIRELESS_COUNTRY'" >>$WIRELESS_CONFIG_FILE
    echo "config wifi-iface" >> $WIRELESS_CONFIG_FILE
    echo "    option device           'radio0'" >> $WIRELESS_CONFIG_FILE
    echo "    option network          '$WIRELESS_NETWORK'" >> $WIRELESS_CONFIG_FILE
    echo "    option mode             '$WIRELESS_TYPE'" >> $WIRELESS_CONFIG_FILE
    echo "    option ssid             '$WIRELESS_SSID'" >> $WIRELESS_CONFIG_FILE
    echo "    option encryption       '$WIRELESS_ENCRYPT'" >> $WIRELESS_CONFIG_FILE
    echo "    option key              '$WIRELESS_KEY'" >> $WIRELESS_CONFIG_FILE

    # Could remove the old file here

    return
}

# Connect the device to an AP as a client
# Params: SSID, encryption_type, encryption_key, channel
wifi_sta_connect()
{
    # Make sure the params are good
    if [ -z $1 ]; then
        #echo "Must have AP name"
        return 1
    else
        AP=$1
    fi
    if [ -z $2 ]; then
        ENCRYPT="none"
    else
        ENCRYPT=$2
    fi
    if [ -z $3 ]; then
        KEY="none"
    else
        KEY=$3
    fi
    if [ -z $4 ]; then
        CHAN="6"
    else
        CHAN=$4
    fi

    #echo "Connect to $AP using $ENCRYPT=>'$KEY', chan $CHAN"
    WIRELESS_SSID=$AP
    WIRELESS_ENCRYPT=$ENCRYPT
    WIRELESS_KEY=$KEY
    WIRELESS_CHAN=$CHAN
    WIRELESS_NETWORK="wwan"

    wifi_write_config
    wifi_reload

    return 0
}

# Create a new AP
# Params: SSID, encryption_type, encryption_key, channel
wifi_ap_create()
{
    # Make sure the params are good
    if [ -z $1 ]; then
        #echo "Must have AP name"
        return 1
    else
        AP=$1
    fi
    if [ -z $2 ]; then
        ENCRYPT="none"
    else
        ENCRYPT=$2
    fi
    if [ -z "$3" ]; then
        KEY="none"
    else
        KEY=$3
    fi
    if [ -z $4 ]; then
        CHAN="6"
    else
        CHAN=$4
    fi

    #echo "Connect to $AP using $ENCRYPT=>'$KEY', chan $CHAN"
    WIRELESS_SSID=$AP
    WIRELESS_ENCRYPT=$ENCRYPT
    WIRELESS_KEY=$KEY
    WIRELESS_CHAN=$CHAN
    WIRELESS_TYPE="ap"
    WIRELESS_NETWORK="apwan"

    wifi_write_config
    wifi_reload

    return 0
}

#wifi_sta_connect
#wifi_sta_connect "SomeNet" "psk" "SomeKey" "6"
#wifi_ap_create "My_test_net" "psk" "THis is key" "11"
#lan_reload
