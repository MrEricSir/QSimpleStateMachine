#include <QString>
#include <QTest>
#include <QSignalSpy>

#include "../include/QSimpleStateMachine/QSimpleStateMachine.h"

class TesQSimpleStateMachine : public QObject
{
    Q_OBJECT
    
public:
    
    enum MyStates {
        START,
        STATE_A,
        STATE_B,
        COMPLETE
    };
    
    TesQSimpleStateMachine();
    
signals:
    void done();
    
public slots:
    void onStateA();
    void onStateB();
    void onStateComplete();
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    
    
private:
    QSimpleStateMachine machine;
    int stateCounter;
};

TesQSimpleStateMachine::TesQSimpleStateMachine()
{
    stateCounter = 0;

    machine.addStateChange(START, STATE_A, [this]() { onStateA(); });
    machine.addStateChange(STATE_A, STATE_B, [this]() { onStateB(); });
    machine.addStateChange(-1, COMPLETE, [this]() { onStateComplete(); });
}

void TesQSimpleStateMachine::initTestCase()
{
}

void TesQSimpleStateMachine::cleanupTestCase()
{
}

void TesQSimpleStateMachine::onStateA()
{
    qDebug() << "Moved to state a.";
    stateCounter++;
}

void TesQSimpleStateMachine::onStateB()
{
    qDebug() << "Moved to state b.";
    stateCounter++;
}

void TesQSimpleStateMachine::onStateComplete()
{
    qDebug() << "Final state!";
    stateCounter++;
    
    emit done();
}

void TesQSimpleStateMachine::testCase1()
{
    QSignalSpy spy(this, &TesQSimpleStateMachine::done);
    
    machine.start(START);
    machine.setState(STATE_A);
    machine.setState(STATE_B);
    machine.setState(COMPLETE);
    
    // Wait for the signal!
    QCOMPARE(spy.count(), 1);
    
    QVERIFY2(stateCounter == 3, "Wrong state count!");
}

QTEST_APPLESS_MAIN(TesQSimpleStateMachine)

#include "tst_testqsimplestatemachine.moc"
