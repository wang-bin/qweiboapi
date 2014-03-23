#!/bin/bash

if [ "$0" = "$BASH_SOURCE" -o -z "$BASH_SOURCE" ]; #??
then
    SCRIPT_FILE=$0
    SCRIPT_DIR=${0%/*}
else
    SCRIPT_DIR=.
fi
. ${SCRIPT_DIR}/template.sh
. ${SCRIPT_DIR}/functions.sh


#TODO:
#some api get empty data(status/home etc.)
#parse Chinese api document(how to get default value?)
#Makefile support(multiple jobs)
#functional style
#./web2api.sh && make
# how to add the descriptions at the beginning of api?

DOWNLOAD=curl
API_HOST="https://api.weibo.com" #json request url
API_URL_BASE="http://open.weibo.com"
API_LIST_URL="$API_URL_BASE/wiki/%E5%BE%AE%E5%8D%9AAPI"
read_dom () {
    local IFS=\>
    read -d \< ENTITY CONTENT
    local RET=$?
    TAG_NAME=${ENTITY%% *}
    ATTRIBUTES=${ENTITY#* }
    return $RET
}


parse_api_dom() {
    #tag frequency: td > tr > table, so parse in this order
    if [ "$TAG_NAME" = "td" -a "$PARSE_TR" = "true" ]; then
        CONTENT=`echo "$CONTENT"` #remove eol \n \r
        if [ $KEY ]; then
            COL=$((COL+1))
            if [ "$CONTENT" = "string" ]; then
                STRING_VALUE=true
            elif [ $COL -eq 3 ]; then #comment. parse default value
                COMMENT="$CONTENT"
                #\u9ED8\u8BA4\u4E3A\u3002
                #VALUE=`echo "$COMMENT" |sed 's/.*默认为\(.*\)。.*/\1/'` #TODO: English page
                #VALUE=`echo "$COMMENT" |sed 's/.*\x9E\xD8\x8B\xA4\x4E\x3A\(.*\)\x30\x02.*/\1/'`
                VALUE=${COMMENT##*Default is }
                [ "$VALUE" = "$COMMENT" ] && VALUE=${COMMENT##*default value is}
                [ "$VALUE" = "$COMMENT" ] && {
                    $STRING_VALUE && VALUE="\"\"" || VALUE=0
                } || VALUE=${VALUE%%.*}
            fi
        else
            KEY="$CONTENT"
            COL=0
        fi
    elif [ "$TAG_NAME" = "/tr" ]; then
    #finish 1 parameter
        $PARSE_TR  && {
            echo "$BEGIN_PARAMETER${KEY}$PARAMETER_VALUE_SEP $VALUE$END_PARAMETER  $BEGIN_COMMENT$COMMENT"
            KEY=
            VALUE=
            STRING_VALUE=false
            COL=0 #not necessary
        } || PARSE_TR=true
    elif [ "$TAG_NAME" = "table" -a "$API_TABLE" = "true" ]; then
        eval local $ATTRIBUTES
        if [ "$class" = "parameters" ]; then
            PARSE_TABLE=true
            PARSE_TR=false #first <tr> is header
        fi
    elif [ "$TAG_NAME" = "/table" ]; then
        PARSE_TABLE=false
        PARSE_TR=false
        API_TABLE=false #another is result table
    fi
}

api_url_get_path() {
    [ $# -lt 1 ] && cecho green "$0 weibo_api_url" && return 1
    local api=${1#*$API_URL_BASE}
    api=${api##*wiki/}
    api=${api%/en} #English version api page
    #api=${api//\//_}
    echo "${api}"
}

api_url_2_name() {
    [ $# -lt 1 ] && cecho green "$0 weibo_api_url" && return 1
    local api=${1#*$API_URL_BASE}
    api=${api##*/[0-9]/}
    api=${api##*wiki/}
    api=${api%/en} #English version api page
    #api=${api//\//_}
    echo "${api//\//_}"
}

parse_api_page() {
    [ $# -lt 1 ] && cecho green "$0 weibo_api_url [description] [dom_parser=parse_api_dom]" && return 1
    local API_URL=$1
    local dom_parser=parse_api_dom
    local api="`api_url_2_name $API_URL`"
    local api_path="`api_url_get_path $API_URL`"
    [ "$api" = "" -o "$api" = "en" ] && return 0 #why it happens?
    [ $# -gt 1 ] && echo "$2"
    begin_api $api $api_path
    API_TABLE=true
    [ $# -gt 2 ] && dom_parser=$3
    $DOWNLOAD $API_URL | while read_dom; do
        $API_TABLE && $dom_parser || break #not break, but parse_api_error_dom
    done
    end_api
}

API_MK=api.mk
API_RULE_MK=api_rule.mk
save_api_in_Makefile() {
    [ $# -eq 0 ] && cecho green "$0 weibo_api_url" && return 1
    local url="$1"
	local comment=
	[ $# -gt 1 ] && comment="$2" && echo "$2"
    local api="`api_url_2_name $url`"
    [ "$api" = "" -o "$api" = "en" ] && return 0 #why it happens?
    [ ! -f $API_MK ] && echo -n "API_ALL =" >$API_MK
    echo -n " $api" >>$API_MK
    cat >>$API_RULE_MK<<EOF
${api}: \$(OBJDIR)/${api}.h
\$(OBJDIR)/${api}.h: \$(OBJDIR)
	@echo "$comment" > \$@
	$SCRIPT_FILE "$url" >> \$@

EOF
}

parse_api_list_page_dom() {
    local api_url_handler=parse_api_page #the handler will be called when an api name is parsed. default action is parse the api page
    [ $# -gt 0 ] && api_url_handler=$1
    if [ "$TAG_NAME" = "td" ]; then #api name, description or category
        CONTENT=`echo "$CONTENT"` #remove eol \n \r
        [ -n "$API_URL_PATH" ] && API_DESC="$CONTENT"
    elif [ "$TAG_NAME" = "a" ]; then
        # <a href="api_path" title="...">api</a>, <a href="..." title="高级接口申请"....>
        # api: replace spaces in title with tabs. full url= $API_URL_BASE/wiki/$api. api may starts with '2'
        $PARSE_API_TR && ! $PARSE_A && ! $IN_TH && { #if <a> is already parsed, then other <a> contains no api
            PARSE_A=true
            eval local $ATTRIBUTES
            API_URL_PATH=${title// /_}
            #echo "/* $API_URL_PATH */"
        }
    elif [ "$TAG_NAME" = "tr" ]; then
        PARSE_API_TR=true
    elif [ "$TAG_NAME" = "/tr" ]; then #finish 1 api
        $PARSE_A && {
            #echo "$BEGIN_COMMENT $API_DESC $END_COMMENT"
            $api_url_handler "$API_URL_BASE/wiki/$API_URL_PATH" "$BEGIN_COMMENT $API_URL_PATH: $API_DESC $END_COMMENT"
        }
        PARSE_A=false
        PARSE_API_TR=false
        API_URL_PATH=
    elif [ "$TAG_NAME" = "th" ]; then
        IN_TH=true
    elif [ "$TAG_NAME" = "/th" ]; then
        IN_TH=false
    fi
}

#<table>'s ist <tr> is the category information, 2nd <tr> has 2 or 3 <td>. the <td> with api name attribute title="2/some/api", the next <td> is discription.
parse_api_list_page() {
    local API_LIST_URL=
    local api_url_handler=
    if [ $# -gt 2 ]; then
        api_url_handler=$3
        dom_parser=$2
        API_LIST_URL=$1
    elif [ $# -gt 1 ]; then
        dom_parser=$2
        API_LIST_URL=$1
    elif [ $# -gt 0 ]; then
        dom_parser=parse_api_list_page_dom
        API_LIST_URL=$1
    else
        cecho green "$0 weibo_api_list_url [dom_parser=parse_api_list_page_dom] [api_url_handler (e.g. save_api_in_Makefile)]" && return 1
    fi
    $DOWNLOAD $API_LIST_URL |while read_dom; do
        $dom_parser $api_url_handler
    done
}


if [ $# -gt 0 ]; then
    if [ "$1" = "-make" ]; then
        rm -rf $API_MK $API_RULE_MK
        parse_api_list_page $API_LIST_URL parse_api_list_page_dom save_api_in_Makefile
        cat $API_MK  Makefile.in  $API_RULE_MK >Makefile
        #time make -j4
        echo "Makefile created. run 'make' to create api file"
    else
        parse_api_page $1
    fi
else
    parse_api_list_page $API_LIST_URL
fi

