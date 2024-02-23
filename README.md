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
* easy to integrate in existing project with custom "templates"


Still Annoying:

* we cannot use a plain C++ attribute `[[from_string]]` as libclang only exposes the attributes it knows about. But we can re-purpose [`clang::annotate`](https://clang.llvm.org/docs/AttributeReference.html#annotate).

## See also

* [serde-cpp](https://github.com/serde-cpp/serde-cpp) follows the same approach but uses an external parser, cppast, which does not have the limitation around unknown attributes. The generator is in C++ itself, it seems intended to be used as a standalone serialization library, not as a repurposeable building block to include in another project.
* open proposals for reflection in C++: [N4856](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/n4856.pdf) and [P1240R2](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p1240r2.pdf)
