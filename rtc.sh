#!/bin/bash



readonly I2C_RTC_ADDRESS=0x68

get_rtc_time()
{
  local rtc_ts=$(get_rtc_timestamp)
  if [ "$rtc_ts" == "" ] ; then
    echo 'N/A'
  else
    echo $(date +'%a %d %b %Y %H:%M:%S %Z' -d @$rtc_ts)
  fi
}

get_rtc_timestamp()
{
	sec=$(bcd2dec $(i2c_read 0x01 $I2C_RTC_ADDRESS 0x00))
	min=$(bcd2dec $(i2c_read 0x01 $I2C_RTC_ADDRESS 0x01))
	hour=$(bcd2dec $(i2c_read 0x01 $I2C_RTC_ADDRESS 0x02))
	date=$(bcd2dec $(i2c_read 0x01 $I2C_RTC_ADDRESS 0x04))
	month=$(bcd2dec $(i2c_read 0x01 $I2C_RTC_ADDRESS 0x05))
	year=$(bcd2dec $(i2c_read 0x01 $I2C_RTC_ADDRESS 0x06))
	echo $(date --date="$year-$month-$date $hour:$min:$sec UTC" +%s)
}


bcd2dec()
{
  local result=$(($1/16*10+($1&0xF)))
  echo $result
}

i2c_read()
{
  local retry=0
  if [ $# -gt 3 ] ; then
    retry=$4
  fi
  local result=$(i2cget -y $1 $2 $3)
  if [[ $result =~ ^0x[0-9a-fA-F]{2}$ ]] ; then
    echo $result;
  else
    retry=$(( $retry + 1 ))
    if [ $retry -eq 4 ] ; then
      log "I2C read $1 $2 $3 failed (result=$result), and no more retry."
    else
      sleep 1
      log2file "I2C read $1 $2 $3 failed (result=$result), retrying $retry ..."
      i2c_read $1 $2 $3 $retry
    fi
  fi
}

rtctime="$(get_rtc_timestamp)"
echo $rtctime
