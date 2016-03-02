--Author: Shivendra Mishra
--Environment: Node MCU v0.9
--Created: 1-Mar-2016
--Description: Following script reads temperature sensor data through a analog switch and updates them 
--             on HTTP server (of Node MCU).

--Prepare to get connected
wifi.setmode(wifi.STATION)
wifi.sta.config("our_wifi","wifi_pwd")
print(wifi.sta.getip())

--Pins to talk
select_1=0 
select_2=1 
gpio.mode(select_1,gpio.OUTPUT) 
gpio.mode(select_2,gpio.OUTPUT) 

--Prepare server
srv=net.createServer(net.TCP) 
srv:listen(80,function(conn) 
conn:on("receive",function(conn,payload) 
print(payload) 

--Read Sensor one
gpio.write(select_1,gpio.LOW) 
gpio.write(select_2,gpio.LOW) 
sensor_one_reading = adc.read(0) 


--Read Sensor Two
gpio.write(select_1,gpio.LOW) 
gpio.write(select_2,gpio.HIGH) 
sensor_two_reading = adc.read(0) 


--Read Sensor Three
gpio.write(select_1,gpio.HIGH) 
gpio.write(select_2,gpio.LOW) 
sensor_three_reading = adc.read(0) 


--Read Sensor Four
gpio.write(select_1,gpio.HIGH) 
gpio.write(select_2,gpio.HIGH) 
sensor_four_reading = adc.read(0) 

--Send stuff to him(python).
stuff_to_send = "<h1> I see: "..sensor_one_reading.." "..sensor_two_reading.." "..sensor_three_reading.." "..sensor_four_reading.."</h1>" 
conn:send(stuff_to_send) 
conn:close() 
end) 
end) 
 

