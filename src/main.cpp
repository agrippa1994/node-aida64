#include "aida64.h"
#include <nan.h>

class ReadWorker : public Nan::AsyncWorker {
	std::string data_;
	std::string error_;
	bool hasError_ = false;

public:
	ReadWorker(Nan::Callback *callback) : Nan::AsyncWorker(callback) {
	}

	void Execute() {
		try {
			data_ = aida64::API().read();
		}
		catch (const std::exception& err) {
			hasError_ = true;
			error_ = err.what();
		}
	}

	void HandleOKCallback() {
		Nan::HandleScope scope;

		v8::Local<v8::Value> args[] = {
			!hasError_ ? Nan::Null() : Nan::New(error_.c_str()).ToLocalChecked(),
			hasError_ ? Nan::Null() : Nan::New(data_.c_str()).ToLocalChecked()
		};

		callback->Call(2, args);
	}
};

NAN_METHOD(readAIDA64) {
	if (info.Length() < 1)
		return Nan::ThrowError("First argument must be a callback!");

	if (!info[0]->IsFunction())
		return Nan::ThrowError("First argument must be a callback!");

	Nan::AsyncQueueWorker(new ReadWorker(new Nan::Callback(info[0].As<v8::Function>())));
}

NAN_METHOD(readAIDA64Sync) {
	try {
		return info.GetReturnValue().Set(Nan::New(aida64::API().read().c_str()).ToLocalChecked());
	}
	catch (const std::exception& err) {
		Nan::ThrowError(err.what());
	}
}

NAN_MODULE_INIT(bindMethods) {
	Nan::Set(target, Nan::New("readAIDA64").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(readAIDA64)).ToLocalChecked());
	Nan::Set(target, Nan::New("readAIDA64Sync").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(readAIDA64Sync)).ToLocalChecked());
}

NODE_MODULE(node_aida64, bindMethods);
