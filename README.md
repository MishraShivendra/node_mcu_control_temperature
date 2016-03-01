# Interfacing Node MCU with LMT84LP temprature sensor
####What does it do?
Following objective is achived:

|**No.**| **Operation**                             |  **File name**                                           |
|-------|-------------------------------------------|----------------------------------------------------------|
|1. | a) Read temprature readings from LMT84 temprature sensor. <br /> b) Upadte the readings on Http server (Node MCU).    | [read_sensor_update_server.lua](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/read_sensor_update_server.lua)|
|2. | a) Read temprature sensor data from Http server (Node MCU). <br /> b) Trigger an e-mail if event happens | [read_sensor_trigger_email.py](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/read_sensor_trigger_email.py)| 

####How to use?
1. Load .lua file to node MCU.
2. Compile .py file in terminal.

####Bloack Diagram of circuit
![Block Diagram](https://github.com/MishraShivendra/node_mcu_py_email/blob/master/block.png)


