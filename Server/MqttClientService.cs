using System.Globalization;
using Microsoft.Extensions.Hosting;
using Microsoft.AspNetCore.SignalR;
using MQTTnet;
using MQTTnet.Client;
using System;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Extensions.Configuration;
using SignalR.Hubs;



namespace TTNMqttWebApi.Services
{
    public class MqttClientService : BackgroundService
    {
        private readonly IConfiguration _configuration;
        private readonly MessageStore _messageStore;
        private readonly string brokerAddress;
        private readonly string appId;
        private readonly string apiKey;
        private readonly IHubContext<BudHub> _hubContext;
        private string payload;

        private IMqttClient mqttClient;
        private MqttClientOptions options; 

        public MqttClientService(IConfiguration configuration, MessageStore messageStore)
        {
            _messageStore = messageStore;
            _configuration = configuration;
            // Read settings from configuration
            brokerAddress = _configuration["TTN:BrokerAddress"];
            appId = _configuration["TTN:AppId"];
            apiKey = _configuration["TTN:ApiKey"];
        }

        //internal IHubContext<BudHub> HubContext => _hubContext;
        private readonly IHubContext<BudHub> hubContext;

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            // Configure MQTT client options
            options = new MqttClientOptionsBuilder()
                .WithClientId(Guid.NewGuid().ToString())
                .WithTcpServer(brokerAddress, 8883)
                .WithCredentials(appId, apiKey)
                .WithTls() // Use TLS for secure connection
                .Build();

            // Create MQTT client
            var factory = new MqttFactory();
            mqttClient = factory.CreateMqttClient();

            // Handle incoming messages
            mqttClient.ApplicationMessageReceivedAsync += e =>
            {
                var payloadString = Encoding.UTF8.GetString(e.ApplicationMessage.Payload);
                _messageStore.UpdatePayload(payloadString);

                Console.WriteLine("### RECEIVED APPLICATION MESSAGE ###");
                Console.WriteLine($"+ Topic = {e.ApplicationMessage.Topic}");
                Console.WriteLine($"+ Payload = {payloadString}");
                Console.WriteLine($"This is the type: {e.GetType()}");
                
                hubContext.Clients.All.SendAsync("ReceiveMqttMessage", payloadString );
                payload = payloadString;

                
                return Task.CompletedTask;
            };
            
            async Task SendAsyncMessage()
            {
                
                    
                await hubContext.Clients.All.SendAsync("ReceiveNumber", payload);

                    //await Task.Delay(1000);
                
            }

            // Handle connection
            mqttClient.ConnectedAsync += async e =>
            {
                Console.WriteLine("### CONNECTED WITH TTN MQTT BROKER ###");

                // Subscribe to uplink messages from all devices in your application
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

            // Connect to MQTT broker
            try
            {
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
    }

}
