# !/ bin / bash

parties_votes=()
input_file=$1


for ((i=0; i<14; i++)); do
    parties_votes[$i]=0
done


parties=()

while read  line; do
  parties+=("$line")
done < "politicalParties.txt"


touch after_sort

sort -k1,2 -u  inputFile > after_sort

#possibly_same=""

while read -a words; do
for ((i=0; i<14; i++)); do
  if [[ "${parties[$i]}" == "${words[2]}" ]]; then
      ((parties_votes[$i]++))
      break
  fi
done
#fi

#possibly_same="${words[1]}"
done < "after_sort"



for ((i=0; i<14; i++)); do
  if [ "${parties_votes[$i]}" -ne 0 ]; then
    echo "${parties[$i]} ${parties_votes[$i]}" >> "$input_file"
  fi
done


rm after_sort
