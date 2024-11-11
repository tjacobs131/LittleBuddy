using Microsoft.AspNetCore.SignalR;

namespace SignalR.Hubs {
    public class BudHub : Hub
    {
        public async Task SendMessage(string user, string message)
        {
            await Clients.All.SendAsync("ReceiveMessage", user, message);
        }
    }
}
