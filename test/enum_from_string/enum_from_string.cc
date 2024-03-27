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

    EXPECT_EQ(to_string(ipc::Message::PONG), "PONG");
    EXPECT_EQ(ipc::Message::PONG, from_string<ipc::Message>("PONG"));
}

TEST(EnumFromString, Fields) {
    for (const auto &f : enum_spec<ipc::Message>::members) {
        EXPECT_EQ(f.first, to_string(f.second));
    }
}

TEST(EnumFromString, StdFormat) {
    auto r = std::format("{}", ipc::Message::PONG);
    EXPECT_EQ(r, "PONG");
}
