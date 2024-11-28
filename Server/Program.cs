using System.Net.Security;
using System.Net.Mime;
using System;
using TTNMqttWebApi.Services;
using Microsoft.Extensions.DependencyInjection;
using SignalR.Hubs;


WebApplicationBuilder builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddControllers();
builder.Services.AddHostedService<MqttClientService>();
builder.Services.AddSingleton<BuddyDataStore>();
builder.Services.AddSignalR();

var app = builder.Build();

app.MapControllers();
app.UseHttpsRedirection();
app.UseDefaultFiles();
app.UseStaticFiles();
app.UseRouting();
app.UseAuthorization();

// Expose the SignalR connection endpoint
app.UseEndpoints(endpoints =>
{
    _ = endpoints.MapHub<BudHub>("/hubs/payload");
});

app.Run();