library leveldb;

import 'dart-ext:leveldb';
import 'dart:nativewrappers';

int getMajorVersion() native "getMajorVersion";
int getMinorVersion() native "getMinorVersion";

bool _delete(db, key, options) native "delete";
bool _put(db, key, value, options) native "put";
int _open(path, options) native "open";
string _get(db, key, options) native "get";
int _seek(db, key, options) native "seek";
int _seekToFirst(db, options) native "seekToFirst";
int _seekToLast(db, options) native "seekToLast";

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

class Iterator extends NativeFieldWrapperClass1 {
  string get key => _iteratorKey(this);
  string get value => _iteratorValue(this);
  string toString() => "$key => $value";
}

class RecordIterator implements Iterator<Record> {
  final Iterator current;
  bool _first;
  RecordIterator(this.current) { _first = true; }

  bool moveNext() {
    bool result = true;

    if ( ! _first) {
      result = _iteratorNext(current);

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

class DB extends NativeFieldWrapperClass1 {
  final string path;

  static DB Open(string path, [ Options options ]) {
    return _open(path, options);
  }

  DB._internal(this.path);

  bool Put(string key, string value, [ WriteOptions options ]) {
    return _put(this, key, value, options);
  }

  string Get(string key, [ ReadOptions options ]) {
    return _get(this, key, options);
  }

  bool Delete(string key, [ WriteOptions options ]) {
    return _delete(this, key, options);
  }

  RecordIterator Seek(string key, [ ReadOptions options ]) {
    return new RecordIterator(_seek(this, key, options));
  }

  RecordIterator SeekToFirst([ ReadOptions options ]) {
    return new RecordIterator(_seekToFirst(this, options));
  }

  Snapshot GetSnapshot() { }
  void ReleaseSnapshot(Snapshot snapshot) { }
  bool GetProperty(Slice property, string) { }
  void GetApproximateSizes(Range range, int n, int sizes) { }
  void CompactRange(Slice begin, Slice end) { }
}

bool DestroyDB(string name, [ Options options ]) { }
bool RepairDB(string name, [ Options options ]) { }
