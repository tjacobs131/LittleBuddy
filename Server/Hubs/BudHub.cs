using Microsoft.AspNetCore.SignalR;

namespace SignalR.Hubs {
    public class BudHub : Hub
    {
        public async Task SendPayload(string payload)
        {
            await Clients.All.SendAsync("ReceivedMessage", payload);
        }
    }
}
