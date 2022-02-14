delete_ary_elmt() {
	local word=$1      # the element to search for & delete
	local aryref="$2[@]" # a necessary step since '${!$2[@]}' is a syntax error
	local arycopy=("${!aryref}") # create a copy of the input array
	local status=1
	for (( i = ${#arycopy[@]} - 1; i >= 0; i-- )); do # iterate over indices backwards
		elmt=${arycopy[$i]}
		[[ $elmt == $word ]] && unset "$2[$i]" && status=0 # unset matching elmts in orig. ary
	done
	return $status # return 0 if something was deleted; 1 if not
}

if (( ${#@} < 3 )); then
	echo "Usage: $0 question validAnswer answers"
	exit 0
fi

question=$1
shift
position=$(( $RANDOM % (${#@}) ))
validAnswer=$1
shift
fakes=("$@")
fakesCopy=()

printf "$question\a" #>/dev/null
printf "0: %.8x" $position | sed -E 's/0: (..)(..)(..)(..)/0: \4\3\2\1/' | xxd -r -g0 #>/dev/null
for i in $(seq 0 ${#@}); do
	if (( $i == $position )); then
		printf "\a$validAnswer" #>/dev/null
	else
		next=$(( $RANDOM % ${#fakes[@]} ))
		printf "\a${fakes[$next]}" #>/dev/null
		delete_ary_elmt "${fakes[$next]}" fakes
		fakesCopy=()
		for i in "${!fakes[@]}"; do
			fakesCopy+=("${fakes[i]}")
		done
		fakes=("${fakesCopy[@]}")
		# for e in "${fakes[@]}"; do
		# 	echo "$e"
		# done
	fi
done
echo