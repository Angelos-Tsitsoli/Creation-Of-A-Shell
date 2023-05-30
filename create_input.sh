# !/ bin / bash

input_file=$1
num_lines=$2

if [ $# -ne 2 ] 
    then echo "Not enough command line arguments."
         exit 
fi


if [ ! -e $input_file ] 
    then echo "The file Doesn't Exist"
         exit 1
fi

if [ ! -f $input_file ] 
    then echo "The file isn't regular"
         exit
fi

if [ ! -r $input_file ] 
    then echo "The file is not  readable"
         exit
fi

if [ $2 -le 0 ] 
    then echo "Error with the numLines"
         exit
fi


political_parties=()

while read -r line; do
  if [[ "$line" == "&" ]]; then
    break
  else 
    political_parties+=("$line")
  fi
done < "$input_file"


characters="abcdefghijklmnopqrstuvwxyz"

touch "inputFile"

LIMIT=$2
PARTIES=14
for (( iterator=1; iterator<=LIMIT; iterator++ )); do
    number=$RANDOM   
    let "number %= PARTIES  "
    let "number_of_characters =$((RANDOM % 10 + 3))"
    for ((iterator2=1; iterator2<=$number_of_characters; iterator2++)); do
        ran_num=$RANDOM
        let "ran_num %=26 "
        number2=${characters:ran_num:1}
        random_string+=$number2
    done
    for ((iterator3=1; iterator3<=$number_of_characters; iterator3++)); do
        ran_num2=$RANDOM
        let "ran_num2 %=26 "
        number3=${characters:ran_num2:1}
        
        random_string2+=$number3
    done

    echo "${political_parties[$((number))]} $random_string $random_string2 " >> inputFile
    random_string=""
    random_string2=""
done



