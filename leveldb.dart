library leveldb;

import 'dart-ext:leveldb';

int getMajorVersion() native "getMajorVersion";
int getMinorVersion() native "getMinorVersion";

bool _delete(db, key, options) native "delete";
bool _put(db, key, value, options) native "put";
int _open(path, options) native "open";
string _get(db, key, options) native "get";
int _seek(db, key, options) native "seek";
int _first(db, options) native "first";
int _last(db, options) native "last";

bool _iteratorValid(iterator) native "iteratorValid";
bool _iteratorNext(iterator) native "iteratorNext";
bool _iteratorPrev(iterator) native "iteratorPrev";
string _iteratorKey(iterator) native "iteratorKey";
string _iteratorValue(iterator) native "iteratorValue";

class LevelDBException extends Exception { }

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
  string toString() => "$key => $value";
}

class RecordIterator implements Iterator<Record> {
  final Record current;
  bool _first;
  RecordIterator(this.current) { _first = true; }

  bool moveNext() {
    bool result = true;

    if ( ! _first) {
      result = _iteratorNext(current.handle);

    } else {
      _first = false;
    }

    return result;
  }
}

class Records implements Iterable<Record> {
  final RecordIterator iterator;
  Records(this.iterator);
}

class DB {
  final int db;
  final string path;

  static DB Open(string path, [ Options options ]) {
    return new DB._internal(_open(path, options), path);
  }

  DB._internal(this.db, this.path);

  bool Put(string key, string value, [ WriteOptions options ]) {
    return _put(db, key, value, options);
  }

  string Get(string key, [ ReadOptions options ]) {
    return _get(db, key, options);
  }
  
  bool Delete(string key, [ WriteOptions options ]) {
    return _delete(db, key, options);
  }

  RecordIterator Seek(string key, [ ReadOptions options ]) {
    return new RecordIterator(new Record(_seek(db, key, options)));
  }

  RecordIterator SeekToFirst([ ReadOptions options ]) {
    return new RecordIterator(new Record(_first(db, options)));
  }

  RecordIterator SeekToLast([ ReadOptions options ]) {
    return new RecordIterator(new Record(_last(db, options)));
  }

  Snapshot GetSnapshot() { }
  void ReleaseSnapshot(Snapshot snapshot) { }
  bool GetProperty(Slice property, string) { }
  void GetApproximateSizes(Range range, int n, int sizes) { }
  void CompactRange(Slice begin, Slice end) { }
}

bool DestroyDB(string name, [ Options options ]) { }
bool RepairDB(string name, [ Options options ]) { }
