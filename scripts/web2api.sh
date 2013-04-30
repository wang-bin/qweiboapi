read_dom () {
    local IFS=\>
    read -d \< ENTITY CONTENT
    local RET=$?
    TAG_NAME=${ENTITY%% *}
    ATTRIBUTES=${ENTITY#* }
    return $RET
}

KEY=
API_TABLE=true
parse_api_dom() {
	if [ "$TAG_NAME" = "table" -a "$API_TABLE" = "true" ]; then
		eval local $ATTRIBUTES
		if [ "$class" = "parameters" ]; then
			PARSE_TABLE=true
			PARSE_TR=false #first <tr> is header
		fi
	elif [ "$TAG_NAME" = "/table" ]; then
		PARSE_TABLE=false
		PARSE_TR=false
		API_TABLE=false #another is result table
	elif [ "$TAG_NAME" = "/tr" ]; then
	#finish 1 parameter
		$PARSE_TR  && { 
			echo "$KEY ==> $VALUE #$COMMENT"
			echo "$KEY ==> $VALUE #$COMMENT" >> log.txt
			KEY=
			VALUE=
			COL=0 #not necessary
		} || PARSE_TR=true 
	elif [ "$TAG_NAME" = "td" -a "$PARSE_TR" = "true" ]; then
		CONTENT=`echo "$CONTENT"` #remove eol \n \r
		if [ $KEY ]; then
			COL=$((COL+1))
			if [ "$CONTENT" = "string" ]; then
				#echo "default value is \"\""
				VALUE="\"\""
			elif [ $COL -eq 3 -a ! "$VALUE" = "\"\"" ]; then #comment. parse default value
				COMMENT="$CONTENT"
				#\u9ED8\u8BA4\u4E3A\u3002
				#VALUE=`echo "$COMMENT" |sed 's/.*Ä¬ÈÏÎª\(.*\)¡£.*/\1/'` #TODO: English page
				#VALUE=`echo "$COMMENT" |sed 's/.*\x9E\xD8\x8B\xA4\x4E\x3A\(.*\)\x30\x02.*/\1/'` #TODO: English page
				VALUE=${COMMENT##*Default is } #`echo "$COMMENT" |sed 's/.*Default is\(.*\)\..*/\1/'` # English page
				[ "$VALUE" = "$COMMENT" ] && VALUE=${COMMENT##*default value is} #`echo "$COMMENT" |sed 's/.*default value is\(.*\)\..*/\1/'`
				[ "$VALUE" = "$COMMENT" ] && VALUE=0  || VALUE=${VALUE%%.*}
			fi
		else
			KEY="$CONTENT"
			COL=0
		fi
	fi
}

API_TABLE=true
while read_dom; do
	$API_TABLE && parse_api_dom || break #not break, but parse_api_error_dom
done