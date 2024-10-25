using System;
using System.Text;
using System.Threading.Tasks;
using MQTTnet;
using MQTTnet.Client;

namespace TTNMqttClient
{
    class Program
    {
        static async Task Main(string[] args)
        {
            // MQTT broker details
            var brokerAddress = "eu1.cloud.thethings.network"; // Change to your TTN cluster
            var appId = "bud-2-boogaloo";
            var apiKey = "NNSXS.CAE6SDIJOHYDNGLYPIK4SCP2BSDPENAJC2LDU7I.WCLIQRPIIFV47ZPHWV6RGMJ443KG5ESSDAWE5WFVYPACOEE75BDA";

            // Create MQTT client options
            var options = new MqttClientOptionsBuilder()
                .WithClientId(Guid.NewGuid().ToString())
                .WithTcpServer(brokerAddress, 8883)
                .WithCredentials(appId, apiKey)
                .WithTls() // Use TLS for secure connection
                .Build();

            // Create MQTT client
            var factory = new MqttFactory();
            var mqttClient = factory.CreateMqttClient();

            // Handle incoming messages
            mqttClient.ApplicationMessageReceivedAsync += e =>
            {
                Console.WriteLine("### RECEIVED APPLICATION MESSAGE ###");
                Console.WriteLine($"+ Topic = {e.ApplicationMessage.Topic}");
                Console.WriteLine($"+ Payload = {Encoding.UTF8.GetString(e.ApplicationMessage.Payload)}");
                Console.WriteLine();

                return Task.CompletedTask;
            };

            // Handle connection
            mqttClient.ConnectedAsync += async e =>
            {
                Console.WriteLine("### CONNECTED WITH TTN MQTT BROKER ###");

                // Subscribe to uplink messages from all devices in your application
                var topicFilter = $"v3/bud-2-boogaloo@ttn/devices/bud-2-boogaloo/up";
                await mqttClient.SubscribeAsync(new MqttTopicFilterBuilder().WithTopic(topicFilter).Build());

                Console.WriteLine($"### SUBSCRIBED TO TOPIC: {topicFilter} ###");
            };

            // Handle disconnection
            mqttClient.DisconnectedAsync += async e =>
            {
                Console.WriteLine("### DISCONNECTED FROM TTN MQTT BROKER ###");
                await Task.Delay(TimeSpan.FromSeconds(5));

                try
                {
                    await mqttClient.ConnectAsync(options, CancellationToken.None);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"### RECONNECTING FAILED ###\n{ex.Message}");
                }
            };

            // Connect to MQTT broker
            try
            {
                await mqttClient.ConnectAsync(options, CancellationToken.None);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"### CONNECTING FAILED ###\n{ex.Message}");
                return;
            }

            Console.WriteLine("Press [Enter] to exit.");
            Console.ReadLine();

            // Disconnect
            await mqttClient.DisconnectAsync();
        }
    }
}