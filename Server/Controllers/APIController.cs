using System.Text.RegularExpressions;
using Microsoft.AspNetCore.Mvc;

namespace TTNMqttWebApi.Controllers
{
    [ApiController]
    [Route("api/payload")]
    public class BudController : ControllerBase
    {
        private readonly BuddyDataStore _messageStore;

        public BudController(BuddyDataStore messageStore)
        {
            _messageStore = messageStore;
        }

        [HttpGet]
        public IActionResult Get()
        {
            BuddyGroup latestReading = _messageStore.GetLatestReading();
            if (latestReading == null){
                return Ok("No data received yet");
            } else {
                return Ok(latestReading.ToJson());
            }
        }
    }
}
