using Microsoft.AspNetCore.Mvc;

namespace TTNMqttWebApi.Controllers
{
    [ApiController]
    [Route("api/test")]
    public class BudController : ControllerBase
    {
        private readonly MessageStore _messageStore;

        public BudController(MessageStore messageStore)
        {
            _messageStore = messageStore;
        }

        [HttpGet]
        public IActionResult Get()
        {
            var latestPayload = _messageStore.GetLatestPayload();
            return Ok(latestPayload ?? "No data received yet");
        }
    }
}
