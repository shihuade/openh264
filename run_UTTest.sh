#!/bin/bash
startTime=`date`

for((i=0; i<400; i++))
do
   echo -e "\n\n*****************************************\n\n"
   echo "test loop, loop index is $i"
   echo -e "\n\n*****************************************\n\n"
   TestStart=`date`
   make test
   [  ! $? -eq 0 ] && echo "test failed under index $i" && exit 1
   TestEnd=`date`
   
   echo -e "\n\n**********************************"
   echo "TestStart is $TestStart"
   echo "TestEnd   is $TestEnd"
   echo "*************************************\n\n"
done

EndTime=`date`

echo "startTime is: $startTime"
echo "EndTime   is: $EndTime"

