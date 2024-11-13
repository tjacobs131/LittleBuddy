
using Microsoft.AspNetCore.SignalR;
using SignalRChat.Hubs;
using System;


public class RandomNumberService
{
    private bool generating;
    public List<int> numbersList;
    private int currentNumber;
    private Random random = new Random();
    private int delay;
    private readonly IHubContext<ChatHub> hubContext;

    
    public RandomNumberService(IHubContext<ChatHub> hContext){

        hubContext = hContext;
        numbersList = new List<int>();
        generating = false;
        delay = 1000;

    }

    public void StartGenerating(){
        generating = true;
        numbersList.Clear();
        Task.Run(()=> GenerateNumbers());
    }

    public int EndGenerating(){
        generating = false;
        return numbersList[0];
    }

    public double CalculateMedian(){
        return numbersList.Average();
    }

    private async Task GenerateNumbers(){

        while(generating){
            currentNumber = random.Next(1,100);
            Console.WriteLine(currentNumber);  
            await hubContext.Clients.All.SendAsync("ReceivedNumber", currentNumber);
            numbersList.Add(currentNumber);
            await Task.Delay(delay);
        }

    }
}
