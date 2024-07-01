#!/bin/bash

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <c_file> <assembly_file>"
  exit 1
fi

c_file="$1"
assembly_file="$2"
output_file="${c_file%.*}"  # Extracting the file name without extension

# Compile C code (-c flag)
gcc -Wall -g -c "$c_file" -o "${output_file}.o"

# Compile assembly code (-c flag)
as -o "${output_file}_asm.o" "$assembly_file"

# Link object files
gcc -o "${output_file}_program" "${output_file}.o" "${output_file}_asm.o"

# Clean up object files (optional)
# rm "${output_file}.o" "${output_file}_asm.o"

echo "Build completed. Executable: ${output_file}_program"

~          
