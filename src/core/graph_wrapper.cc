#include "graph_wrapper.h"
#include "graph.h"
#include "tensorflow/c/c_api.h"
#include "operation_wrapper.h"

Nan::Persistent<v8::FunctionTemplate> GraphWrapper::constructor;

GraphWrapper::GraphWrapper() { m_graph = new Graph(); }
GraphWrapper::~GraphWrapper() {}

void GraphWrapper::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  //Class
  v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(GraphWrapper::New);
  ctor->InstanceTemplate()->SetInternalFieldCount(1);
  ctor->SetClassName(Nan::New("Graph").ToLocalChecked());

  // Prototype
  Nan::SetPrototypeMethod(ctor, "Placeholder", Placeholder);
  Nan::SetPrototypeMethod(ctor, "ScalarConst", ScalarConst);
  Nan::SetPrototypeMethod(ctor, "Add", Add);
  Nan::SetPrototypeMethod(ctor, "Run", Run);

  constructor.Reset(ctor);
  exports->Set(Nan::New("Graph").ToLocalChecked(), ctor->GetFunction());
}

void GraphWrapper::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Nan::HandleScope scope;

  GraphWrapper *instance = new GraphWrapper();
  instance->Wrap(info.Holder());
  info.GetReturnValue().Set(info.Holder());
}

void GraphWrapper::Placeholder(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  GraphWrapper* obj = ObjectWrap::Unwrap<GraphWrapper>(info.Holder());
  TF_Operation* result = obj->m_graph->Placeholder();
  
  v8::Local<v8::Object> n = Nan::New(OperationWrapper::constructor)->GetFunction()->NewInstance();
  OperationWrapper *i = new OperationWrapper(result);
  i->DoWrap(n);
  info.GetReturnValue().Set(n);
}

void GraphWrapper::ScalarConst(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  GraphWrapper* obj = ObjectWrap::Unwrap<GraphWrapper>(info.Holder());
  int arg0 = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
  TF_Operation* result = obj->m_graph->ScalarConst(arg0);
  v8::Local<v8::Object> n = Nan::New(OperationWrapper::constructor)->GetFunction()->NewInstance();
  OperationWrapper *i = new OperationWrapper(result);
  i->DoWrap(n);
  info.GetReturnValue().Set(n);
}

void GraphWrapper::Add(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  GraphWrapper* obj = ObjectWrap::Unwrap<GraphWrapper>(info.Holder());
  TF_Operation* arg0 = Nan::ObjectWrap::Unwrap<OperationWrapper>(info[0]->ToObject())->m_operation;
  TF_Operation* arg1 = Nan::ObjectWrap::Unwrap<OperationWrapper>(info[1]->ToObject())->m_operation;

  TF_Operation* result = obj->m_graph->Add(arg0, arg1);
  v8::Local<v8::Object> n = Nan::New(OperationWrapper::constructor)->GetFunction()->NewInstance();
  OperationWrapper *i = new OperationWrapper(result);
  i->DoWrap(n);
  info.GetReturnValue().Set(n);
}

void GraphWrapper::Run(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  GraphWrapper* obj = ObjectWrap::Unwrap<GraphWrapper>(info.Holder());
  int arg0 = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
  info.GetReturnValue().Set(Nan::New(obj->m_graph->Run(arg0)));
}
