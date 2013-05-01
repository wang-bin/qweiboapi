OUT_FILE=weiboapi.h  #cat >> &1 is wrong, why?

begin_api() {
	[ $# -eq 0 ] && return 1
	API=$1
	:<<EOF
class Q_EXPORT ${API} : public Request
{
public:
    ${API}();
protected:
    void initParameters() {
        (*this)
EOF
echo "REQUEST_API_BEGIN($API)"
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

