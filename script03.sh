#!/bin/bash
textDir=$1                      
cd $textDir
copyDir=$textDir/c
echo `mkdir ${copyDir}`
for file in $textDir/*
    do cp $file $copyDir
done
for file in $copyDir/*
    do
    echo `chmod +x ${file}`
    newFile="2${file##*/}"
    cat $file | while read line
        do echo $line | tr -d '\r' >> $newFile
    done
    mv $newFile $file
    mv  $file $textDir 
done
rmdir $copyDir