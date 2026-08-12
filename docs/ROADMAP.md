# Roadmap

## v0.1.0 Project Foundation

* [x] Initialize project structure
* [x] Configure CMake
* [x] Integrate vcpkg
* [x] Configure Clang/LLVM toolchain
* [x] Add clang-format
* [x] Add clang-tidy
* [x] Configure sanitizers
* [x] Configure code coverage
* [x] Configure GitHub Actions

---

## v0.2.0 Core

* [x] Implement `Error`
* [ ] Implement `Result<T>` using `std::expected`
* [x] Implement version information
* [ ] Design public API
* [ ] Design memory allocator strategy (e.g., Arena/Tape)

---

## v0.3.0 Stage 1: Structural Scan

* [ ] Implement SWAR Backend for structural scanning
* [ ] Implement SIMD Backend (e.g., AVX2/NEON) for structural scanning
* [ ] Identify structural characters
* [ ] Identify string boundaries
* [ ] UTF-8 validation
* [ ] Produce structural information (Tape)

---

## v0.4.0 DOM (Document Object Model)

* [ ] Implement `Value`
* [ ] Implement `Array`
* [ ] Implement `Object`
* [ ] Implement `Document`

---

## v0.5.0 Stage 2: Structural Parsing

* [ ] Build DOM based on structural information
* [ ] Fast path string parsing (escaping/unescaping)
* [ ] Fast path number parsing (integers and floats)
* [ ] Parse literals (`true`, `false`, `null`)
* [ ] Improve parser diagnostics

---

## v0.6.0 Testing

* [ ] Unit tests
* [ ] Integration tests
* [ ] Fuzz testing

---

## v0.7.0 Benchmark

* [ ] Integrate Google Benchmark
* [ ] Add structural scan benchmarks
* [ ] Add parsing benchmarks
* [ ] Add DOM benchmarks
* [ ] Benchmark against real-world datasets

---

## v0.8.0 Benchmark Reporter

* [ ] Save benchmark history
* [ ] Compare benchmark results
* [ ] Detect performance regression
* [ ] Generate Markdown report
* [ ] Generate charts

---

## v0.9.0 Performance

* [ ] Reduce memory allocations
* [ ] Optimize string parsing
* [ ] Optimize number parsing
* [ ] Optimize UTF-8 validation

---

## v0.10.0 Documentation

* [ ] API documentation
* [ ] Architecture guide
* [ ] Benchmark guide
* [ ] Examples

---

## v1.0.0 Stable Release

- [ ] Public API stabilization
- [ ] Complete test coverage
- [ ] Performance baseline established
- [ ] Documentation completed
- [ ] Cross-platform validation
- [ ] First stable release

---

## Future

* [ ] Serializer
* [ ] Add SIMD backend (Additional architectures)
* [ ] On-Demand API
* [ ] SAX parser
* [ ] JSON Pointer (RFC 6901)
* [ ] JSON Patch (RFC 6902)
* [ ] Custom allocator
* [ ] C++ Modules