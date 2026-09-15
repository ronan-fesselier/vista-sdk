using Test
using VistaSdk

@testset "Vis" begin
    @testset "instance is stable" begin
        v1 = vis()
        v2 = vis()
        @test versions(v1) == versions(v2)
        @test latest(v1) == latest(v2)
    end

    @testset "versions returns all vis versions" begin
        @test versions(vis()) == collect(instances(VisVersion))
    end

    @testset "versions are ordered" begin
        vs = versions(vis())
        for i = 1:(length(vs)-1)
            @test Int(vs[i]) < Int(vs[i+1])
        end
    end

    @testset "latest returns latest version" begin
        @test latest(vis()) == last(instances(VisVersion))
    end

    @testset "concurrent singleton access" begin
        results = Vector{Any}(undef, 10)
        tasks = map(1:10) do i
            Threads.@spawn begin
                v = vis()
                results[i] = (versions(v), latest(v))
            end
        end
        foreach(wait, tasks)
        for (vs, lat) in results
            @test vs == collect(instances(VisVersion))
            @test lat == last(instances(VisVersion))
        end
    end
end
