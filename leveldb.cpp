#include <iostream>

#include <leveldb/db.h>

#include "include/dart_api.h"

static Dart_Handle library;

Dart_NativeFunction ResolveName(Dart_Handle name, int argc, bool * auto_setup_scope);

DART_EXPORT Dart_Handle leveldb_Init(Dart_Handle parent_library) {
	if (Dart_IsError(parent_library)) {
		return parent_library;
	}

	Dart_Handle result_code = Dart_SetNativeResolver(parent_library, ResolveName, NULL);

	if (Dart_IsError(result_code)) {
		return result_code;
	}

	library = Dart_NewPersistentHandle(parent_library);

	return Dart_Null();
}

Dart_Handle HandleError(Dart_Handle handle) {
	if (Dart_IsError(handle)) {
		Dart_PropagateError(handle);
	}

	return handle;
}

struct DartScope {
	~DartScope() {
		Dart_ExitScope();
	}

	DartScope(void) {
		Dart_EnterScope();
	}
};

template < int N >
struct DartArguments : DartArguments< N - 1> {
	Dart_Handle handle;

	typedef DartArguments< N - 1 > Prev;
	typedef DartArguments< N > Self;

	DartArguments(const Dart_NativeArguments & arguments) :
		Prev(arguments),
		handle(Dart_GetNativeArgument(arguments, N - 1)) { }
};

template < >
struct DartArguments< 0 > : DartScope {
	DartArguments(Dart_NativeArguments a) :
		arguments(a) { }

	operator Dart_NativeArguments (void) const {
		return arguments;
	}

	Dart_NativeArguments arguments;
};

template < int N >
Dart_Handle get(const DartArguments< N + 1 > & a) {
	return a.handle;
}

namespace leveldb {
	void GetMajorVersion(Dart_NativeArguments arguments) {
		DartScope scope;
		Dart_Handle result = HandleError(Dart_NewInteger(leveldb::kMajorVersion));
		Dart_SetReturnValue(arguments, result);
	}

	void GetMinorVersion(Dart_NativeArguments arguments) {
		DartScope scope;
		Dart_Handle result = HandleError(Dart_NewInteger(leveldb::kMinorVersion));
		Dart_SetReturnValue(arguments, result);
	}

	void Open(Dart_NativeArguments a) {
		DartArguments< 3 > arguments(a);

		const static Dart_Handle optionsClass = Dart_GetClass(library, Dart_NewStringFromCString("Options"));
		const static Dart_Handle statusClass = Dart_GetClass(library, Dart_NewStringFromCString("Status"));

		const char * path;
		Dart_StringToCString(get< 0 >(arguments), &path);

		DB::DB * db;
		Status status;

		Options options;
		options.create_if_missing = true;

		status = DB::Open(options, path, &db);

		Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(reinterpret_cast< int64_t >(db))));
	}

	void Put(Dart_NativeArguments a) {
		DartArguments< 4 > arguments(a);

		DB::DB * db;

		{
			int64_t address;
			Dart_IntegerToInt64(get< 0 >(arguments), &address);
			db = reinterpret_cast< DB::DB * >(address);
		}

		Status status;

		if (db) {
			const char * key,
				* value;

			Dart_StringToCString(get< 1 >(arguments), &key);
			Dart_StringToCString(get< 2 >(arguments), &value);

			status = db->Put(WriteOptions(), key, value);
		}

		Dart_SetReturnValue(arguments, HandleError(Dart_NewBoolean(status.ok())));
	}

	void Get(Dart_NativeArguments a) {
		DartArguments< 3 > arguments(a);

		const char * key;
		Dart_StringToCString(get< 1 >(arguments), &key);

		DB::DB * db;

		{
			int64_t address;
			Dart_IntegerToInt64(get< 0 >(arguments), &address);
			db = reinterpret_cast< DB::DB * >(address);
		}

		Status status;
		Dart_Handle result = Dart_Null();

		if (db) {
			std::string value;
			status = db->Get(ReadOptions(), key, &value);
			result = HandleError(Dart_NewStringFromCString(value.c_str()));
		}

		Dart_SetReturnValue(arguments, result);
	}

	void Delete(Dart_NativeArguments a) {
		DartArguments< 3 > arguments(a);

		const char * key;
		Dart_StringToCString(get< 1 >(arguments), &key);

		DB::DB * db;

		{
			int64_t address;
			Dart_IntegerToInt64(get< 0 >(arguments), &address);
			db = reinterpret_cast< DB::DB * >(address);
		}

		Status status;

		if (db) {
			status = db->Delete(WriteOptions(), key);
		}

		Dart_SetReturnValue(arguments, HandleError(Dart_NewBoolean(status.ok())));
	}

	void First(Dart_NativeArguments a) {
		DartArguments< 2 > arguments(a);

		DB::DB * db;

		{
			int64_t address;
			Dart_IntegerToInt64(get< 0 >(arguments), &address);
			db = reinterpret_cast< DB::DB * >(address);
		}

		Status status;
		Iterator * iterator = NULL;

		if (db) {
			iterator = db->NewIterator(ReadOptions());

			if (iterator) {
				iterator->SeekToFirst();
			}
		}

		Dart_SetReturnValue(arguments,
			HandleError(Dart_NewInteger(reinterpret_cast< int64_t >(iterator))));
	}

	void Last(Dart_NativeArguments a) {
		DartArguments< 2 > arguments(a);

		DB::DB * db;

		{
			int64_t address;
			Dart_IntegerToInt64(get< 0 >(arguments), &address);
			db = reinterpret_cast< DB::DB * >(address);
		}

		Status status;
		Iterator * iterator = NULL;

		if (db) {
			iterator = db->NewIterator(ReadOptions());

			if (iterator) {
				iterator->SeekToLast();
			}
		}

		Dart_SetReturnValue(arguments,
			HandleError(Dart_NewInteger(reinterpret_cast< int64_t >(iterator))));
	}

	void Seek(Dart_NativeArguments a) {
		DartArguments< 3 > arguments(a);

		DB::DB * db;

		{
			int64_t address;
			Dart_IntegerToInt64(get< 0 >(arguments), &address);
			db = reinterpret_cast< DB::DB * >(address);
		}

		Status status;
		Iterator * iterator = NULL;

		if (db) {
			iterator = db->NewIterator(ReadOptions());

			if (iterator) {
				const char * key;
				Dart_StringToCString(get< 1 >(arguments), &key);
				iterator->Seek(key);
			}
		}

		Dart_SetReturnValue(arguments, HandleError(Dart_NewInteger(reinterpret_cast< int64_t >(iterator))));
	}

	namespace iterator {
		void Key(Dart_NativeArguments a) {
			DartArguments< 1 > arguments(a);
			Iterator * iterator = NULL;

			{
				int64_t address;
				Dart_IntegerToInt64(get< 0 >(arguments), &address);
				iterator = reinterpret_cast< Iterator * >(address);
			}

			if (iterator && iterator->Valid()) {
				const std::string key = iterator->key().ToString();
				Dart_SetReturnValue(arguments,
					HandleError(Dart_NewStringFromCString(key.c_str())));
			}
		}

		void Next(Dart_NativeArguments a) {
			DartArguments< 1 > arguments(a);
			Iterator * iterator = NULL;

			{
				int64_t address;
				Dart_IntegerToInt64(get< 0 >(arguments), &address);
				iterator = reinterpret_cast< Iterator * >(address);
			}

			bool valid = false;

			if (iterator && iterator->Valid()) {
				iterator->Next();
				valid = iterator->Valid();
			}

			Dart_SetReturnValue(arguments, Dart_NewBoolean(valid));
		}

		void Prev(Dart_NativeArguments a) {
			DartArguments< 1 > arguments(a);
			Iterator * iterator = NULL;

			{
				int64_t address;
				Dart_IntegerToInt64(get< 0 >(arguments), &address);
				iterator = reinterpret_cast< Iterator * >(address);
			}

			bool valid = false;

			if (iterator && iterator->Valid()) {
				iterator->Prev();
				valid = iterator->Valid();
			}

			Dart_SetReturnValue(arguments, Dart_NewBoolean(valid));
		}

		void Valid(Dart_NativeArguments a) {
			DartArguments< 1 > arguments(a);
			Iterator * iterator = NULL;

			{
				int64_t address;
				Dart_IntegerToInt64(get< 0 >(arguments), &address);
				iterator = reinterpret_cast< Iterator * >(address);
			}

			if (iterator) {
				const bool valid = iterator->Valid();
				Dart_SetReturnValue(arguments,
					HandleError(Dart_NewBoolean(valid)));
			}
		}

		void Value(Dart_NativeArguments a) {
			DartArguments< 1 > arguments(a);
			Iterator * iterator = NULL;

			{
				int64_t address;
				Dart_IntegerToInt64(get< 0 >(arguments), &address);
				iterator = reinterpret_cast< Iterator * >(address);
			}

			if (iterator && iterator->Valid()) {
				const std::string value = iterator->value().ToString();
				Dart_SetReturnValue(arguments,
					HandleError(Dart_NewStringFromCString(value.c_str())));
			}
		}
	}
}

struct FunctionLookup {
	const char * name;
	Dart_NativeFunction function;
};

FunctionLookup function_list[] = {
	{ "delete", leveldb::Delete },
	{ "first", leveldb::First},
	{ "get", leveldb::Get },
	{ "getMajorVersion", leveldb::GetMajorVersion },
	{ "getMinorVersion", leveldb::GetMinorVersion },
	{ "iteratorKey", leveldb::iterator::Key },
	{ "iteratorNext", leveldb::iterator::Next },
	{ "iteratorPrev", leveldb::iterator::Prev },
	{ "iteratorValid", leveldb::iterator::Valid },
	{ "iteratorValue", leveldb::iterator::Value },
	{ "last", leveldb::Last },
	{ "open", leveldb::Open },
	{ "put", leveldb::Put },
	{ "seek", leveldb::Seek },
	{ NULL, NULL },
};

Dart_NativeFunction ResolveName(Dart_Handle name, int argc, bool * auto_setup_scope) {
	if ( ! Dart_IsString(name)) {
		return NULL;
	}

	Dart_NativeFunction result = NULL;

	if (auto_setup_scope == NULL) {
		return NULL;
	}

	DartScope scope;
	const char * cname;
	HandleError(Dart_StringToCString(name, &cname));

	for (int i = 0; function_list[i].name != NULL; ++i) {
		if (strcmp(function_list[i].name, cname) == 0) {
			*auto_setup_scope = true;
			result = function_list[i].function;
			break;
		}
	}

	return result;
}
