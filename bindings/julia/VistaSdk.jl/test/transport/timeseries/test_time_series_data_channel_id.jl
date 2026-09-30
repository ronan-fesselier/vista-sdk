using Test
using VistaSdk

const LOCAL_ID_STR = "/dnv-v2/vis-3-4a/411.1/C101.62/S205/meta/qty-temperature/cnt-lubricating.oil/pos-inlet"

@testset "TsdChannelId" begin
    @testset "from local_id string" begin
        id = from_string(TsdChannelId, LOCAL_ID_STR)
        @test id !== nothing
        @test is_local_id(id)
        @test !is_short_id(id)
        @test local_id(id) !== nothing
        @test short_id(id) === nothing
    end

    @testset "from short_id string" begin
        id = from_string(TsdChannelId, "CH001")
        @test id !== nothing
        @test !is_local_id(id)
        @test is_short_id(id)
        @test short_id(id) == "CH001"
        @test local_id(id) === nothing
    end

    @testset "from_string empty returns nothing" begin
        @test from_string(TsdChannelId, "") === nothing
    end

    @testset "invalid local_id becomes short_id" begin
        id = from_string(TsdChannelId, "/invalid/local/id")
        @test id !== nothing
        @test !is_local_id(id)
        @test is_short_id(id)
        @test short_id(id) == "/invalid/local/id"
    end

    @testset "equality - same local_ids" begin
        id1 = from_string(TsdChannelId, LOCAL_ID_STR)
        id2 = from_string(TsdChannelId, LOCAL_ID_STR)
        @test id1 == id2
    end

    @testset "equality - same short_ids" begin
        id1 = from_string(TsdChannelId, "CH001")
        id2 = from_string(TsdChannelId, "CH001")
        @test id1 == id2
    end

    @testset "inequality - different types" begin
        lid = from_string(TsdChannelId, LOCAL_ID_STR)
        sid = from_string(TsdChannelId, "CH001")
        @test lid != sid
    end

    @testset "inequality - different short_ids" begin
        id1 = from_string(TsdChannelId, "CH001")
        id2 = from_string(TsdChannelId, "CH002")
        @test id1 != id2
    end

    @testset "to_string - local_id" begin
        id = from_string(TsdChannelId, LOCAL_ID_STR)
        @test string(id) == LOCAL_ID_STR
    end

    @testset "to_string - short_id" begin
        id = from_string(TsdChannelId, "CH001")
        @test string(id) == "CH001"
    end
end
