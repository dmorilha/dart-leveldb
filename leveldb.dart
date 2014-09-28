library leveldb;

import 'dart-ext:leveldb';
import 'dart:math';

int getMajorVersion() native "getMajorVersion";
int getMinorVersion() native "getMinorVersion";

bool _delete(db, key, options) native "delete";
bool _put(db, key, value, options) native "put";
int _open(path, options) native "open";
string _get(db, key, options) native "get";
int _seek(db, key, options) native "seek";

string _iteratorKey(iterator) native "iteratorKey";
string _iteratorValue(iterator) native "iteratorValue";

class Options {
  bool create_if_missing;
  bool error_if_exists;
  bool paranoid_checks;
  int block_restart_interval;
  int block_size;
  int max_open_files;
  int write_buffer_size;
}

class ReadOptions { }
class WriteOptions { }
class WriteBatch { }

class Snapshot { }

class Range { }

class Record {
  final int handle;
  Record(this.handle);
  string get key => _iteratorKey(handle);
  string get value => _iteratorValue(handle);
}

class RecordIterator implements Iterator<Record> {
  Record current;
  RecordIterator(int handle) { current = new Record(handle); }
  bool moveFirst() => _iteratorNext(current);
  bool moveLast() => _iterator(current);
  bool moveNext() => _iteratorNext(current);
  bool movePrev() => _iterator(current);
}

class DB {
  final int _handle;
  final string _path;

  string get path => _path;

  static DB Open(string path, [ Options options ]) {
    return new DB._internal(_open(path, options), path);
  }

  DB._internal(this._handle, this._path);

  bool Put(string key, string value, [ WriteOptions options ]) {
    return _put(_handle, key, value, options);
  }

  string Get(string key, [ ReadOptions options ]) {
    return _get(_handle, key, options);
  }
  
  bool Delete(string key, [ WriteOptions options ]) {
    return _delete(_handle, key, options);
  }

  RecordIterator Seek(string key, [ ReadOptions options ]) {
    return new RecordIterator(_seek(_handle, key, options));
  }

  Snapshot GetSnapshot() { }
  void ReleaseSnapshot(Snapshot snapshot) { }
  bool GetProperty(Slice property, string) { }
  void GetApproximateSizes(Range range, int n, int sizes) { }
  void CompactRange(Slice begin, Slice end) { }
}

bool DestroyDB(string name, Options options) { }

bool RepairDB(string dbname, Options options) { }

void main() {
  final int major = getMajorVersion(),
      minor = getMinorVersion();

  string value;
  final int random = (new Random()).nextInt(1024);

  print("Hello There");
  print("leveldb version: $major.$minor");

  var db = DB.Open("/tmp/leveldb.db");

  db.Put("number", "$random");

  value = db.Get("number");
  print("number => $value");

  RecordIterator iterator = db.Seek("number");
  value = iterator.current.value;
  print("iterator => $value");

  db.Delete("number");

  value = db.Get("number");
  print("number => $value");


}
