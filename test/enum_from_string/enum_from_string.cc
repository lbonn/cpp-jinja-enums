#include <iostream>
#include <gtest/gtest.h>

#include "enum_from_string.hh"
#include "enum_from_string_to_string.hh"

TEST(EnumFromString, Simple) {
    {
        std::stringstream ss;
        ss << ipc::Message::PING;
        EXPECT_EQ(ss.str(), "PING");
    }

    {
        ipc::Message msg;
        std::stringstream ss("PONG");
        ss >> msg;
        EXPECT_EQ(msg, ipc::Message::PONG);
    }
}

TEST(EnumFromString, Fields) {
    for (const auto &f : enum_fields<ipc::Message>) {
        std::stringstream ss;
        ss << f.second;
        EXPECT_EQ(ss.str(), f.first);
    }
}
