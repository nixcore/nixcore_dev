#!/bin/sh

# CGI related functions

getvars ()
{
    if [ "$REQUEST_METHOD" = POST ]; then
        # http://stackoverflow.com/questions/3919755/how-to-parse-query-string-from-a-bash-cgi-script
        read -n $CONTENT_LENGTH QUERY
    elif [ "$REQUEST_METHOD" = GET ]; then
        QUERY=$QUERY_STRING
    fi
    saveIFS=$IFS
    IFS='&'
    eval $QUERY
    IFS=$saveIFS
    # The query is now in $QUERY
    # Variables are now in $VARNAME and ${VARNAME}
}

# EOF
