public class BuddyGroup
{
    public Dictionary<string, BuddyDevice> BuddyDevices { get; set; }

    public BuddyGroup()
    {
        BuddyDevices = new Dictionary<string, BuddyDevice>();
    }

    public BuddyDevice GetBuddyDevice(string deviceId)
    {
        return BuddyDevices.ContainsKey(deviceId) ? BuddyDevices[deviceId] : null;
    }

    public void AddBuddyDevice(string deviceId, BuddyDevice device)
    {
        BuddyDevices[deviceId] = device;
    }

    public string ToJson()
    {
        if (BuddyDevices == null || BuddyDevices.Count == 0)
        {
            return "No buddy readings available.";
        }

        var devicesJson = string.Join(",", BuddyDevices.Select(kvp =>
        {
            var deviceId = kvp.Key;
            var buddyDevice = kvp.Value;
            var buddyName = kvp.Value.DeviceName;
            var readingsJson = buddyDevice.ToJson();

            return $"{{\"DeviceID\":\"{deviceId}\",\"Username\":\"{buddyName}\",\"BuddyDevice\":{readingsJson}}}";
        }));

        return $"[{devicesJson}]";
    }
}