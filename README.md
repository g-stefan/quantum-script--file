# Quantum Script Extension File

```javascript
File();
File.isFile(x);
File.prototype.openReadOnly(file);
File.prototype.openWrite(file);
File.prototype.openReadAndWrite(file);
File.prototype.openAppend(file);
File.prototype.openStdIn();
File.prototype.openStdOut();
File.prototype.openStdErr();
File.prototype.read(size);
File.prototype.readLn(size);
File.prototype.write(str);
File.prototype.writeLn(str);
File.prototype.close();
File.prototype.flush();
File.prototype.seekFromBegin(pos);
File.prototype.seekFromEnd(pos);
File.prototype.seek(pos);
File.prototype.seekTell();
File.prototype.readToBuffer(buffer,ln);
File.prototype.writeFromBuffer(buffer);
```

## License

Copyright (c) 2016-2025 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
