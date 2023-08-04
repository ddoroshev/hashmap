# HashMap Implementation in C

This repository provides a hashmap implementation in the C programming language. The hashmap uses a dynamic array for storage and handles collisions using open addressing with quadratic probing.

## How it Works

The hashmap calculates the hash of a key string using a `hash` function. It uses this hash and the length of the underlying storage array to determine the index where an item should be placed.

If a collision occurs (two keys produce the same hash), the hashmap uses quadratic probing to find an available slot in the array.

When the load factor of the hashmap (the number of stored items divided by the total array size) reaches a certain threshold, the hashmap will automatically resize to ensure enough available space.

The hashmap supports basic operations like setting a value, getting a value, and deleting an item.

## Limitations and Possible Improvements

While this implementation serves as a basic hashmap in C, there are some limitations and potential areas for improvement:

- The current hash function used is quite simple and might not produce a uniform distribution for all key sets.
- The hashmap only supports string keys and integer values. It could be extended to support generic data types.
- The hashmap does not physically remove deleted items until a resize operation is performed, which could potentially waste some memory space.

## Getting Started

### Prerequisites

- Docker (optional, for testing and debugging)
- gcc
- lcov and genhtml (optional, for coverage report)

### Compiling

Run `make` to compile the main application.

```bash
make
```

The compiled binary can be found under the `bin/` directory.

## Usage

To use the hashmap, include `hashmap.h` in your file.

```c
#include "hashmap.h"
```

### Basic Operations

Initialize a hashmap:

```c
hashmap *hm = hashmap_init();
```

Set a value:

```c
int ret = hashmap_set(hm, "key", 1);
```

Get a value:

```c
hashmap_item *item = hashmap_get(hm, "key");
```

Delete a value:

```c
int ret = hashmap_delete(hm, "key");
```

Free a hashmap:

```c
hashmap_free(hm);
```

## Testing

Run the tests with the following command:

```bash
make test
```

To generate a coverage report, run:

```bash
make cov
```

The report can be found in the `cov-report` directory.

## Benchmarking

To run benchmarks:

```bash
make benchmark
```

## Debugging

Use the following commands for debugging:

```bash
make playground
```

Or, if you have Docker installed, you can debug using the provided Dockerfiles:

```bash
make docker-playground-debug
```

## Contributing

We welcome contributions! Please open an issue if you have any suggestions, or a pull request if you have made some improvements.
