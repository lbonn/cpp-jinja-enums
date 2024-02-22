namespace ipc {

enum class [[clang::annotate("from_string")]] Message {
    PING,
    PONG,
};
}

enum class [[clang::annotate("from_string")]] EnumExample {
    A,
    B = 1,
    C,
    D,
    E,
    F,
};
