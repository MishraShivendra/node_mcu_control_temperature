# Interfacing Node MCU with LMT84LP temperature sensor
####What does it do?
Following objective is achieved:

|**No.**| **Operation**                             |  **File name**                                           |
|-------|-------------------------------------------|----------------------------------------------------------|
|1. | a) Read temperature readings from LMT84 temperature sensor. <br /> b) Update the readings on HTTP server (Node MCU).    | [read_sensor_update_server.lua](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/read_sensor_update_server.lua)|
|2. | a) Read temperature sensor data from HTTP server (Node MCU). <br /> b) Trigger an E-mail if event happens | [read_sensor_trigger_email.py](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/read_sensor_trigger_email.py)| 

####How to use?
1. Load .lua file to node MCU.
2. Compile .py file in terminal.

####Block Diagram of circuit
![Block Diagram](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/block.png)

####Environment
Node MCU v0.9 and Python 2.7.3

####Extension(TBD)
1. Yet to add a thread to read temperature data from server(Node MCU) and plot a graph.
