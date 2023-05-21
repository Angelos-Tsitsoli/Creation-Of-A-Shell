# !/ bin / bash
#echo Number of input parameters = $1
file=$1

while read line; do
  echo $line
done < $file

