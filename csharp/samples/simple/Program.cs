using System.Globalization;
using Vista.SDK.Transport.Json;

var sourceData = new[]
{
    new BallastEvent(
        Mode: "Stripping",
        Location: "IMO",
        Start: DateTimeOffset.Parse("2023-09-13T21:04:38.00Z", CultureInfo.InvariantCulture),
        Stop: DateTimeOffset.Parse("2023-09-13T23:56:00.00Z", CultureInfo.InvariantCulture),
        Volume: 740,
        UVTreatment: 92,
        Salinity: 0,
        Position: (62.84933, 7.120334)
    )
};
var package = TimeSeries.GetPackage(sourceData);
var json = package.Serialize();

Console.WriteLine($"Produced TimeSeriesDataPackage:\n{json}");
