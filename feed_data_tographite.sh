: '
Following scripts gets sensor readings from 
Node MCU and feeds to graphite.
'

Node_mcu_address='10.11.3.93'
graphite_address='localhost'
graphite_port=2003
iterator=0
sensor_data=()
while true; 
do
	# Read Data from Sensor
	sensor_readings=`curl -s $Node_mcu_address`
	iterator=0
	#Parse data
	for each_sensor_reading in $sensor_readings 
	do
		sensor_data[$iterator]=$each_sensor_reading
		((iterator++))
	done
	iterator=0
	# Send it to graphite
	#echo ${sensor_data[*]}
	for iterator in `seq 0 3`
	do
		echo "Sensor-$iterator ${sensor_data[$iterator]}  `date +%s`" | nc -q0 $graphite_address $graphite_port
	done
	sleep 1
done

