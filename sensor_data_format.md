# General structure
Each sensor's data must be packed into 3 bytes.<br>
Byte 1: SensorID<br>
Byte 2 and 3: Sensor value<br>
You can send a payload of any length, as long as the total length is divisible by 3.<br>
<br>
Sensor ID:<br>
00 RFID<br>
01 Temp_ Humid<br>
02 Temp_Degrees<br>
03 Sound_db<br>
04 Gas_ppb<br>
05 Gas_S02<br>
06 Gas_N02<br>
07 Gas_N0<br>
08 Gas_03<br>
09 Gas_C0<br>
10 Gas_C6H6<br>
<br>
Data:<br>
RFID<br> geef factor
Temp<br> factor 10
Sound<br> factor 10
Gas<br> factor 10
<br>
| Sensor | Possible payload (hex) | Description |
| ------ | --------------- | ----------- |
| RFID (SensorID = 0)  | 00 00 01 | Value bytes can range from 0 to 3. These represent each one of our names.
| Rest of sensors (SensorID=(1,2,3)) | 03 00 F2 | You can send an arbitrary amount of these in the payload example (slashes can be ignored, they are just spacers): <br> 01 00 00 / 02 00 4B / 02 00 FF / 03 00 F2 


