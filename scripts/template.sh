begin_api() {
	[ $# -eq 0 ] && return 1
	API=$1
	cat <<EOF
class Q_EXPORT ${API}Request : public Request
{
public:
    ${API}Request();
protected:
    void initParameters() {
        (*this)
EOF
}

end_api() {
	cat <<EOF
        ;
    }
};

EOF
}

BEGIN_PARAMETER="        ("
END_PARAMETER=\)
BEGIN_COMMENT="//"

