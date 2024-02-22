# C++ reflection with clang, Python and Jinja (demo)

Templates are one the distinguishing feature of C++. Feared and (sometimes?) loved, they are are a great tool for generic programming.

Still, they fall short on other types of meta programming and something which behaves like a [Python enum](https://docs.python.org/3/library/enum.html) usually involves some error-prone boilerplate. Or you use the C pre-processor
and re-invent your own small language, that is how [Better Enums](https://github.com/aantron/better-enums) solves this.

Meanwhile, Rust programmers have a macro system where they can manipulate the abstract tree of program instead of dumb strings.

A famous example is [serde](https://serde.rs) and its derive macro:

```rust
#[derive(Serialize, Deserialize)]
struct Point {
    x: i32,
    y: i32,
}
```

Here, the structure is defined in native rust, in contrast with the usual pre-processor solutions.

*But*, we also have a lot of overpowered tools like libclang and... Python?

## Concept

It uses [libclang binding in python](https://libclang.readthedocs.io/en/latest/) to get an AST, extract information from select tree nodes and then feed this to a Jinja template to generate C++ code!

It looks like:

```c++
#include "message_generated.h"

enum class [[clang::annotate("from_string")]] Message {
     PING,
     PONG,
};

int main() {
    std::cout << Message::PING << "\n";  // "PING"
}
```

Advantages:

* reflection!
* type declaration is a normal C++ enum in the code
* writing (jinja) templated code is almost like writing regular C++ code, no dark pre-processor magic required

## Other attempts

[serde-cpp](https://github.com/serde-cpp/serde-cpp) is using an external parser cppast and the generator is in C++ itself. That looks promising but it is more difficult to re-purpose
