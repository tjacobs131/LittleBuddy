using System.Text.RegularExpressions;

public class BuddyDataStore
{
    private readonly ReaderWriterLockSlim _lock = new ReaderWriterLockSlim();
    private BuddyGroup? lastBuddyGroupState = new BuddyGroup();

    public void UpdateReading(BuddyGroup groupReading)
    {
        lastBuddyGroupState = null;
        _lock.EnterWriteLock();
        try
        {
            lastBuddyGroupState = groupReading;
        }        
        finally
        {
            _lock.ExitWriteLock();
        }
    }

    public BuddyGroup GetLatestReading()
    {

        Console.WriteLine(lastBuddyGroupState.ToJson());
        _lock.EnterReadLock();
        try
        {
            return lastBuddyGroupState!;
        }
        finally
        {
            _lock.ExitReadLock();
        }
    }
}
