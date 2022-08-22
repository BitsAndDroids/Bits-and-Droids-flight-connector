//
// Created by DaveRiedel on 22-8-2022.
//
#include <gtest/gtest.h>
#include <QString>
#include <utils/OutputConverters.h>

TEST(UtilsTests, com_to_format) {
    OutputConverters converters = OutputConverters();
    float rawCom = 123456;
    std::pair< int,std::string> received = converters.parseOutputString(rawCom, "201", 0);
    QString expectedChar = "201 123456";
    QString found = QString::fromStdString(received.second);
    EXPECT_EQ(found, expectedChar);
}
