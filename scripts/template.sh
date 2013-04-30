OUT_FILE=weiboapi.h  #cat >> &1 is wrong, why?

begin_api() {
	[ $# -eq 0 ] && return 1
	API=$1
	cat >> $OUT_FILE <<EOF
class Q_EXPORT ${API} : public Request
{
public:
    ${API}();
protected:
    void initParameters() {
        (*this)
EOF
}

end_api() {
	cat >> $OUT_FILE <<EOF
        ;
    }
};

EOF
}

BEGIN_PARAMETER="        ("
END_PARAMETER=\)
BEGIN_COMMENT="//"

