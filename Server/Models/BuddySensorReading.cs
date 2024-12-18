public class BuddySensorReading
{
    public int Value { get; set; }
    public DateTime Time { get; set; }

    public BuddySensorReading(int value, DateTime time)
    {
        Value = value;
        Time = time;
    }
    
}