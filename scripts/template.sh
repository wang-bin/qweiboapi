OUT_FILE=weiboapi.h  #cat >> &1 is wrong, why?

begin_api() {
	[ $# -eq 0 ] && return 1
	API=$1
	URL=$2
	:<<EOF
class QWEIBOAPI_EXPORT ${API} : public Request
{
public:
    ${API}() {prepare();};
protected:
    void initParameters() {
        (*this)
EOF
echo "REQUEST_API_BEGIN($API, \"$URL\")"
}

end_api() {
	:<<EOF
        ;
    }
};

EOF
echo "REQUEST_API_END()"
}

BEGIN_PARAMETER="        (\""
END_PARAMETER=\)
PARAMETER_VALUE_SEP="\","
BEGIN_COMMENT="//"
END_COMMEND=

