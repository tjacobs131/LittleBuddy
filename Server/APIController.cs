using Microsoft.AspNetCore.Mvc;

namespace TTNMqttWebApi.Controllers
{
    [ApiController]
    [Route("api/test")]
    public class BudController : ControllerBase
    {
        [HttpGet]
        public IActionResult Get()
        {
            // Return some data or status
            return Ok("Peenuts");
        }
    }
}
