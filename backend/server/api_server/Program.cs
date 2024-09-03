using System.Xml.Serialization;
using api_server.Dtos;
using YourNamespaceName.Data;

var builder = WebApplication.CreateBuilder(args);

// Add CORS services
builder.Services.AddCors(options =>
{
    options.AddPolicy("AllowAll",
        policy =>
        {
            policy.WithOrigins("http://localhost:3000")
                  .AllowAnyMethod()
                  .AllowAnyHeader();
        });
});

var app = builder.Build();

// Use CORS
app.UseCors("AllowAll");

List<WordQuerys> words = new List<WordQuerys>
{
    new("word", new List<string> { "www.mmm.vom", "www.ppp.com", "www.kkk.vom", "www.lll.vom" }),
    new("world", new List<string> { "https://www.ynet.co.il/news", "www.lkoppppll.vom" }),
    new("is", new List<string> { "www.israel.vom", "www.omea.vom" }),
};

app.MapGet("words/{word}", (string word) =>
{
    var result = DatabaseService.GetUrlsForWord(word);

    if (result == null || result.Count == 0) // Corrected syntax for logical OR and list length check
    {
        return Results.NotFound(); // Return 404 if not found
    }

    return Results.Ok(result); // Return the list of URLs directly
});


app.Run();
