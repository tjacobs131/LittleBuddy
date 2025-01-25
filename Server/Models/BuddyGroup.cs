using TTNMqttWebApi.Services;


public class BuddyGroup
{
    public Dictionary<string, UserCreation> Users = new Dictionary<string, UserCreation>();

    // public BuddyGroup()
    // {
       
    // }

    // public BuddyDevice GetBuddyDevice(string deviceId)
    // {
    //     return BuddyDevices.ContainsKey(deviceId) ? BuddyDevices[deviceId] : throw new KeyNotFoundException();
    // }

    // public void AddBuddyDevice(string userName, BuddyDevice device)
    // {
    //     BuddyDevices[userName] = device;
    // }
    
    public void AddUser(string UserName)
    {
        if (!Users.ContainsKey(UserName))
        {
            Users.Add(UserName, new UserCreation(UserName));
        }
    }

    public UserCreation GetUser(string UserName){
        return Users[UserName];
    }

    public string ToJson()
    {
        if (Users == null || Users.Count == 0)
        {
            return "No devices connected.";
        }

        var usersJson = string.Join(",", Users.Select(kvp =>
        {
            var userName = kvp.Key;
            var user = kvp.Value;

            // Call the user's ToJson
            var userJson = user.ToJson();

            return $"{{\"Username\":\"{userName}\",\"Sensors\":{userJson}}}";
        }));

        return $"[{usersJson}]";
    }
}