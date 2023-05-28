# !/ bin / bash

parties_votes=()
candidates_names=()

for ((i=0; i<14; i++)); do
    parties_votes[$i]=0
done


#for ((i=0; i<14; i++)); do
#    echo "${parties[$i]}"
#done

flag_if_candidate_already_vote=0
flag_if_candidate_already_vote2=0

parties=()

while read  line; do
  parties+=("$line")
done < "politicalParties.txt"

while read -a words; do
  length_of_array=${#words[@]}
  
  if [ $length_of_array -eq 3 ]; then
    for element in "${candidates_name[@]}"; do
        if [ "$element" == "${words[1]} ${words[2]}" ]; then
            flag_if_candidate_already_vote=1
        fi 
    done

    if [ $flag_if_candidate_already_vote == 0 ]; then
        candidates_names+=("${words[1]} ${words[2]}")
        for ((i=0; i<14; i++)); do
            if [ "${parties[$i]}" == "${words[0]}" ]; then
                ((parties_votes[$i]++))
            fi
        done
    fi

    if [ $flag_if_candidate_already_vote == 1 ]; then
        flag_if_candidate_already_vote=0
    fi 
  fi

  if [ $length_of_array -eq 4 ]; then
    for element in "${candidates_name[@]}"; do
        if [ "$element" == "${words[2]} ${words[3]}" ]; then
            flag_if_candidate_already_vote2=1
        fi 
    done

    if [ $flag_if_candidate_already_vote2 == 0 ]; then
        candidates_names+=("${words[2]} ${words[3]}")
         for ((i=0; i<14; i++)); do
            if [ "${parties[$i]}" == "${words[0]} ${words[1]}" ]; then
                ((parties_votes[$i]++))
            fi
        done
    fi

    if [ $flag_if_candidate_already_vote2 == 1 ]; then
        flag_if_candidate_already_vote2=0
    fi 
  fi

  # echo "$length"
  # candidates_names+=("$line")

done < "inputFile.txt"

for ((i=0; i<14; i++)); do
    echo "${parties_votes[$i]}"
done


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



