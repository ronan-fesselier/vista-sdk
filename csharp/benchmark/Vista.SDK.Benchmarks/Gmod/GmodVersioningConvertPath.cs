using Microsoft.Extensions.DependencyInjection;

using Vista.SDK;

namespace Vista.Benchmarks.SDK;

[MemoryDiagnoser]
[Orderer(SummaryOrderPolicy.FastestToSlowest, MethodOrderPolicy.Declared)]
public class GmodVersioningConvertPath
{
    private ServiceProvider _serviceProvider;
    private Vista.SDK.Gmod _gmod;
    private GmodPath _gmodPath;
    private GmodVersioning _gmodVersioning;

    [GlobalSetup]
    public async Task Setup()
    {
        var services = new ServiceCollection();

        services.AddVIS();

        _serviceProvider = services.BuildServiceProvider();

        var vis = _serviceProvider.GetRequiredService<IVIS>();
        _gmod = await vis.GetGmod(VisVersion.v3_4a);
        _gmodPath = _gmod.ParsePath("411.1/C101.72/I101");
        _gmodVersioning = await vis.GetGmodVersioning();
    }

    [Benchmark]
    public async ValueTask<GmodPath> ConvertPath() =>
        await _gmodVersioning.ConvertPath(VisVersion.v3_4a, _gmodPath, VisVersion.v3_5a);

    [GlobalCleanup]
    public async Task Cleanup()
    {
        await _serviceProvider.DisposeAsync();
    }
}
