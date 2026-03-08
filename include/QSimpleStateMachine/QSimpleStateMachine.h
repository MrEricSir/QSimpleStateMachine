// MIT License

// Copyright (c) 2026 Eric Gregory <mrericsir@gmail.com>

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef QSIMPLESTATEMACHINE_H
#define QSIMPLESTATEMACHINE_H

#include <QDebug>
#include <QObject>
#include <QMap>
#include <QPair>
#include <functional>

// When running QDoc we have to ignore Q_OBJECT and signals: to generate header-only docs.
#ifdef Q_QDOC
#define Q_OBJECT
#define signals public
#endif

/*!
    \module QSimpleStateMachine
    \title QSimpleStateMachine C++ Classes
    \brief Header-only library for a stupidly simple state machine.
*/

/*!
    \class QSimpleStateMachine
    \inmodule QSimpleStateMachine

    \brief This class provides a stupidly simple state machine for Qt applications with few
    dependencies. Cross-platform for Windows, macOS, and Linux.

    This library consists of a header only.

    // Set up your states as enums, like this:

    enum MyStates {

      START,

      STATE_A,

      STATE_B,

      COMPLETE

    };

    // Transitions are handled via callbacks.

    QSimpleStateMachine machine;

    machine.addStateChange(START, STATE_A, [this]() { startToStateA(); });

    machine.addStateChange(START, STATE_B, [this]() { startToStateB(); });

    machine.addStateChange(STATE_A, STATE_B, [this]() { stateAToStateB(); });

    // ... etc.

    // To switch from any state to a specific state, use -1

    machine.addStateChange(-1, COMPLETE, [this]() { onComplete(); });

    // Set the start state, then call setState() as needed.

    machine.start(START);

    // ...

    machine.setState(STATE_B);


    That's it!
 */
class QSimpleStateMachine : public QObject
{
    Q_OBJECT
    
public:
    /*!
        \fn explicit inline QSimpleStateMachine::QSimpleStateMachine(QObject *parent = nullptr)

        Constructs a QSimpleStateMachine object. Optionally, the \a parent is the owner of the object.
     */
    explicit inline QSimpleStateMachine(QObject *parent = nullptr) :
        QObject(parent),
        currentState(-1)
    {
    }

    /*!
        \fn inline void start(int initialState)

        Starts the state machine with the provided \a initialState. Does not trigger any callbacks.
     */
    inline void start(int initialState)
    {
        currentState = initialState;
    }

    /*!
        \fn inline void QSimpleStateMachine::setState(int state)

        Causes a state change, assuming \a state != getState().  If there is no path
        from the previous state to this state, a critical error will be logged.
     */
    inline void setState(int state)
    {
        if (currentState == state) {
            // Nothing to do.
            return;
        }

        // Make the switch.
        int oldState = currentState;
        currentState = state;

        // Find the callback.
        QPair<int, int> pair(oldState, currentState);
        if (!stateMap.contains(pair)) {
            // Default to -1 (all) state.
            pair = QPair<int, int>(-1, currentState);
            if (!stateMap.contains(pair)) {
                qCritical() << "SimpleStateMachine: State transition from" << oldState
                            << "to" << currentState << "not defined";
                return;
            }
        }

        // Emit the signal.
        emit stateChanged(oldState, currentState);

        // Run the callback.
        const std::function<void()>& callback = stateMap[pair];
        if (callback) {
            callback();
        } else {
            qCritical() << "SimpleStateMachine: Callback for transition" << oldState
                        << "->" << currentState << "is null";
        }
    }
    
    /*!
        \fn inline int QSimpleStateMachine::getState()

        Returns the \c current state.
     */
    inline int getState() { return currentState; }

    /*!
        \fn inline void QSimpleStateMachine::addStateChange(int from, int to, std::function<void()> callback)

        Sets a callback to be executed when the state changes \a from one state \a to
        another. The \a callback can be any std::function that matches the parameter type.
     */
    inline void addStateChange(int from, int to, std::function<void()> callback)
    {
        stateMap.insert(QPair<int, int>(from, to), callback);
    }

signals:
    /**
        \fn void QSimpleStateMachine::stateChanged(int from, int to)

        Signals a state change \a from one state \a to another. Useful for debug logging.
     */
    void stateChanged(int from, int to);

private:
    // The state.
    int currentState;

    // Maps a (from, to) state pair to a callback function.
    QMap<QPair<int, int>, std::function<void()>> stateMap;
};

#endif // QSIMPLESTATEMACHINE_H
