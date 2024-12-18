
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


public class UserCreation{

        Dictionary<int, List<BuddySensorReading>> sensorDictionary = new Dictionary<int, List<BuddySensorReading>>();
        String userName = "No User Found!!";
        public UserCreation(String userName){
            this.userName = userName;
        }

        public void AddSensorValue(int SensorNumber, BuddySensorReading sensorValue){
            try{
                this.sensorDictionary[SensorNumber].Add(sensorValue);
            }catch{
                
                this.sensorDictionary.Add(SensorNumber, new List<BuddySensorReading>());
                this.sensorDictionary[SensorNumber].Add(sensorValue);
            }
            
        }

        public List<BuddySensorReading> RetrieveUserValues(int SensorID){
            return this.sensorDictionary[SensorID];
        }

        public string GetSensorReadingsJson()
        {
            if (sensorDictionary == null || sensorDictionary.Count == 0)
            {
                return "No sensor readings available.";
            }

            var readingsJson = string.Join(",", sensorDictionary.Select(kvp =>
            {
                var sensorId = kvp.Key;
                var readingsList = kvp.Value;

                var readingsArrayJson = string.Join(",", readingsList.Select(reading =>
                    $"{{\"Timestamp\":\"{reading.Time:O}\",\"Value\":{reading.Value}}}"
                ));

                return $"{{\"SensorID\":{sensorId},\"Readings\":[{readingsArrayJson}]}}";
            }));

        return readingsJson;
    }
        public string ToJson()
        {
        if (sensorDictionary == null || sensorDictionary.Count == 0)
        {
            return "No sensor readings available.";
        }

        return $"[{GetSensorReadingsJson()}]";
        }

    }
