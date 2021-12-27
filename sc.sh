#!/bin/bash
tmp_p="tmp_p.bin"
tmp_q="tmp_q.bin"
tim_p="tim_p.bin"
tim_q="tim_q.bin"
size="1000000"
echo > $tim_p
echo > $tim_q
for nproc in 1 2 4 8 16
do
for iter in 1 2 3 4 5 6 7 8 9 10
do
./prob3 $size $nproc >> $tmp_p
done
echo $nproc $iter
./tim $tmp_p $tim_p $nproc
echo > $tmp_p
./qs $size >> $tmp_q
done
./tim $tmp_q $tim_q 1
