#!/bin/bash


for arg in $@
do
    gcc $arg -o ${arg%.*}
done