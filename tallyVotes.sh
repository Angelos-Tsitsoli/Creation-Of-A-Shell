# !/ bin / bash

parties_votes=()
candidates_names=()

for ((i=0; i<14; i++)); do
    parties_votes[$i]=0
done


#for ((i=0; i<14; i++)); do
#    echo "${parties[$i]}"
#done



parties=()

while read  line; do
  parties+=("$line")
done < "politicalParties.txt"

while read -a words; do
  length_of_array=${#words[@]}
  
  if [ $length_of_array -eq 3 ]; then
    #echo "geia"
    candidates_names+=("${words[1]} ${words[2]}")
    for element in "${my_array[@]}"; do
        echo "$element"
    done

  fi

  if [ $length_of_array -eq 4 ]; then
    #echo "ela"
    candidates_names+=("${words[2]} ${words[3]}")
  fi

  # echo "$length"
  # candidates_names+=("$line")

done < "inputFile.txt"

#for ((i=0; i<14; i++)); do
#    echo "${candidates_names[$i]}"
#done


for ((i=0; i<14; i++)); do
    count=$(grep -o -w "${parties[$i]}" "inputFile.txt" | wc -l)
    parties_votes[$i]=$count
done

#for ((i=0; i<14; i++)); do
#    echo "${parties[$i]}" "${parties_votes[$i]}"
#done


#for ((j=0; j<14; j++)); do
#    parties_votes[$i]=0
#done



