# !/ bin / bash
#echo Number of input parameters = $1
#file=$1
#counter=0 
#
##while read line; do
#  let counter=counter+1
#done < $file
#  
#touch "filename.sh"

#$ chmod +x create_input.sh
#$ ./create_input.sh politicalParties.txt


if [ "$#" != 2 ]; then
  echo "Not enough command line arguments."
  exit 
fi

file="$1"
political_parties=()

while read -r line; do
  political_parties+=("$line")
done < "$file"


touch "inputFile.txt"

LIMIT=8
for (( iterator=1; iterator<=LIMIT; iterator++ )); do
 echo "political_parties($iterator) ${political_parties[iterator-1]}" >> "inputFile.txt"
done




