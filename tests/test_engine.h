#ifndef TEST_ENGINE_H
#define TEST_ENGINE_H

#include <QObject>
#include <QTest>
#include "../src/core/rogerian/Engine.h"

class TestEngine : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testResponse_data();
    void testResponse();

    void testReflection_data();
    void testReflection();

private:
    deep_thonk::Engine m_engine;
};

#endif // TEST_ENGINE_H
