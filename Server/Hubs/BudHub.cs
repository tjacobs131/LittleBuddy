using Microsoft.AspNetCore.SignalR;

namespace SignalR.Hubs {
    public class BudHub : Hub
    {
        BuddyDataStore readingsStore;

        public BudHub(BuddyDataStore readingsStore) {
            this.readingsStore = readingsStore;
        }

        public async Task SendPayload(string payload)
        {
            await Clients.All.SendAsync("ReceiveMessage", payload);
        }

        public override async Task OnConnectedAsync(){
            await base.OnConnectedAsync();

            var latestReadings = readingsStore.GetLatestReading();

            if (latestReadings != null)
            {
                await Clients.Caller.SendAsync("ReceivePayload", latestReadings.ToJson());
            }
            else
            {
                await Clients.Caller.SendAsync("ReceivePayload", "No sensor readings available.");
            }
        }
    }
}
