# !/ bin / bash

array_tha_holds_parties=() #for the parties
arg_fil=$1  #the input file 


if [ ! -e $arg_fil ]    #if it doesnt exist
    then echo "The file Doesn't Exist"
         exit 1
fi


for ((i=0; i<14; i++)); do    #we initialize the array that holds the votes
    array_tha_holds_parties[$i]=0
done


the_part=() # this will hold the name of parties 

while read  line; do  # we put the name of the parties
  the_part+=("$line")
done < "politicalParties.txt"


#touch after_sort

#sort -k1,2 -u  inputFile > after_sort

cp inputFile first_f    # we create another file like the input we got in order to do changes

sort -k1 first_f > second_f #we sort it in order to use uniq after to exclude the same voters

uniq second_f > third_f


while read -a words; do  # we add votes
for ((i=0; i<14; i++)); do
  if [[ "${the_part[$i]}" == "${words[2]}" ]]; then
      ((array_tha_holds_parties[$i]++))
      break
  fi
done

done < "third_f"



for ((i=0; i<14; i++)); do  # we give the result to the file
  if [ "${array_tha_holds_parties[$i]}" -ne 0 ]; then
    echo "${the_part[$i]} ${array_tha_holds_parties[$i]}" >> "$arg_fil"
  fi
done


rm first_f   # we remove every unnecessary file
rm second_f
rm third_f
