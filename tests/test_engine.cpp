#include "test_engine.h"
#include <QFile>
#include <QTextStream>

void TestEngine::init()
{
    QFile enFile(":/resources/rules/en-US.json");
    if (enFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&enFile);
        std::string content = in.readAll().toStdString();
        m_engine.loadRulesFromString(content);
        enFile.close();
    }
    m_engine.setLocale("en-US");
}

void TestEngine::cleanup()
{
}

void TestEngine::testResponse_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expectedResponse");

    QTest::newRow("greeting") << "Hello" << "How do you do. Please state your problem.";
    QTest::newRow("i feel") << "I feel" << "Tell me more about these feelings.";
}

void TestEngine::testResponse()
{
    QFETCH(QString, input);
    QFETCH(QString, expectedResponse);

    QString actualResponse = QString::fromStdString(m_engine.respond(input.toStdString()));

    QCOMPARE(actualResponse, expectedResponse);
}

void TestEngine::testReflection_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expectedReflection");

    QTest::newRow("i am") << "i am happy" << "you are happy";
    QTest::newRow("you are") << "you are not happy" << "i am not happy";
    QTest::newRow("i think") << "i think you are my friend" << "you think i am your friend";
    QTest::newRow("case insensitive") << "I AM happy" << "you are happy";
}

void TestEngine::testReflection()
{
    QFETCH(QString, input);
    QFETCH(QString, expectedReflection);

    // This is a bit of a hack, as we don't have direct access to the reflect method.
    // We'll rely on a rule that we know will reflect.
    std::string response = m_engine.respond(("Please reflect: " + input).toStdString());

    QVERIFY(QString::fromStdString(response).contains(expectedReflection, Qt::CaseInsensitive));
}
