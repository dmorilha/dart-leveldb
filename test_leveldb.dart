library leveldb;

import 'dart:math';

import "leveldb.dart";

void main() {
  final int major = getMajorVersion(),
      minor = getMinorVersion();

  string value;

  print("leveldb version: $major.$minor");

  var db = DB.Open("/tmp/leveldb.db");

  for (int i = 0; i < 100; ++i) {
    final int random = (new Random()).nextInt(1024);
    db.Put("number $i", "$random");
  }

  for (var record in new Records(db.SeekToFirst())) {
    print(record);
  }
}
