# Quantum Script Extension File

Quantum Script extension
- A `File` type for streaming files from scripts: open for reading,
writing, read and write, or appending, and the process `stdin` / `stdout` /
`stderr`.
- Text I/O in chunks or line by line (`read`, `readLn`, `write`, `writeLn`),
byte I/O with `Buffer` (`readToBuffer`, `writeFromBuffer`).
- Random access with 64-bit positions (`seekFromBegin`, `seek`,
`seekFromEnd`, `seekTell`).
- Errors are return values (`false`, `undefined`), not exceptions.

```javascript
Script.requireExtension("File");

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

Built on `quantum-script` and `quantum-script--buffer`, part of the XYO C++ SDK.

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, load from a script, register in a C++ host, static builds, file names
- [File model](docs/file-model.md) - open modes, position, lines, standard streams, errors
- [Script API](docs/script-api.md) - every function: exact behavior, edge cases
- [Recipes](docs/recipes.md) - read lines, append, copy binary files, file size, tail, patch in place, stdin filters
- [C++ API](docs/cpp-api.md) - `VariableFile`, using `File` objects in native functions
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--file](.claude/skills/quantum-script--file/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
