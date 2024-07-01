#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <filename>"
  exit 1
fi

filename=$1

as -o "${filename}.o" "${filename}.s"
ld -o "${filename}" "${filename}.o"
qemu-arm "./${filename}"

~                         
