@testset "SerializableDocument: null constructor" begin
    doc = SerializableDocument()
    @test kind(doc) == DocNull
    @test is_null(doc)
    @test !is_boolean(doc)
    @test !is_integer(doc)
    @test !is_double(doc)
    @test !is_string(doc)
    @test !is_array(doc)
    @test !is_object(doc)
end

@testset "SerializableDocument: boolean constructor" begin
    t = SerializableDocument(true)
    f = SerializableDocument(false)
    @test kind(t) == DocBoolean
    @test is_boolean(t)
    @test as_boolean(t) == true
    @test as_boolean(f) == false
end

@testset "SerializableDocument: integer constructor" begin
    doc = SerializableDocument(Int64(42))
    @test kind(doc) == DocInteger
    @test is_integer(doc)
    @test as_integer(doc) == Int64(42)
end

@testset "SerializableDocument: integer constructor (generic Integer)" begin
    doc = SerializableDocument(7)
    @test is_integer(doc)
    @test as_integer(doc) == Int64(7)
end

@testset "SerializableDocument: double constructor" begin
    doc = SerializableDocument(3.14)
    @test kind(doc) == DocDouble
    @test is_double(doc)
    @test as_double(doc) ≈ 3.14
end

@testset "SerializableDocument: string constructor" begin
    doc = SerializableDocument("hello")
    @test kind(doc) == DocString
    @test is_string(doc)
    @test as_string(doc) == "hello"
end

@testset "SerializableDocument: sd_array constructor" begin
    doc = sd_array()
    @test kind(doc) == DocArray
    @test is_array(doc)
    @test array_size(doc) == 0
end

@testset "SerializableDocument: sd_object constructor" begin
    doc = sd_object()
    @test kind(doc) == DocObject
    @test is_object(doc)
    @test object_size(doc) == 0
end

@testset "SerializableDocument: push_back!" begin
    arr = sd_array()
    push_back!(arr, SerializableDocument(Int64(1)))
    push_back!(arr, SerializableDocument(Int64(2)))
    push_back!(arr, SerializableDocument(Int64(3)))
    @test array_size(arr) == 3
    @test as_integer(array_at(arr, 1)) == Int64(1)
    @test as_integer(array_at(arr, 2)) == Int64(2)
    @test as_integer(array_at(arr, 3)) == Int64(3)
end

@testset "SerializableDocument: set!" begin
    obj = sd_object()
    set!(obj, "x", SerializableDocument(true))
    set!(obj, "y", SerializableDocument(Int64(99)))
    @test object_size(obj) == 2
    @test has_key(obj, "x")
    @test has_key(obj, "y")
    @test !has_key(obj, "z")
end

@testset "SerializableDocument: find" begin
    obj = sd_object()
    set!(obj, "name", SerializableDocument("vista"))
    ref = find(obj, "name")
    @test ref !== nothing
    @test as_string(ref) == "vista"
    @test find(obj, "missing") === nothing
end

@testset "SerializableDocument: object_key_at / object_value_at" begin
    obj = sd_object()
    set!(obj, "alpha", SerializableDocument(Int64(1)))
    n = object_size(obj)
    @test n == 1
    @test object_key_at(obj, 1) == "alpha"
    @test as_integer(object_value_at(obj, 1)) == Int64(1)
end

@testset "SerializableDocument: array_at bounds" begin
    arr = sd_array()
    @test_throws BoundsError array_at(arr, 1)
end

@testset "SerializableDocument: equality" begin
    a = SerializableDocument(Int64(42))
    b = SerializableDocument(Int64(42))
    c = SerializableDocument(Int64(99))
    @test a == b
    @test a != c
end

@testset "SerializableDocument: copy" begin
    original = sd_object()
    set!(original, "key", SerializableDocument("value"))
    cloned = copy(original)
    @test cloned == original
    set!(cloned, "extra", SerializableDocument(true))
    @test cloned != original
end

@testset "SerializableDocument: nested object" begin
    root = sd_object()
    inner = sd_object()
    set!(inner, "n", SerializableDocument(Int64(7)))
    set!(root, "child", inner)

    ref = find(root, "child")
    @test ref !== nothing
    @test is_object(ref)
    n_ref = find(ref, "n")
    @test n_ref !== nothing
    @test as_integer(n_ref) == Int64(7)
end

@testset "SerializableDocument: nested array of objects" begin
    arr = sd_array()
    for i = 1:3
        obj = sd_object()
        set!(obj, "i", SerializableDocument(Int64(i)))
        push_back!(arr, obj)
    end
    @test array_size(arr) == 3
    for i = 1:3
        elem = array_at(arr, i)
        @test is_object(elem)
        v = find(elem, "i")
        @test v !== nothing
        @test as_integer(v) == Int64(i)
    end
end
