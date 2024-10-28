using System.Threading;

public class MessageStore
{
    private readonly ReaderWriterLockSlim _lock = new ReaderWriterLockSlim();
    private string _latestPayload;

    public void UpdatePayload(string payload)
    {
        _latestPayload = "";
        _lock.EnterWriteLock();
        try
        {
            _latestPayload = payload;
        }        
        finally
        {
            _lock.ExitWriteLock();
        }
    }

    public string GetLatestPayload()
    {
        _lock.EnterReadLock();
        try
        {
            return _latestPayload;
        }
        finally
        {
            _lock.ExitReadLock();
        }
    }
}
