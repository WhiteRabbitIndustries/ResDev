#!/bin/sh
#Credit to Fordsford and JKW for original scripts, Biggy for modifications

#Subroutine for detecting short button press

button_pressed()
{ 
#BTN= i2cget -f -y 0 0x34 0x4a #Read AXP209 Register 34H
BTN=`i2cget -f -y 0 0x34 0x4a`
    if [ "$BTN" == "0x00" ]; then
        return 1 #No Press
    elif [ "$BTN" == "0x02" ]; then
        return 0 #Short Press
    else
        echo "unknown register value"
        exit 1
    fi
}

run_scripts()
{	
	#Inform syslog of Event Start
	logger -t cust_pwr_btn -p local7.info -is "Initiating Custom Power Button Scripts."

	#Run scripts
	echo "running scripts"
	for script in /etc/cust_pwr_btn/*
	do scriptbase=${script##*/} # Strip pathname
	if [ -d $script ] ;then continue; fi
	( echo "Processing $script"
	sh $script
	) 2>&1 | logger -t $scriptbase -p local7.info -is
	done

	#Inform syslog of Event Finish
	logger -t cust_pwr_btn -p local7.info -is "Completed Custom Power Button Scripts."
	echo "exiting"
    exit 0
}

COUNTER=0
#Infinite loop for checking conditions
while true; do

	if button_pressed; then
		# reset button register
		i2cset -f -y 0 0x34 0x4a 0x02
		COUNTER=$((COUNTER+1))
		BUTTON_PRESS_TIME=$SECONDS
		echo Button Pressed on $BUTTON_PRESS_TIME
	fi


	if [[  $((SECONDS - BUTTON_PRESS_TIME)) -gt 3 ]] && [[ $COUNTER -ne 0 ]]; then
		# means no activity
		COUNTER=0
		echo Counter reset to $COUNTER
	fi

	if [[ "$COUNTER" -gt 2 ]]; then
		# run scripts.
		run_scripts
	fi

done


