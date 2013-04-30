if [ "$0" = "$BASH_SOURCE" -o -z "$BASH_SOURCE" ]; #??
then
	SCRIPT_DIR=${0%/*}
else
	SCRIPT_DIR=.
fi
echo $SCRIPT_DIR
. ${SCRIPT_DIR}/template.sh
. ${SCRIPT_DIR}/functions.sh


API_HOST="https://api.weibo.com" #json request url
API_URL_BASE="http://open.weibo.com/wiki"
API_LIST_URL="$API_URL_BASE/%E5%BE%AE%E5%8D%9AAPI"
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
				#echo "default value is \"\""
				VALUE="\"\""
			elif [ $COL -eq 3 ]; then #comment. parse default value
				COMMENT="$CONTENT"
				if [ ! "$VALUE" = "\"\"" ]; then
					#\u9ED8\u8BA4\u4E3A\u3002
					#VALUE=`echo "$COMMENT" |sed 's/.*Ä¬ÈÏÎª\(.*\)¡£.*/\1/'` #TODO: English page
					#VALUE=`echo "$COMMENT" |sed 's/.*\x9E\xD8\x8B\xA4\x4E\x3A\(.*\)\x30\x02.*/\1/'` #TODO: English page
					VALUE=${COMMENT##*Default is } #`echo "$COMMENT" |sed 's/.*Default is\(.*\)\..*/\1/'` # English page
					[ "$VALUE" = "$COMMENT" ] && VALUE=${COMMENT##*default value is} #`echo "$COMMENT" |sed 's/.*default value is\(.*\)\..*/\1/'`
					[ "$VALUE" = "$COMMENT" ] && VALUE=0  || VALUE=${VALUE%%.*}
				fi
			fi
		else
			KEY="$CONTENT"
			COL=0
		fi
	elif [ "$TAG_NAME" = "/tr" ]; then
	#finish 1 parameter
		$PARSE_TR  && { 
			echo "$BEGIN_PARAMETER$KEY, $VALUE$END_PARAMETER  $BEGIN_COMMENT$COMMENT"
			echo "$BEGIN_PARAMETER$KEY, $VALUE$END_PARAMETER  $BEGIN_COMMENT$COMMENT" >> $OUT_FILE
			KEY=
			VALUE=
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

#curl $API_LIST_URL | parse_api_list_page

parse_api_page() {
	[ $# -lt 1 ] && cecho green "$0 weibo_api_url" && return 1
	local API_URL=$1
	local api=${API_URL#*$API_URL_BASE}
	api=${api##*[0-9]/}
	api=${api%/en}
	api=${api//\//_}
	begin_api $api
	API_TABLE=true
	echo "parsing api url: $API_URL"
	curl $API_URL | while read_dom; do
		$API_TABLE && parse_api_dom || break #not break, but parse_api_error_dom
	done
	end_api
}


parse_api_list_page_dom() {
	if [ "$TAG_NAME" = "td" ]; then #api name or description(has attribute 'title') or category
		PARSE_A=true
		CONTENT=`echo "$CONTENT"` #remove eol \n \r
		[ -n "$API_URL_PATH" ] && API_DESC="$CONTENT"
	elif [ "$TAG_NAME" = "a" ]; then
		$PARSE_API_TR && {
			eval local $ATTRIBUTES
			if [ -z "$title" ]; then
				PARSE_A=false
			else
				PARSE_A=true
				API_URL_PATH="$title"
				echo "$API_URL_PATH"		
			fi
		}
	elif [ "$TAG_NAME" = "tr" ]; then 
		PARSE_API_TR=true	
	elif [ "$TAG_NAME" = "/tr" ]; then #finish 1 api
		$PARSE_A && {
			echo "$BEGIN_COMMENT $API_DESC"
			parse_api_page $API_URL_BASE/$API_URL_PATH/en
		}
		PARSE_A=false
		PARSE_API_TR=false
		API_URL_PATH=
	fi
}

#<table>'s ist <tr> is the category information, 2nd <tr> has 2 or 3 <td>. the <td> with api name attribute title="2/some/api", the next <td> is discription.
parse_api_list_page() {
	local api_list_url=$API_LIST_URL
	[ $# -gt 0 ] && api_list_url=$1
	curl $api_list_url |while read_dom; do
		parse_api_list_page_dom
	done
}

echo >$OUT_FILE

if [ $# -gt 0 ]; then
	parse_api_page $1
else
	parse_api_list_page
fi


