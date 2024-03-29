#include <benchmark/benchmark.h>
#include <memory/buffer.h>

using namespace spruce;

static void bufferNullptrAllocFree(benchmark::State& state) {
	for (auto _ : state) { // NOLINT(clang-analyzer-deadcode.DeadStores)
		buffer<uint8> buf(nullptr);
		benchmark::DoNotOptimize(buf);
		buf.free();
	}
}
BENCHMARK(bufferNullptrAllocFree);

static void buffer1MBAllocFree(benchmark::State& state) {
	for (auto _ : state) { // NOLINT(clang-analyzer-deadcode.DeadStores)
		buffer<uint8> buf(1024 * 1024);
		benchmark::DoNotOptimize(buf);
		buf.free();
	}
}
BENCHMARK(buffer1MBAllocFree);

static void buffer1MBAllocRealloc2MBFree(benchmark::State& state) {
	for (auto _ : state) { // NOLINT(clang-analyzer-deadcode.DeadStores)
		buffer<uint8> buf(1024 * 1024);
		benchmark::DoNotOptimize(buf);
		buf.realloc(2 * 1024 * 1024);
		buf.free();
	}
}
BENCHMARK(buffer1MBAllocRealloc2MBFree);

static void bufferSqBracketOpAssign(benchmark::State& state) {
	buffer<uint8> buf(1);
	for (auto _ : state) { // NOLINT(clang-analyzer-deadcode.DeadStores)
		benchmark::DoNotOptimize(buf);
		buf[0] = 0;
	}
	buf.free();
}
BENCHMARK(bufferSqBracketOpAssign);
