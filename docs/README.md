# Quantum Script Extension File — Documentation

`quantum-script--file` is the **file stream extension of Quantum Script**.
Loaded with `Script.requireExtension("File")`, it adds a `File` type to
scripts: an object that opens a file (or the process `stdin` / `stdout` /
`stderr`), reads and writes it as text or bytes, line by line or in
chunks, moves the file position, and closes it.

It is a thin wrapper over `XYO::System::File` (C `FILE *` streams opened in
binary mode), so the behavior is the one of `fopen` / `fread` / `fwrite` /
`fseek`, with 64-bit offsets for files over 2 GB.

- **One object, many opens.** `var f = new File();` then `openReadOnly`,
  `openWrite`, `openReadAndWrite`, `openAppend`, `openStdIn`, `openStdOut`
  or `openStdErr`. Opening closes whatever the object had open before.
- **Text.** `read(size)` / `write(str)` move raw bytes as strings;
  `readLn(size)` returns one line **with its line end**; `writeLn(str)`
  appends `"\r\n"`.
- **Bytes.** `readToBuffer(buffer, ln)` / `writeFromBuffer(buffer)` work
  with the `Buffer` type of `quantum-script--buffer`, loaded automatically.
- **Position.** `seekFromBegin(pos)`, `seek(delta)` (relative, may be
  negative), `seekFromEnd(pos)`, `seekTell()`.
- **No exceptions for I/O errors.** Failures are return values: `false`
  from `open*` and `seek*`, `undefined` from `read` / `readLn` at the end of
  the file, `0` bytes written.

```
scripts: fabricare build scripts, quantum-script .js, ...
quantum-script--file      <-- this extension: the File type
quantum-script--buffer    (Buffer, for readToBuffer / writeFromBuffer)
quantum-script            (Executive, Variable, Context)
xyo-system                (XYO::System::File, Stream::read / readLn / write / writeLn)
xyo-encoding, xyo-multithreading, xyo-data-structures, xyo-managed-memory, xyo-platform
```

## Why it exists

| Need | What `File` gives |
|------|-------------------|
| Process a file bigger than memory, or line by line | `readLn()` / `read(size)` in a loop, `undefined` at the end |
| Write a file progressively (logs, generated sources, reports) | `openWrite` / `openAppend`, then `write` / `writeLn` |
| Binary I/O in chunks | `readToBuffer` / `writeFromBuffer` with one reused `Buffer` |
| Random access: headers, trailers, patching in place | `seekFromBegin`, `seek`, `seekTell`, `openReadAndWrite` |
| Read input from a pipe, write to `stdout` / `stderr` | `openStdIn`, `openStdOut`, `openStdErr` |

For a whole file at once, `Shell.fileGetContents(file)` /
`Shell.filePutContents(file, text)` (and their `Buffer` variants) from
`quantum-script--shell` are shorter; use `File` when you need streaming,
appending, positions or the standard streams.

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the extension | `Script.requireExtension("File");` | also loads `Buffer` |
| New file object | `var f = new File();` or `File()` | not open yet |
| Read an existing file | `f.openReadOnly(name)` | `false` if it cannot be opened |
| Create / truncate | `f.openWrite(name)` | the folder must exist |
| Update in place | `f.openReadAndWrite(name)` | the file must exist, position 0 |
| Add to the end | `f.openAppend(name)` | creates the file; every write goes to the end |
| Standard streams | `f.openStdIn()`, `f.openStdOut()`, `f.openStdErr()` | `close()` does not close the process stream |
| Read text | `f.read(size)`, `f.readLn(size)` | default size 32768; `undefined` at end of file |
| Write text | `f.write(str)`, `f.writeLn(str)` | return bytes written; `writeLn` adds `"\r\n"` |
| Read / write bytes | `f.readToBuffer(buffer, ln)`, `f.writeFromBuffer(buffer)` | `Buffer` from `quantum-script--buffer` |
| Move | `f.seekFromBegin(pos)`, `f.seek(delta)`, `f.seekFromEnd(pos)` | `true` / `false`; `undefined` for invalid numbers |
| Where am I / file size | `f.seekTell()`; `f.seekFromEnd(0); f.seekTell()` | |
| Finish | `f.flush()`, `f.close()` | closing twice is harmless |
| Type test | `File.isFile(x)`, `typeof(x) == "File"`, `x instanceof File` | |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, load the extension from a script, fabricare scripts, register it in a C++ host, static builds, threads |
| [File model](file-model.md) | Open modes, the file position, lines and line ends, text and bytes, standard streams, closing, how errors are reported |
| [Script API](script-api.md) | Every function: arguments, exact behavior, edge cases, return values |
| [Recipes](recipes.md) | Read lines, write and append, copy binary files, file size, tail, patch in place, stdin filters |
| [C++ API](cpp-api.md) | `VariableFile`, `FileContext`, `registerInternalExtension`, using `File` objects from native code |
| [API reference](reference.md) | Every script and C++ symbol on one page |

Quantum Script itself (the language, `Script.requireExtension`, embedding,
writing extensions) is documented in the `quantum-script` repository,
`docs/`; the `Buffer` type in the `quantum-script--buffer` repository,
`docs/`; the underlying `XYO::System::File` in the `xyo-system` repository,
`docs/`.

## Source map

```
source/XYO/QuantumScript.Extension/File.hpp            umbrella header, include this from C++
source/XYO/QuantumScript.Extension/File.Amalgam.cpp    the whole extension in one translation unit
source/XYO/QuantumScript.Extension/File/
    Dependency.hpp                                     <XYO/QuantumScript.hpp>, export macro
    Library[.hpp/.cpp]                                 initExecutive, registerInternalExtension,
                                                       File() and every native function
    Context.hpp                                        FileContext: the File prototype per thread
    VariableFile[.hpp/.cpp]                            the script value type (wraps XYO::System::File)
    Copyright / License / Version                      library metadata
test/test.01.cpp                                       C++ host registering Console, Buffer and File as internal
test/test.01.js                                        loads Console and File
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--file/`](../.claude/skills/quantum-script--file/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that use `File`
(fabricare scripts, Quantum Script tools, other extensions).
