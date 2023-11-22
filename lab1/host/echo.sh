#!/bin/bash
for gpc in /dev/gpc*
do
  echo "Echo read from $gpc"
  cat $gpc | xxd
done  
