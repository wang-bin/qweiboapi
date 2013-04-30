if [ "$0" = "$BASH_SOURCE" -o -z "$BASH_SOURCE" ];
then
	SCRIPT_DIR=${0%/*}
else
	SCRIPT_DIR=.
fi
echo $SCRIPT_DIR
. ${SCRIPT_DIR}/template.sh
. ${SCRIPT_DIR}/functions.sh


API_LIST_URL="http://open.weibo.com/wiki/%E5%BE%AE%E5%8D%9AAPI"
API_HOST="https://api.weibo.com"
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
			echo "$BEGIN_PARAMETER$KEY, $VALUE$END_PARAMETER  $BEGIN_COMMENT$COMMENT" >> log.txt
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
	begin_api Test
	API_TABLE=true
	echo "parsing api url: $API_URL"
	curl $API_URL | while read_dom; do
		$API_TABLE && parse_api_dom || break #not break, but parse_api_error_dom
	done
	end_api
}


parse_api_list_page_dom() {
	
	if [ "$TAG_NAME" = "td" -a "$PARSE_API_TR" = "true" ]; then #api name or description(has attribute 'title') or category
		eval local $ATTRIBUTES
		if [ -z "$title" ]; then
			PARSE_API_TR=false
		else
			PARSE_API_TR=true
			API_URL_PATH="$title"
			CONTENT=`echo "$CONTENT"` #remove eol \n \r
			[ $API_URL_PATH ] && API_DESC=$CONTENT
		fi
	elif [ "$TAG_NAME" = "/tr" ]; then #finish 1 api
		[ $PARSE_API_TR ] && {
			$BEGIN_COMMENT $API_DESC
			parse_api_page $API_HOST/$API_URL_PATH
		}
	fi
}

#<table>'s ist <tr> is the category information, 2nd <tr> has 2 or 3 <td>. the <td> with api name attribute title="2/some/api", the next <td> is discription.
parse_api_list_page() {
	while read_dom; do
		parse_api_list_page_dom
	done
}
