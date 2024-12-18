public class BuddyGroup
{
    public Dictionary<string, BuddyDevice> BuddyDevices { get; set; }

    public BuddyGroup()
    {
        BuddyDevices = new Dictionary<string, BuddyDevice>();
    }

    public BuddyDevice GetBuddyDevice(string deviceId)
    {
        return BuddyDevices.ContainsKey(deviceId) ? BuddyDevices[deviceId] : throw new KeyNotFoundException();
    }

    public void AddBuddyDevice(string userName, BuddyDevice device)
    {
        BuddyDevices[userName] = device;
    }

    public string ToJson()
    {
        if (BuddyDevices == null || BuddyDevices.Count == 0)
        {
            return "No devices connected.";
        }

        var devicesJson = string.Join(",", BuddyDevices.Select(kvp =>
        {
            var deviceId = kvp.Key;
            var buddyDevice = kvp.Value;
            var buddyName = kvp.Value.UserName;
            var readingsJson = buddyDevice.ToJson();

            return $"{{\"DeviceID\":\"{deviceId}\",\"Username\":\"{buddyName}\",\"BuddyDevice\":{readingsJson}}}";
        }));

        return $"[{devicesJson}]";
    }
}