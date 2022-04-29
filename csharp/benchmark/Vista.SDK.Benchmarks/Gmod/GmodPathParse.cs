using Microsoft.Extensions.DependencyInjection;

using Vista.SDK;

namespace Vista.SDK.Benchmarks.Gmod;

[MemoryDiagnoser]
[Orderer(SummaryOrderPolicy.FastestToSlowest, MethodOrderPolicy.Declared)]
public class GmodPathParse
{
    private ServiceProvider _serviceProvider;
    private Vista.SDK.Gmod _gmod;

    [GlobalSetup]
    public async Task Setup()
    {
        var services = new ServiceCollection();

        services.AddVIS();

        _serviceProvider = services.BuildServiceProvider();

        var vis = _serviceProvider.GetRequiredService<IVIS>();
        _gmod = await vis.GetGmod(VisVersion.v3_4a);
    }

    [Benchmark]
    public bool TryParse() => GmodPath.TryParse("411.1/C101.72/I101", _gmod, out _);

    [GlobalCleanup]
    public async Task Cleanup()
    {
        await _serviceProvider.DisposeAsync();
    }
}
