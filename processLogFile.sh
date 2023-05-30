# !/ bin / bash

parties_votes=()
input_file=$1


for ((i=0; i<14; i++)); do
    parties_votes[$i]=0
done


parties=()

while read line; do
  parties+=("$line")
done < "politicalParties.txt"


touch after_sort2

sort -k2 -t' ' $input_file > after_sort2

possibly_same=""

while read -a words; do

if [[ "${words[1]}" != "$possibly_same" ]]; then
  for ((i=0; i<14; i++)); do
    if [[ "${parties[$i]}" == "${words[0]}" ]]; then
      ((parties_votes[$i]++))
    fi
  done
fi

possibly_same="${words[1]}"
done < "after_sort2"



for ((i=0; i<14; i++)); do
    echo "${parties[$i]} ${parties_votes[$i]}" >> pollerResultsFile
done

rm after_sort2