#include <QTest>
#include "test_engine.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QCoreApplication app(argc, argv);
    int status = 0;
    {
        TestEngine testEngine;
        status |= QTest::qExec(&testEngine, argc, argv);
    }
    return status;
}
