//
// Created by DaveRiedel on 22-8-2022.
//
#include <gtest/gtest.h>
#include <QString>
#include <utils/OutputConverters.h>
#include <models/commands/output.h>

/*!
 * \brief Test the com frequency outputs
 */
TEST(UtilsTests, com_to_format) {
    OutputConverters converters = OutputConverters();
    float rawCom = 123456;

    Output output = Output();
    output.setPrefix(900);
    output.setType(0);

    std::string received = converters.formatOutgoingFloat(rawCom, output);
    std::string expectedChar = "900 123456\n";
    EXPECT_EQ(received, expectedChar);
}
/*!
 * \brief Test the conversion of percentage based outputs
 */
TEST(UtilsTests, percentage_to_format) {
    OutputConverters converters = OutputConverters();
    float incValue = 0.8345;

    Output output = Output();
    output.setPrefix(536);
    output.setType(5);

    std::string received = converters.formatOutgoingFloat(incValue, output);
    std::string expectedChar = "536 83\n";
    EXPECT_EQ(received, expectedChar);
}
/*!
 * \brief Test the prefix padding to always be 4 characters
 */
TEST(UtilsTests, prefix_padded_to_4_chars) {
    OutputConverters converters = OutputConverters();
    float incValue = 0.0;

    Output output = Output();
    output.setPrefix(53);
    output.setType(0);
    std::string expectedChar = "53  0\n";
    std::string formated = converters.formatOutgoingFloat(incValue, output);
    EXPECT_EQ(expectedChar, formated);

    Output output3 = Output();
    output3.setPrefix(536);
    output3.setType(0);
    std::string expectedChar3 = "536 0\n";
    std::string formated3 = converters.formatOutgoingFloat(incValue, output3);
    EXPECT_EQ(expectedChar3, formated3);

    Output output4 = Output();
    output4.setPrefix(5367);
    output4.setType(0);
    std::string expectedChar4 = "53670\n";
    std::string formated4 = converters.formatOutgoingFloat(incValue, output4);
    EXPECT_EQ(expectedChar4, formated4);
}
