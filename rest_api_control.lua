-- Notice: All parts of this code is protected
-- with following license:
-- https://github.com/MishraShivendra/node_mcu_py_email/blob/master/LICENSE.md
-- Corporate/commercial usage is restricted.

-- Author: Shivendra Mishra
-- Environment: Node MCU v0.9
-- Created: 1-Mar-2016
-- Description: 
--  Following script reads temperature sensor 
--  data through a analog switch and controls
--  exhaust fan speed.
--  I feel this all should be written into separate
--  files but again it is not large (or complex) enough 
--  to consider that. 




-- This is little strange. Our WLAN throws us
-- out every now and then. So, we'll reset our self
-- whenever we feel like nobody is talking to us.
-- And try to get into network again. That's what 
-- timer two does.

-- Let's connect to network. 
wifi.sta.disconnect();
wifi.setmode(wifi.STATION); 
wifi.sta.config("That_neighbour","shivendra332211"); 
print(wifi.sta.getip()); 
wifi.sta.autoconnect (1);
-- WLAN related reset alarm. Timeout period 20 sec.
tmr.alarm (2, 20000, 1, function ( ) 
      node.restart()
end) 

-- Waiting for router to respond
tmr.alarm (1, 1000, 1, function ( ) 
      if wifi.sta.getip ( ) == nil then 
         --print ("Waiting for Wifi connection"); 
      else 
         tmr.stop (1); 
         print ("ESP8266 mode is: " .. wifi.getmode ( )); 
         print ("The module MAC address is: " .. wifi.ap.getmac ( )); 
         print ("Config done, IP is " .. wifi.sta.getip ( )); 
      end 
end) 

-- Pins to talk with temperature sensor
select_1=0; 
select_2=1; 
gpio.mode(select_1,gpio.OUTPUT);  
gpio.mode(select_2,gpio.OUTPUT); 


-- We will use these pins for talking
-- with exhaust fan. 
-- 0th and 1st pins are already busy
-- talking with someone else ( temperature
-- sensors ).
l1_select=2
l2_select=3
l3_select=4

-- Set them to out.
gpio.mode(l1_select,gpio.OUTPUT)
gpio.mode(l2_select,gpio.OUTPUT)
gpio.mode(l3_select,gpio.OUTPUT)
			
-- This is a SPDT switch. I'm ignoring
-- one of these throw - unfortunately, I 
-- couldn't get SPST. Clearing a particular pin
-- attaches corresponding resistance to the gate of TRIAC.
-- So, Initially I set all of them so that circuit is 
-- open.
gpio.write(l1_select,gpio.HIGH)
gpio.write(l2_select,gpio.HIGH)
gpio.write(l3_select,gpio.HIGH)



-- This function controls power level.
-- Valid inputs are ON, 1, 2, 3, OFF.
-- It set/resets corresponding pins accordingly.
function ctrl_power(input)
	-- Get input
	if input=="ON"  then 
		gpio.write(l3_select,gpio.LOW)
		gpio.write(l1_select,gpio.HIGH)
		gpio.write(l2_select,gpio.HIGH)
		return 
	end
	if input=="OFF" then 
		gpio.write(l1_select,gpio.HIGH)
		gpio.write(l2_select,gpio.HIGH)
		gpio.write(l3_select,gpio.HIGH)
		return 
	end
	if input=="1" then 
		gpio.write(l1_select,gpio.LOW)
		gpio.write(l2_select,gpio.HIGH)
		gpio.write(l3_select,gpio.HIGH)
		return 
	end
	if input=="2" then 
		gpio.write(l1_select,gpio.HIGH)
		gpio.write(l2_select,gpio.LOW)
		gpio.write(l3_select,gpio.HIGH)
		return 
	end
	if input=="3" then 
		gpio.write(l1_select,gpio.HIGH)
		gpio.write(l2_select,gpio.HIGH)
		gpio.write(l3_select,gpio.LOW)
		return 
	end
end



-- Read device conf file and set corresponding ports.
-- Ok, Problem is that we might get disconnected 
-- from WLAN and in such case device will reboot itself
-- to reconnect with WLAN. So, In such case we don't want
-- exhaust fan to get reset.
-- That's why we store the power level in file and set 
-- that value on boot.
function read_set_dev_conf()
	files = file.list()   
	if files["power_level.conf"] then
		file.open( "power_level.conf", "r")
		power_level = file.readline()
		ctrl_power( power_level )
		file.close()
	end
end



-- Following function records and return temperature
-- Input is sensor number/name.
function record_sensor( sensor_name )
	if sensor_name == "1" then
		--Read Sensor one
		gpio.write(select_1,gpio.LOW); 
		gpio.write(select_2,gpio.LOW); 
		sensor_one_reading = adc.read(0);
		-- Ok, I have adopted these numbers from
		-- http://www.ti.com/lit/ds/symlink/lmt84.pdf pp. 10, 11
		-- and tuned a little bit based on my environment - This 
		-- might change a bit.
		return ((((345-sensor_one_reading)*100)/187)-12); 
	elseif sensor_name == "2" then
		--Read Sensor Two
		gpio.write(select_1,gpio.LOW); 
		gpio.write(select_2,gpio.HIGH);
		sensor_two_reading = adc.read(0); 
		return ((((345-sensor_two_reading)*100)/187)-12); 
	elseif sensor_name == "3" then
		--Read Sensor Three
		gpio.write(select_1,gpio.HIGH); 
		gpio.write(select_2,gpio.LOW); 
		sensor_three_reading = adc.read(0); 
		return ((((345-sensor_three_reading)*100)/187)-12); 
	elseif sensor_name == "4" then
		--Read Sensor Four
		gpio.write(select_1,gpio.HIGH); 
		gpio.write(select_2,gpio.HIGH); 
		sensor_four_reading = adc.read(0);  
		return ((((345-sensor_four_reading)*100)/187)-12); 
	end
end
	
-- Init device
read_set_dev_conf()



-- Create a server and set 30sec 
-- time out for a inactive client.
srv = net.createServer(net.TCP, 30)

-- Server listen on 80
-- Print HTTP headers to console
srv:listen(80,function(c)  
	c:on("receive", function(conn, payload)
		print(payload)
		temp_flag=0
		temp=-100
		if (string.find(payload, "POST /power/") ~= nil) then
			file.open( "power_level.conf","w+" )
		end
		if (string.find(payload, "POST /power/on") ~= nil) then
			ctrl_power("ON")
			file.write( "ON" )
		elseif (string.find(payload, "POST /power/off") ~= nil) then
			ctrl_power("OFF")
			file.write( "OFF" )
		elseif (string.find(payload, "POST /power/one") ~= nil) then
			ctrl_power("1")
			file.write( "1" )
		elseif (string.find(payload, "POST /power/two") ~= nil) then
			ctrl_power("2")
			file.write( "2" )
		elseif (string.find(payload, "POST /power/three") ~= nil) then
			ctrl_power("3")
			file.write( "3" )
		elseif (string.find(payload, "GET /temp?sensor=1") ~= nil) then
			temp=record_sensor("1")
			temp_flag=1
		elseif (string.find(payload, "GET /temp?sensor=2") ~= nil) then
			temp=record_sensor("2")
			temp_flag=1
		elseif (string.find(payload, "GET /temp?sensor=3") ~= nil) then
			temp=record_sensor("3")
			temp_flag=1
		elseif (string.find(payload, "GET /temp?sensor=4") ~= nil) then
			temp=record_sensor("4")
			temp_flag=1
		end
		if (string.find(payload, "POST /power/") ~= nil) then
			file.close()
		end
		
		if temp_flag == 1 then
			stuff="HTTP/1.1 200 OK\n\n"..temp.." deg"
			conn:send(stuff);
		else
			conn:send("HTTP/1.1 200 OK\n\n");
		end
		conn:close()
		tmr.stop(2); 
		tmr.alarm (2, 25000, 1, function ( ) 
      			node.restart()
		end)
	end)
end)  
