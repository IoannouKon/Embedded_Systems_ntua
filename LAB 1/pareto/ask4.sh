#!/bin/bash

ld1_s=('2kB' '4kB' '8kB' '16kB' '32kB' '64kB')
li1_s=('2kB' '4kB' '8kB' '16kB' '32kB' '64kB')
l2_s=('128kB' '256kB' '512kB' '1024kB')
u_f=('2' '4' '8' '16' '32')


for i in "${ld1_s[@]}"
do
    for j in "${li1_s[@]}" 
    do
        for i2 in "${l2_s[@]}" 
        do
            for i3 in "${u_f[@]}" 
            do
               build/X86/gem5.opt configs/learning_gem5/part1/two_level.py tables_UF/tables_uf$i3.exe --l1i_size=$j --l1d_size=$i --l2_size=$i2
	       cat m5out/stats.txt | grep "numCycles" >>cycles.txt
            done
        done
    done
done
