#include <QtTest>
#include "../../utils/OutputConverters.h"
// add necessary includes here
#include <utility>
#include <string>
class OutputTests : public QObject
{
    Q_OBJECT

public:
    OutputTests();
    ~OutputTests();

private slots:
    void test_coms_parsed_to_format();
};

OutputTests::OutputTests()
{
}

OutputTests::~OutputTests()
{

}

void OutputTests::test_coms_parsed_to_format()
{
    OutputConverters converters = OutputConverters();
    float rawCom = 123456;
    std::pair< int,std::string> received = converters.parseOutputString(rawCom, "201", 0);
    QString expectedChar = "201 123456";
    QString found = QString::fromStdString(received.second);
    QCOMPARE(found, expectedChar);
}

QTEST_APPLESS_MAIN(OutputTests)

#include "tst_outputtests.moc"
