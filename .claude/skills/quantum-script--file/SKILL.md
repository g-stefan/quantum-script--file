---
name: quantum-script--file
description: >-
  How to use the Quantum Script File extension (quantum-script--file), the
  file stream type loaded with Script.requireExtension("File") (which also
  loads Buffer): new File() / File(), File.isFile, openReadOnly ("rb"),
  openWrite ("wb", truncates), openReadAndWrite ("r+b", must exist),
  openAppend ("ab", writes always at the end), openStdIn / openStdOut /
  openStdErr; read(size) and readLn(size) (default 32768, undefined at end of
  file, readLn keeps the "\r\n" / "\n" line end), write / writeLn (adds
  "\r\n"), readToBuffer(buffer, ln) / writeFromBuffer(buffer), flush, close;
  seekFromBegin, seek (relative, may be negative), seekFromEnd (pos bytes
  before the end), seekTell; file size, tail, copy, patch in place, stdin
  filters; the C++ side (VariableFile, its XYO::System::File value,
  FileContext, registerInternalExtension). Use when writing or reviewing
  Quantum Script or fabricare .js code that opens, reads or writes files or
  the standard streams with File, C++ code that includes
  <XYO/QuantumScript.Extension/File.hpp>, a fabricare.json depending on
  "quantum-script--file", or when working inside the quantum-script--file
  repository.
---

# quantum-script--file

File stream extension of Quantum Script (see the `quantum-script` skill for
the language and its differences from JavaScript, the
`quantum-script--buffer` skill for `Buffer`, and the `xyo-system` skill for
the underlying `XYO::System::File`; their rules apply). Purpose: **stream
files from scripts** — read line by line or in chunks, write and append
progressively, random access with positions, and the process
`stdin` / `stdout` / `stderr` — where `Shell.fileGetContents` /
`filePutContents` (whole file at once) are not enough.

Full documentation: `docs/` in the quantum-script--file repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script--file\docs` on this machine):
README, getting-started, **file-model** (modes, position, lines, standard
streams, errors), **script-api** (exact behavior of every method, edge
cases), **recipes** (lines, append, copy, size, tail, patch, filters),
cpp-api, reference. Read the matching page when you need more than this
summary. When in doubt read
`source/XYO/QuantumScript.Extension/File/Library.cpp` (~400 lines) and
`xyo-system` `source/XYO/System/File.cpp` / `Stream.cpp`.

## Script API

```javascript
Script.requireExtension("File");        // also loads Buffer; fabricare and magnet register it as internal

var f = new File();                     // closed; File() is the same
File.isFile(f);                         // true; typeof(f) == "File"; f instanceof File

f.openReadOnly(name);                   // "rb"   must exist                       -> true / false
f.openWrite(name);                      // "wb"   create or TRUNCATE, folder must exist
f.openReadAndWrite(name);               // "r+b"  must exist, keeps content, position 0
f.openAppend(name);                     // "ab"   create if needed, EVERY write goes to the end
f.openStdIn(); f.openStdOut(); f.openStdErr();   // always true; close() only detaches

f.read(size);                           // up to size bytes (default 32768) as String; undefined at EOF
f.readLn(size);                         // one line WITH "\r\n" / "\n"; undefined at EOF
f.write(str);                           // bytes written (str via toString; Buffer -> its bytes)
f.writeLn(str);                         // str + "\r\n" (every platform); returns bytes incl. the 2
f.readToBuffer(buffer, ln);             // one read from index 0, ln default 32768 clipped to size; sets length; 0 at EOF
f.writeFromBuffer(buffer);              // writes buffer.length bytes
f.flush(); f.close();                   // close is safe to repeat

f.seekFromBegin(pos);                   // true / false; undefined if pos < 0 / NaN / Infinity
f.seek(delta);                          // RELATIVE, delta may be negative
f.seekFromEnd(pos);                     // pos bytes BEFORE the end: 0 = end; pos > size -> false, position unspecified
f.seekTell();                           // position; 18446744073709551616 when not open
```

## Hard rules

1. **Check `open*`**: they return `false`, they never throw. A closed object
   reads `undefined`, writes `0`, seeks `false`. Every `open*` closes what
   the object had open first.
2. **End of file is `undefined`**, and `readLn` keeps the line end, so an
   empty line is `"\r\n"` / `"\n"`, not `""`. Loop with
   `while (Script.isString(line = f.readLn())) { ... }` and never use
   `line == ""` as the end test. Strip the line end with `line.trim()`.
3. **`read()` is not "read all"**: it reads at most 32768 bytes. Loop until
   `undefined`, or use `Shell.fileGetContents(name)`.
4. **`readLn(size)` splits long lines**: more than `size` bytes before the
   line end come back in pieces without a line end.
5. **`writeLn` writes `"\r\n"` on Linux too.** For `"\n"` files use
   `write(str + "\n")`.
6. **Files are binary, standard streams are not.** Files never translate
   line ends; on Windows `stdout` / `stderr` are text mode, so `"\n"`
   becomes `"\r\n"` and `writeLn` gives `"\r\r\n"`: write `str + "\n"` to
   standard streams.
7. **`seekFromEnd(n)` is `n` bytes before the end.** Last `n` bytes:
   `f.seekFromEnd(n); f.read(n);` (`false` if the file is shorter than
   `n`, and the position is then unspecified — MSVC leaves it at the end;
   clip `n` to the size). Size: `f.seekFromEnd(0); f.seekTell()`. The
   behavior comes from `XYO::System::File::seekFromEnd`; an `xyo-system`
   older than its fix moved `n` bytes *past* the end; `seekFromEnd(0)` works
   with both.
8. **`openAppend` ignores the position** for writes; to modify bytes use
   `openReadAndWrite` (which does not create the file).
9. **Read/write switching** in `openReadAndWrite`: put a `seek*` (e.g.
   `seek(0)`) or `flush()` between a write and a read, and a `seek*` between
   a read and a write.
10. **Never read a write-only object** (`openWrite` / `openAppend` /
    `openStdOut`): after a write the result is undefined (on Windows it
    returned a stray byte and corrupted the file).
11. **Close what you write.** Garbage collection closes the stream, but only
    `close()` / `flush()` guarantee the data is written at a known time.
12. **Names go to `fopen` as is**: relative to the process current
    directory (not the script), the folder must exist
    (`Shell.mkdirRecursivelyIfNotExists`), UTF-8 names on Windows need a
    UTF-8 code page manifest (`quantum-script.exe` has it).
13. **Strings are bytes**: `read(size)` counts bytes and may split a UTF-8
    character; no encoding conversion. For binary data prefer
    `readToBuffer` / `writeFromBuffer` with one reused `Buffer`.
14. **No own properties** (`f.name = x` throws `setPropertyBySymbol`); methods
    can be added to `File.prototype`. `if (f)` is always `true`. Methods on
    a non-File `this`, or buffer methods without a `Buffer`, throw
    `invalid parameter`.
15. **One engine per thread**: each thread requires `File` itself; a `File`
    object cannot be passed to another thread — pass the name.

## Patterns

```javascript
var line;                                                         // lines
while (Script.isString(line = f.readLn())) { use(line.trim()); };

var chunk = Buffer(65536);                                        // binary copy
while (src.readToBuffer(chunk, chunk.size) > 0) { dst.writeFromBuffer(chunk); };

var log = new File();                                             // append
if (log.openAppend("app.log")) { log.writeLn(msg); log.close(); };

f.openReadAndWrite("data.bin"); f.seekFromBegin(16);              // patch
f.writeFromBuffer(Buffer.fromHex("cafe")); f.close();

var o = new File(); o.openStdOut(); o.write(text + "\n"); o.flush();   // stdout
```

## C++

```cpp
#include <XYO/QuantumScript.Extension/File.hpp>
#include <XYO/QuantumScript.Extension/File/VariableFile.hpp>
using namespace XYO::QuantumScript;
using Extension::File::VariableFile;

Extension::Buffer::registerInternalExtension(executive);   // host init callback: File requires Buffer
Extension::File::registerInternalExtension(executive);     // scripts still requireExtension("File")

// argument
if (!TIsType<VariableFile>(arguments->index(0))) { throw(Error("invalid parameter")); };
XYO::System::File &file = ((VariableFile *)(arguments->index(0)).value())->value;   // bool(file) = is open
XYO::System::Stream::writeLn(file, "text");                // read / readLn / write / readToBuffer helpers

// return
TPointer<Variable> r(VariableFile::newVariable());          // closed
((VariableFile *)r.value())->value.openRead(name);
```

- Another extension using `File` objects: depend on `"quantum-script--file"`
  / `"quantum-script--file.static"` in `fabricare.json`, and in its
  `initExecutive` run
  `executive->compileStringX("Script.requireExtension(\"File\");");` before
  anything creates file objects (otherwise the per thread `File.prototype`
  is missing and method lookups crash).
- Inside `namespace XYO::QuantumScript::Extension::File`, `File` names the
  namespace: write `XYO::System::File`.
- Static build: `quantum-script--file.static` exports
  `XYO_QUANTUMSCRIPT_EXTENSION_FILE_LIBRARY` (empty export macro, no
  `quantumScriptExtension` entry point); register `Buffer` and `File` as
  internal.

## Working in this repository

- Build: `fabricare make`, `fabricare test` (runs `test/test.01`, which
  registers Console, Buffer and File as internal extensions and runs
  `test/test.01.js`; run `make` first), `fabricare install` (see the
  `fabricare` skill). `quantum-script`, `quantum-script--console` and
  `quantum-script--buffer` must be installed first.
- Native methods live in `File/Library.cpp` as
  `static TPointer<Variable> name(VariableFunction *, Variable *this_, VariableArray *arguments)`,
  check `TIsType<VariableFile>(this_)` first, and are registered in
  `initExecutive` with `executive->setFunction2("File.prototype.name(args)", name)`.
  Static functions: `"File.name(args)"`. Invalid numbers return `undefined`
  rather than throw.
- New methods: update `README.md`, `docs/script-api.md`, `docs/reference.md`
  and this skill.
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX header: MIT for `source/` and `docs/`,
  Unlicense for `test/` and `.claude/` (see `.reuse/dep5`).
