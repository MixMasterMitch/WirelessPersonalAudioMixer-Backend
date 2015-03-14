#ifndef USELESS_TASK_H_
#define USELESS_TASK_H_

#include <nan.h>
#include <signal.h>
#include <iostream>

using namespace std;

/*
    The worker pauses execution and does not finish. This is useful for keeping the main node event loop alive without
    having to join one of the other threads.
*/
class UselessTask : public NanAsyncWorker {
 public:
  UselessTask();
  void Execute();
  void HandleOKCallback();
};

#endif
