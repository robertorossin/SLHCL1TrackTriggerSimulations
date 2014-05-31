#!/bin/bash

# This script comes from CRAB crabWatchdog.sh

pid=`pgrep amsim`

maxRss=0
maxVsz=0
maxDisk=0
maxCpu=0
maxTime=0
maxWall=0

iter=0
startTime=`date +%s`

while true
do
  metrics=`ps --no-headers -o pid,cputime,rss,vsize,args  -ww -p ${pid}`
  if [ $? -ne 0 ] ; then break ; fi # make sure process is still alive

  now=`date +'%b %d %T %Z'`

  cpu=`echo $metrics|awk '{print $2}'`  # in the form [dd-]hh:mm:ss
  #convert to seconds
  [[ $cpu =~ "-" ]] && cpuDays=`echo $cpu|cut -d- -f1`*86400
  [[ ! $cpu =~ "-" ]] && cpuDays=0
  cpuHMS=`echo $cpu|cut -d- -f2` # works even if there's no -
  cpuSeconds=10\#`echo ${cpuHMS}|cut -d: -f3`
  cpuMinutes=10\#`echo ${cpuHMS}|cut -d: -f2`*60
  cpuHours=10\#`echo ${cpuHMS}|cut -d: -f1`*3600
  let cpuTime=$cpuSeconds+$cpuMinutes+$cpuHours+$cpuDays

  rss=`echo $metrics|awk '{print $3}'`
  vsize=`echo $metrics|awk '{print $4}'`
  cmd=`echo $metrics|awk '{print $5" "$6" "$7" "$8" "$9" "$10" "$11" "$12" "$13" "$14" "$15}'`

  let wallTime=`date +%s`-${startTime}

  if [ $rss -gt $maxRss ]; then maxRss=$rss; fi
  if [ $vsize -gt $maxVsz ]; then maxVsz=$vsize; fi
  if [ $cpuTime -gt $maxCpu ]; then maxCpu=$cpuTime; fi
  if [ $wallTime -gt $maxWall ]; then maxWall=$wallTime; fi

  echo -e " $now\t$pid\t$maxRss\t$maxVsz\t$maxDisk\t$cpuTime\t$wallTime\t$cmd"

  let iter=${iter}+1
  sleep 60
done
  
