# !/ bin / bash

arg_fil=$1
the_num_of_lines_argument=$2



if [ $# -ne 2 ]  #If there are not 2 arguments
    then echo "Not enough command line arguments."
         exit 
fi

if [ ! -e $arg_fil ] #If it doesnt exist
    then echo "The file Doesn't Exist"
         exit 1
fi


if [ $2 -le 0 ] #If the number we gave is negative
    then echo "Error with the numLines"
         exit
fi



polit_par=() #storage for the parties

while read -r line; do  #storing the parties
    polit_par+=("$line")
done < "$arg_fil"


#putting the letters
array_of_the_let=()
iter=0
array_of_the_let[$iter]='a'
iter=$((iter+1))
array_of_the_let[$iter]='b'
iter=$((iter+1))
array_of_the_let[$iter]='c'
iter=$((iter+1))
array_of_the_let[$iter]='d'
iter=$((iter+1))
array_of_the_let[$iter]='e'
iter=$((iter+1))
array_of_the_let[$iter]='f'
iter=$((iter+1))
array_of_the_let[$iter]='g'
iter=$((iter+1))
array_of_the_let[$iter]='h'
iter=$((iter+1))
array_of_the_let[$iter]='i'
iter=$((iter+1))
array_of_the_let[$iter]='j'
iter=$((iter+1))
array_of_the_let[$iter]='k'
iter=$((iter+1))
array_of_the_let[$iter]='l'
iter=$((iter+1))
array_of_the_let[$iter]='m'
iter=$((iter+1))
array_of_the_let[$iter]='n'
iter=$((iter+1))
array_of_the_let[$iter]='o'
iter=$((iter+1))
array_of_the_let[$iter]='p'
iter=$((iter+1))
array_of_the_let[$iter]='q'
iter=$((iter+1))
array_of_the_let[$iter]='r'
iter=$((iter+1))
array_of_the_let[$iter]='s'
iter=$((iter+1))
array_of_the_let[$iter]='t'
iter=$((iter+1))
array_of_the_let[$iter]='u'
iter=$((iter+1))
array_of_the_let[$iter]='v'
iter=$((iter+1))
array_of_the_let[$iter]='w'
iter=$((iter+1))
array_of_the_let[$iter]='x'
iter=$((iter+1))
array_of_the_let[$iter]='y'
iter=$((iter+1))
array_of_the_let[$iter]='z'



touch "inputFile" #create the file

polit_pa=14 #number of parties
for (( iterator=1; iterator<=the_num_of_lines_argument; iterator++ )); do  
    number=$RANDOM      #to pick a random party
    number=$((number % polit_pa))
    number_of_array_of_the_let=$((RANDOM % 10 + 3))  # to pick randomly the number of letters
    for ((iterator2=1; iterator2<=$number_of_array_of_the_let; iterator2++)); do
        ran_num=$RANDOM         #to create a random name
        ran_num=$((ran_num % 26))
        number2=${array_of_the_let[ran_num]}
        random_string+=$number2
    done
    for ((iterator3=1; iterator3<=$number_of_array_of_the_let; iterator3++)); do
        ran_num2=$RANDOM     #to create a random surname
        ran_num2=$((ran_num2 % 26))
        number3=${array_of_the_let[ran_num2]}
        random_string2+=$number3
    done

    echo "$random_string $random_string2 ${polit_par[number]}" >> inputFile  #give to the inputFile
    random_string="" #reset
    random_string2="" #reset
done



