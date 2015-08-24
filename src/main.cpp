#include "aida64.h"
#include <node.h>
#include <v8.h>

struct ReadAIDA64DataAsync {
    v8::Persistent<v8::Function> callback;

    bool hasError = false;
    std::string errorData;
    std::string aidaData;
};

void readAIDA64Worker(uv_work_t *req) {
    auto data = (ReadAIDA64DataAsync *)req->data;

    try {
        data->aidaData = aida64::API().read();
    } catch(const std::exception& err) {
        data->hasError = true;
        data->errorData = err.what();
    }
}

void readAIDA64After(uv_work_t *req, int) {
    v8::HandleScope scope;

    auto data = (ReadAIDA64DataAsync *)req->data;

    v8::Handle<v8::Value> args[] = {
        data->hasError ? v8::String::New(data->errorData.c_str()) : v8::Null(),
        data->hasError ? v8::Null() : v8::String::New(data->aidaData.c_str())
    };

    v8::TryCatch catcher;
    data->callback->Call(v8::Context::GetCurrent()->Global(), 2, args);
    if(catcher.HasCaught()) {
        // Ignore exceptions in the callback
    }

    data->callback.Dispose();
    delete data;
    delete req;
}

v8::Handle<v8::Value> readAIDA64(const v8::Arguments& args) {
    v8::HandleScope scope;

    if(args.Length() < 1) {
        v8::ThrowException(v8::Exception::Error(v8::String::New("Wrong parameter count!")));
        return scope.Close(v8::Undefined());
    }

    if(!args[0]->IsFunction()) {
        v8::ThrowException(v8::Exception::Error(v8::String::New("First parameter is not a function!")));
        return scope.Close(v8::Undefined());
    }

    auto callback = v8::Local<v8::Function>::Cast(args[0]);

    auto data = new ReadAIDA64DataAsync;
    auto worker = new uv_work_t;

    data->callback = v8::Persistent<v8::Function>::New(callback);
    worker->data = data;

    uv_queue_work(uv_default_loop(), worker, readAIDA64Worker, readAIDA64After);

    return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> readAIDA64Sync(const v8::Arguments& args) {
    v8::HandleScope scope;

    try {
        return scope.Close(v8::String::New(aida64::API().read().c_str()));
    } catch(const std::exception& err) {
        v8::ThrowException(v8::Exception::Error(v8::String::New(err.what())));
    }

    return scope.Close(v8::Undefined());
}

void bindMethods(v8::Handle<v8::Object> target) {
    NODE_SET_METHOD(target, "readAIDA64", readAIDA64);
    NODE_SET_METHOD(target, "readAIDA64Sync", readAIDA64Sync);
}

NODE_MODULE(node_aida64, bindMethods);
