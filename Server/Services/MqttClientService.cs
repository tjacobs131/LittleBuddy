using Microsoft.AspNetCore.SignalR;
using MQTTnet;
using MQTTnet.Client;
using System.Text;
using SignalR.Hubs;
using Newtonsoft.Json;
using System.Text.RegularExpressions;

namespace TTNMqttWebApi.Services
{
    #region  Internal classes for payload processing
    internal class Payload
    {
        public required UplinkMessage uplink_message { get; set; }
        public required EndDeviceIds end_device_ids { get; set; }
        public required DateTime received_at { get; set; }
    }

    internal class UplinkMessage
    {
        public required string frm_payload { get; set; }
    }

    internal class EndDeviceIds
    {
        public required string device_id { get; set; }
    }
    #endregion

    public class MqttClientService : BackgroundService
    {
        // TTN app configuration
        private readonly IConfiguration _configuration;
        private readonly string brokerAddress;
        private readonly string appId;
        private readonly string apiKey;

        // SignalR hub
        private readonly IHubContext<BudHub> _hubContext;

        private IMqttClient? mqttClient;
        private MqttClientOptions? options;

        BuddyGroup previousBuddyGroup = new BuddyGroup(); // For memory persistence

        private readonly BuddyDataStore buddyDataStore;

        public MqttClientService(BuddyDataStore buddyDataStore, IConfiguration configuration, IHubContext<BudHub> hubContext)
        {
            this.buddyDataStore = buddyDataStore;
            _configuration = configuration;
            _hubContext = hubContext;

            // Read TTN connection settings from configuration
            brokerAddress = _configuration["TTN:BrokerAddress"]!;
            appId = _configuration["TTN:AppId"]!;
            apiKey = _configuration["TTN:ApiKey"]!;
        }

        private void ConfigureMqttClient(CancellationToken stoppingToken)
        {
            options = new MqttClientOptionsBuilder()
                .WithClientId(Guid.NewGuid().ToString())
                .WithTcpServer(brokerAddress, 8883)
                .WithCredentials(appId, apiKey)
                .WithTlsOptions(o =>
                {
                    o.UseTls();
                    o.WithSslProtocols(System.Security.Authentication.SslProtocols.Tls12);
                    o.WithAllowUntrustedCertificates(false); // Reject untrusted certificates
                    o.WithIgnoreCertificateChainErrors(false); // Validate the entire chain
                    o.WithIgnoreCertificateRevocationErrors(false); // Check for revoked certificates
                })
                .Build();

            var factory = new MqttFactory();
            mqttClient = factory.CreateMqttClient();

            // Handle incoming messages
            mqttClient.ApplicationMessageReceivedAsync += e =>
            {
                // Payload received from TTN
                string? payloadString = Encoding.UTF8.GetString(e.ApplicationMessage.PayloadSegment);

                Console.WriteLine("### RECEIVED APPLICATION MESSAGE ###");
                Console.WriteLine($"+ Topic = {e.ApplicationMessage.Topic}");
                Console.WriteLine($"+ Payload = {payloadString}");

                // Deserialize payload
                Payload readablePayload;
                try
                {
                    readablePayload = JsonConvert.DeserializeObject<Payload>(payloadString)!;
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error deserializing payload: {ex.Message}");
                    return Task.CompletedTask;
                }

                if (string.IsNullOrEmpty(readablePayload.uplink_message.frm_payload))
                {
                    Console.WriteLine("frm_payload is null or empty");
                    return Task.CompletedTask;
                }

                // Isolate the ESP32's payload
                byte[] payloadBytes;
                payloadBytes = Convert.FromBase64String(readablePayload.uplink_message.frm_payload);

                // Ensure payload length is a multiple of 3
                if (payloadBytes.Length < 3 || payloadBytes.Length % 3 != 0)
                {
                    Console.WriteLine("Incorrect amount of payload bytes");
                    return Task.CompletedTask;
                }
                
                // string deviceID = readablePayload.end_device_ids.device_id;
                // BuddyDevice buddyDevice;

                // try {
                //     buddyDevice = previousBuddyGroup.GetBuddyDevice(deviceID);
                // } catch (KeyNotFoundException)
                // {
                //     Console.WriteLine($"### REGISTERING NEW DEVICE: {deviceID} ###");
                //     buddyDevice = new BuddyDevice();
                // }
                
                // Process the payload in 3 byte chunks
                string username = "";
                for (int i = 0; i < payloadBytes.Length; i += 3)
                {
                    int sensorID = payloadBytes[i];
                    int value = (payloadBytes[i + 1] << 8) | payloadBytes[i + 2]; // The sensor's reading is represented in the last two bytes
                    
                    if (sensorID == 0){ // RFID sensor code, we are setting the username
                        switch(value) {
                            case 0:
                                username = "Koen van Wijlick";
                                break;
                            case 1:
                                username = "Koen Dirckx";
                                break;
                            case 2:
                                username = "James Boogaard";
                                break;
                            case 3:
                                username = "Tom Jacobs";
                                break;
                            default:
                                username = "Buddy";
                                break;
                        }
                        Console.WriteLine($"### {username} is now a registered user ###");

                        previousBuddyGroup.AddUser(username);

                        // buddyDevice.RegisterNewUser(username);
                        // previousBuddyGroup.AddBuddyDevice(username, buddyDevice);

                        continue;
                    }
                    
                    // Create SensorReading new sensor reading
                    BuddySensorReading sensorReading = new BuddySensorReading(value/10, readablePayload.received_at);

                    // 
                    UserCreation user = previousBuddyGroup.GetUser(username);
                    // buddyDevice.AddSensorReading(sensorID, sensorReading);

                    user.AddSensorValue(sensorID, sensorReading);
                    // previousBuddyGroup.AddBuddyDevice(username, buddyDevice);
                }

                // Update the reading for use when opening the page the first time
                buddyDataStore.UpdateReading(previousBuddyGroup);

                // Send all readings after processing
                _ = SendSensorReadings();

                return Task.CompletedTask;
            };

            // Handle connection
            mqttClient.ConnectedAsync += async e =>
            {
                Console.WriteLine("### CONNECTED WITH TTN MQTT BROKER ###");

                var topicFilter = $"v3/{appId}@ttn/devices/+/up";
                await mqttClient.SubscribeAsync(new MqttTopicFilterBuilder().WithTopic(topicFilter).Build());

                Console.WriteLine($"### SUBSCRIBED TO TOPIC: {topicFilter} ###");
            };

            // Handle disconnection
            mqttClient.DisconnectedAsync += async e =>
            {
                Console.WriteLine("### DISCONNECTED FROM TTN MQTT BROKER ###");
                await Task.Delay(TimeSpan.FromSeconds(5), stoppingToken);

                try
                {
                    await mqttClient.ConnectAsync(options, stoppingToken);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"### RECONNECTING FAILED ###\n{ex.Message}");
                }
            };
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            ConfigureMqttClient(stoppingToken);

            // Connect to MQTT broker
            try
            {
                if (mqttClient == null || options == null)
                {
                    Console.WriteLine("MQTT client or options is not configured.");
                    return;
                }

                await mqttClient.ConnectAsync(options, stoppingToken);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"### CONNECTING FAILED ###\n{ex.Message}");
                return;
            }

            // Keep the task running until cancellation is requested
            await Task.Delay(Timeout.Infinite, stoppingToken);
        }

        private async Task SendSensorReadings(){

            string json = previousBuddyGroup.ToJson();
            
            Console.WriteLine("### SENDING JSON TO FRONTEND ###");
            Console.WriteLine($"+ Frontend Json = {json}");

            await SendAsyncMessage(json);
        }

        private async Task SendAsyncMessage(string payload)
        {
            await _hubContext.Clients.All.SendAsync("ReceivePayload", payload);
        }
    }
}
