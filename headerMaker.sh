if (( ${#@} != 3 )); then
	echo "Usage: $0 font language description"
	exit 0
fi

echo -e "qPUG\a$1\a$2"
echo -e "$3"