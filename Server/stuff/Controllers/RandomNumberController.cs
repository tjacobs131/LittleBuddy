using System.Xml.Serialization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;


namespace NumberReader.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class RandomNumberController : ControllerBase
    {
        
       private readonly RandomNumberService _randomNumberService;

        public RandomNumberController(RandomNumberService randomNumberService)
        {
            _randomNumberService = randomNumberService;
        }
        

        [HttpPost("Start")]
        public IActionResult Start()
        {
            _randomNumberService.StartGenerating();
            return Ok("Started with generating");
        }

        [HttpPost("Stop")]
        public IActionResult Stop()
        {
            int firstNumber = _randomNumberService.EndGenerating();
            return Ok($"Ended Generation. Here is the first number: {firstNumber}");
        }

        [HttpPost("Median")]
        public IActionResult GetMedian(){
            double median = _randomNumberService.CalculateMedian();
            return Ok($"median: {median}");
        }
    }
}
