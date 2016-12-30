make clean
make
sleep 1
for (( i=1 ; i<="$1"; i+=1 ))
do
echo "task:"$i
time ./TCGA2015.out
sleep 0.5
done
