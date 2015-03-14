#include <nan.h>
#include "UselessTask.hpp"

UselessTask::UselessTask() : NanAsyncWorker(NULL) {}

void UselessTask::Execute() {
    // Do nothing
    pause();
}

void UselessTask::HandleOKCallback() {
    NanScope();
    // Do nothing
}
