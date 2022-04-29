using BenchmarkDotNet.Running;

// BenchmarkSwitcher.FromAssembly(typeof(Program).Assembly).Run(args, new DebugInProcessConfig());

BenchmarkSwitcher.FromAssembly(typeof(Program).Assembly).Run(args);
