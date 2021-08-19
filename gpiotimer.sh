logger "gpiotimer.sh start"
#params=$(tr ' ' ' |' < /etc/rikgpio/rikgpio.conf) 
#arrold=(${params// / })
#modeold=${arrold[0]}
#gpioserverold=${arrold[1]}
#if [ $gpioserverold == 'Firsttime' ]; then
#  logger "gpio.sh starting firsttime"  
#  busctl set-property xyz.openbmc_project.Settings /xyz/openbmc_project/time/sync_method xyz.openbmc_project.Time.Synchronization TimeSyncMethod s "xyz.openbmc_project.Time.Synchronization.Method.Manual"
#  logger "gpio.sh busctl set-property Manual"  
#fi
method=$(busctl get-property xyz.openbmc_project.Settings /xyz/openbmc_project/time/sync_method xyz.openbmc_project.Time.Synchronization TimeSyncMethod)
#logger "gpiotimer.sh method=$method"
arr1=(${method// / })
mode1=${arr1[1]}
mode2=$(echo $mode1 | tr -d \") 
arr2=(${mode2//./ })
mode=${arr2[5]}
logger "gpiotimer.sh mode=$mode"

#logger "gpio.sh modeold=$modeold gpioserverold=$gpioserverold"

#if [ $mode == 'Manual' ]; then
#  if [ $mode != $modeold ]; then
#    logger "gpio.sh systemctl enable pch-time-sync.service"
#    systemctl enable pch-time-sync.service
#    sleep 10
#    logger "gpio.sh systemctl start pch-time-sync.service"
#    systemctl start pch-time-sync.service
#    sleep 5
#  fi
#  logger "gpio.sh modenew=$mode gpioservernew=Manual"
  #sleep 5
  #newdate=$(nc time.nist.gov 13 | grep -o '[0-9]\{2\}\-[0-9]\{2\}\-[0-9]\{2\} [0-9]\{2\}\:[0-9]\{2\}\:[0-9]\{2\}' | sed -e 's/^/20/')
  #date -s "$newdate"
  #logger "gpio.sh set Manual = $newdate"
#  logger "gpio.sh firsttine write $mode to rikgpio.conf"
#  echo $mode Manual > /etc/rikgpio/rikgpio.conf
#  sleep 1
#el
if [ $mode == 'NTP' ]; then
# if [ $mode != $modeold ]; then
#   logger "gpio.sh   systemctl stop pch-time-sync.service"
#   systemctl stop pch-time-sync.service
#   sleep 5
#   logger "gpio.sh   systemctl disable pch-time-sync.service"
#   systemctl disable pch-time-sync.service
#   sleep 10
# fi
 servers=$(busctl get-property xyz.openbmc_project.Network /xyz/openbmc_project/network/eth0 xyz.openbmc_project.Network.EthernetInterface NTPServers)
 if [ -z "$servers" ]; then
   numserv=0
 else
   arr3=(${servers// / })
   numserv=${arr3[1]}
 fi
 logger "gpiotimer.sh numserv=$numserv"
 #if [ $numserv -eq 0 ]; then
 #  mode='Manual'
 #fi

 for i in "${!arr3[@]}"; do 
  sleep 1
  if [ $i -gt 1 ];  then
    gpioserver=$(echo ${arr3[$i]} | tr -d \") 
    gpiodate -u $gpioserver
    if [ $? -eq 0 ]; 
    then
      sleep 5
      logger "gpiotimer.sh modenew=$mode gpioservernew=$gpioserver"
      newdate=$(date) 
      logger "gpiotimer.sh set NTP from $gpioserver = $newdate"
      #logger "gpiotimer.sh NTP write $mode $gpioserver to rikgpio.conf"
      #echo $mode $gpioserver > /etc/rikgpio/rikgpio.conf
      sleep 1
      break
    fi
    logger "gpiotimer.sh set NTP from $gpioserver failed, trying next"
    sleep 5
  fi
 done

fi

logger "gpiotimer.sh complete"
