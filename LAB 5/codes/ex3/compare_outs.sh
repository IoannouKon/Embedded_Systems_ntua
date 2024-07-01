#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <file1.out> <file2.out>"
    exit 1
fi

file1=$1
file2=$2

# Use diff command to compare the contents of the two files
diff "$file1" "$file2" > /dev/null

if [ $? -eq 0 ]; then
    echo "Files are the same."
else
    echo "Files are different."
fi

~                                                                                                    
~                                                                                                    
~                                                                                                    
~                         
