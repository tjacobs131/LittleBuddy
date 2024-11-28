# General structure
Each sensor's data must be packed into 3 bytes.<br>
Byte 1: SensorID<br>
Byte 2 and 3: Sensor value<br>
You can send a payload of any length, as long as the total length is divisible by 3.

Sensor ID:
00 RFID
01 Temp_ Himid
02 Temp_Degrees
03 Sound_db
04 Gas_ppb
05 Gas_S02
06 Gas_N02
07 Gas_N0
08 Gas_03
09 Gas_C0
10 Gas_C6H6

Data:
RFID
Temp
Sound
Gas

| Sensor | Possible payload (hex) | Description |
| ------ | --------------- | ----------- |
| RFID (SensorID = 0)  | 00 00 01 | Value bytes can range from 0 to 3. These represent each one of our names.
| Rest of sensors (SensorID=(1,2,3)) | 03 00 F2 | You can send an arbitrary amount of these in the payload example (slashes can be ignored, they are just spacers): <br> 01 00 00 / 02 00 4B / 02 00 FF / 03 00 F2 


