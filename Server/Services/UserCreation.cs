
//PROPERTIES
//user sensorsCount?
//Maybe a map that has a sensor id mapped to an array of the historic sensor values
//userName or deviceId

//METHODS
//AddSensorValue(sensor,value)
//RetrieveUserValues(User/DeviceID)

//## Just get the time series data from the time the payload sends

//## Now the data sent in the mqtt string only relates directly to what was sent from things network and no data persistence within the string.

using System.Collections.Generic;

namespace TTNMqttWebApi.Services{


class UserCreation{

        Dictionary<int, int[]> sensorDictionary = new Dictionary<int, int[]>();
        String userName = "No User Found!!";
        public UserCreation(Dictionary<int, int[]> iSensorDic, String userName){
            this.sensorDictionary = iSensorDic;
            this.userName = userName;
        }

        public void AddSensorValue(int SensorNumber, int sensorValue){
            int[] tempArray = this.sensorDictionary[SensorNumber];
            tempArray[tempArray.Length] = sensorValue;
        }

        public int[] RetrieveUserValues(int SensorID){
            return this.sensorDictionary[SensorID];
        }
    }
}