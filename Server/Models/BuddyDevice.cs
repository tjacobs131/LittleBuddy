public class BuddyDevice
{
    public Dictionary<int, List<BuddySensorReading>> SensorReadings { get; set; }
    public string DeviceName { get; set; } = "Buddy";

    public BuddyDevice()
    {
        SensorReadings = new Dictionary<int, List<BuddySensorReading>>();
    }

    public void RegisterDeviceName(string deviceName)
    {
        DeviceName = deviceName;
    }

    public void AddSensorReading(int sensorId, BuddySensorReading reading)
    {
        if (SensorReadings.ContainsKey(sensorId))
        {
            SensorReadings[sensorId].Add(reading);
        }
        else
        {
            SensorReadings.Add(sensorId, new List<BuddySensorReading> { reading });
        }
    }

    public string GetSensorReadingsJson()
    {
        if (SensorReadings == null || SensorReadings.Count == 0)
        {
            return "No sensor readings available.";
        }

        var readingsJson = string.Join(",", SensorReadings.Select(kvp =>
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
        if (SensorReadings == null || SensorReadings.Count == 0)
        {
            return "No sensor readings available.";
        }

        return $"[{GetSensorReadingsJson()}]";
    }
}