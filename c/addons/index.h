#ifndef NAN_ADDONS_H
#define NAN_ADDONS_H

#include <nan.h>

#define NAN_CONSTRUCTOR(c) Nan::Persistent<v8::FunctionTemplate> c
#define NAN_NEW(n) void n(const Nan::FunctionCallbackInfo<v8::Value>& info)

#endif // NAN_ADDONS_H
