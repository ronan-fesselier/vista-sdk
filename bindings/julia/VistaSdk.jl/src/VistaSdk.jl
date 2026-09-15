module VistaSdk

include(joinpath(@__DIR__, "..", "deps", "deps.jl"))

include("core/vis_version.jl")

export versions, latest

end
