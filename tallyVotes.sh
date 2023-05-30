# !/ bin / bash

parties_votes=()
candidates_names=()
input_file=$1


for ((i=0; i<14; i++)); do
    parties_votes[$i]=0
done




flag_if_candidate_already_vote=0
flag_if_candidate_already_vote2=0

parties=()

while read  line; do
  parties+=("$line")
done < "politicalParties.txt"


touch after_sort.txt

sort -k2 -t' ' inputFile.txt > after_sort.txt

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
done < "after_sort.txt"


#for ((i=0; i<14; i++)); do
#    count=$(grep -o -w "${parties[$i]}" "inputFile.txt" | wc -l)
#    parties_votes[$i]=$count
#done


for ((i=0; i<14; i++)); do
    echo "${parties[$i]} ${parties_votes[$i]}" >> $input_file
done

