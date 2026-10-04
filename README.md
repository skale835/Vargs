# Vargs

A small C++ class for storing and manipulating command-line arguments. I am aware that there are many of these. This one is designed to be easy.
Its initial purpose was to eliminate the 
`std::vector<std::string>::iterator `
verbage and thereby improve readability. Later, functionality was added to provide convenience. This will continue as long as it is used.

## Installation

Copy `Vargs.hpp` into your project directory and include it:
```cpp
#include "Vargs.hpp"
```
No separate source file or library is required.

## Use

Declare a `Vargs` object from the `argc` and `argv` supplied to `main()`:

```cpp
int main(int argc, char* argv[]) {
    Vargs vargs(argc, argv);

    // ...
}
```

`Vargs` stores the command-line arguments beginning with `argv[1]`. The called utility that normally would constitute the `argv[0]` term is discarded, so vargs.arg(0) is the first argument.

An empty `Vargs` object may also be declared:

```cpp
Vargs vargs;
```

Note: Vargs generally relies on std::vector's robustness. It passes through errors from std::vector when doing so makes sense.

See `Vargs.hpp` for the available methods and usage documentation.

## Test function

`test_Vargs.cpp` provides a test program for the class.

The test program is provided to assist you if you modify Vargs for your needs. It checks all functions and edge cases.

## License

Vargs is released under the MIT License. See `LICENSE` for the full license text.

Copyright (c) 2026 Sameer Kale
