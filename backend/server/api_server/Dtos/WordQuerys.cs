namespace api_server.Dtos;

public record class WordQuerys(
    string Word,
    List<string> Urls
)//TODO add a date and time paramers
{

}
