**Notice:** _This repository is protected with [license](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/LICENSE.md), corporate/commercial use is restricted._

# Interfacing Node MCU with LMT84LP temperature sensor and controlling Actuator 
#### What does it do?
Following objective is achieved:
1. For monitoring temperature sensor readings on Graphite:

|**No.**| **Operation**                             |  **File name**                                           |
|-------|-------------------------------------------|----------------------------------------------------------|
| 1. | a) Read temperature readings from LMT84 temperature sensor. | [read_sensor_update_server.lua](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/read_sensor_update_server.lua) |
| 2. | a) Read temperature sensor data from HTTP server (Node MCU). <br /> b) Trigger an E-mail if event happens | [read_sensor_trigger_email.py](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/read_sensor_trigger_email.py)|
|3. | a) Push Data to Graphite | [feed_data_tographite.sh](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/feed_data_tographite.sh)

2. For storing temperature readings in SQL and controlling actuator:

|**No.**| **Operation**                             |  **File name**                                           |
|-------|-------------------------------------------|----------------------------------------------------------|
| 1. | a) Read temperature readings from LMT84 temperature sensor. <br /> b) Allow access of temperature with the help of REST APIs <br /> c) Allow control of actuators through Rest APIs | [rest_api_control.lua](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/rest_api_control.lua)|
| 2. | a) Read temperature sensor data from HTTP server (Node MCU). <br /> b) Trigger an E-mail if event happens | [read_sensor_trigger_email.py](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/read_sensor_trigger_email.py)|
|3. | a) Push temperature Data to SQL (And control actuators on web app, plus plot the data) | [push_pull_app.cpp](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/push_pull_app.cpp)



#### How to use?
1. Load .lua file to node MCU.
2. Compile .py file in terminal.
3. Do "make all" in repository folder. It will generate pull_push_app binary, which have to be started at system boot time (wrap this binary with a script kept in init.d).

#### Block Diagram of circuit
![Block Diagram](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/whole.png)

#### Environment
1. Node MCU v0.9 and Python 2.7.3
2. GCC 5.4

#### Demo Video

[Following](https://youtu.be/NXFKr1k28os) is a demo video, shows temperature sensor data plotted on Graphite:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=NXFKr1k28os
" target="_blank"><img src="http://img.youtube.com/vi/NXFKr1k28os/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a> 
