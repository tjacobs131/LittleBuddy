using System.Net.Security;
using System.Net.Mime;
using System;
using TTNMqttWebApi.Services;
using Microsoft.Extensions.DependencyInjection;
using SignalR.Hubs;


var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddControllers();

// Register the MQTT client background service
builder.Services.AddHostedService<MqttClientService>();

builder.Services.AddSingleton<BuddyDataStore>();

builder.Services.AddSignalR();

var app = builder.Build();

app.MapControllers();

// Configure the HTTP request pipeline.
app.UseHttpsRedirection();

app.UseDefaultFiles();
app.UseStaticFiles();

app.UseRouting();

app.UseAuthorization();

//app.MapHub<BuddyHub>("/BuddyHub");
app.UseEndpoints(endpoints =>
{
    _ = endpoints.MapHub<BudHub>("/hubs/payload");
});

app.Run();

Console.WriteLine("Running on: " + app.Urls);
