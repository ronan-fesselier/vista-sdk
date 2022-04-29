using Microsoft.Extensions.DependencyInjection;

namespace Vista.SDK;

public static class DIExtensions
{
    public static IServiceCollection AddVIS(this IServiceCollection services)
    {
        services.AddSingleton<IVIS, VIS>();
        return services;
    }
}
