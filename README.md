![Clip from video for Kraftwerk's "The Man Machine"](https://media3.giphy.com/media/v1.Y2lkPTc5MGI3NjExamx5YTAyNXd6Z3M0NnJ2djhpNjBsbXY5cmF2MmtkaXZzbW16d2dtZSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/xLg9bOhJK2Z2w/giphy.gif)

# QSimpleStateMachine

[![Unit Tests](https://github.com/MrEricSir/QSimpleStateMachine/workflows/Test/badge.svg)](https://github.com/MrEricSir/QSimpleStateMachine/actions)
[![Coverage Report](https://img.shields.io/badge/coverage-report-blue.svg)](https://mrericsir.github.io/QSimpleStateMachine/coverage/)

QSimpleStateMachine provides a stupidly simple state machine for Qt applications with few dependencies.
Cross-platform for Windows, macOS, and Linux.

]

## Project Origins

I originally developed this for the [Fang newsreader](https://github.com/MrEricSir/Fang) when I needed
a state machine that worked with Qt and didn't have much of a learning curve. So I built it myself.

## Usage

Set up states as anything that will compile to a 0 or greater `int`. Set up callbacks for any possible
transition from one state to another, or use -1 as a catch-all.

Call `start()` to kick off your state machine with an initial state.



````cpp
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
````

You can also listen to the `stateChanged()` signal to be notified of state changes that way
which is often useful for logging and debugginges.

Code docuemntation [is available here.](https://mrericsir.github.io/QSimpleStateMachine/docs/qsimplestatemachine.html)

## How To Include

There are two option to include this in your project:

1. Copy and paste the header file
1. Git Submodule

Let's go over these in more detail.

### Copy and paste the header file

Grab the `QSimpleStateMachine.h` in this project and copy the file into your own. That's all there is to it!

Now this does come with one downside -- there's no easy way to update your copy of the file. Which brings us to:

### Git Submodule

In this example we'll add a submodule to an exiting `git` repo that contains your project. It will live in a
directory called `external/QSimpleStateMachine`.

In the root directory of your project:

````bash
git submodule add git@github.com:MrEricSir/QSimpleStateMachine.git external/QSimpleStateMachine
git add .gitmodules
git commit -m "Add QSimpleStateMachine submodule"
````

***Note:*** You can then update the header file just as you would with any other submodule. In addition you can 
use the unit tests in your project if you feel so inclined. Both of these are beyond the scope of this document.

Assuming you're using `CMake` to build your project, modify your CMakeLists.txt file

````cmake
# Add QSimpleStateMachine directory
add_subdirectory(external/QSimpleStateMachine)

# Add it to MyApp's link libraries
target_link_libraries(MyApp PRIVATE
  QSimpleStateMachine
  # ...etc.
)
````

With this approach, you can include the header in your C++ code with:

````cpp
include <QSimpleStateMachine/QSimpleStateMachine.h>
````

## Contributing

Found a bug? Want to propose an improvement? File [a ticket](https://github.com/MrEricSir/QSimpleStateMachine/issues)
and/or submit [a pull request](https://github.com/MrEricSir/QSimpleStateMachine/pulls).

Contributions are welcome!
