if (( ${#@} <= 3 )); then
	echo "Usage: $0 question validAnswer answers"
	exit 0
fi

question=$1
shift
position=$(( $RANDOM % (${#@}) ))
validAnswer=$1
shift
fakes=("$@")
fakesCopy=("$@")

printf "$question\a"
printf "0: %.8x" $position | sed -E 's/0: (..)(..)(..)(..)/0: \4\3\2\1/' | xxd -r -g0
for i in $(seq 0 ${#@}); do
	if (( $i == $position )); then
		printf "\a$validAnswer"
	else
		next=$(( $RANDOM % ${#fakesCopy[@]} ))
		printf "\a${fakesCopy[$next]}"
		fakesCopy=("${fakesCopy[@]/${fakesCopy[$next]}}")
	fi
done
echo