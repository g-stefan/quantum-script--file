# API reference

## Script

Available after `Script.requireExtension("File")` (which also loads
`Buffer`).

### Global

| Symbol | Returns | Notes |
|--------|---------|-------|
| `File()` / `new File()` | File | a new, closed file object |
| `File.isFile(x)` | Boolean | `x` is a file object |

### Opening

| Method | Returns | Behavior |
|--------|---------|----------|
| `f.openReadOnly(file)` | Boolean | `"rb"`: must exist |
| `f.openWrite(file)` | Boolean | `"wb"`: create or truncate |
| `f.openReadAndWrite(file)` | Boolean | `"r+b"`: must exist, content kept |
| `f.openAppend(file)` | Boolean | `"ab"`: create if needed, writes always at the end |
| `f.openStdIn()` / `f.openStdOut()` / `f.openStdErr()` | `true` | process streams; `close()` only detaches |

Every `open*` closes the previous stream of the object first.

### Reading and writing

| Method | Returns | Behavior |
|--------|---------|----------|
| `f.read(size)` | String / `undefined` | up to `size` bytes (default 32768); `undefined` at end of file |
| `f.readLn(size)` | String / `undefined` | one line **with** its `"\r\n"` / `"\n"`; at most `size` bytes before the line end (default 32768); `undefined` at end of file |
| `f.write(str)` | Number | bytes written |
| `f.writeLn(str)` | Number | writes `str` + `"\r\n"`; bytes written including the line end |
| `f.readToBuffer(buffer, ln)` | Number | one read into `buffer` from index 0, `ln` (default 32768) clipped to `buffer.size`; sets `buffer.length`; `0` at end of file |
| `f.writeFromBuffer(buffer)` | Number | writes `buffer.length` bytes |
| `f.flush()` | `undefined` | flush buffered data |
| `f.close()` | `undefined` | close (or detach a standard stream); safe to repeat |

### Position

| Method | Returns | Behavior |
|--------|---------|----------|
| `f.seekFromBegin(pos)` | Boolean / `undefined` | absolute position; `undefined` for negative / `NaN` / infinite |
| `f.seek(delta)` | Boolean / `undefined` | relative, may be negative; `undefined` for `NaN` / infinite |
| `f.seekFromEnd(pos)` | Boolean / `undefined` | `pos` bytes **before** the end (`0` = end); `false` if `pos` > size (position then unspecified); `undefined` for negative / `NaN` / infinite |
| `f.seekTell()` | Number | current position; `18446744073709551616` when not open |

### Conversions

| Expression | Result |
|------------|--------|
| `typeof(f)` | `"File"` |
| `f instanceof File` | `true` |
| `"" + f` | `"File"` |
| `Convert.toBoolean(f)`, `if (f)` | `true` (open or not) |
| `f.x = 1` | throws `setPropertyBySymbol` |

### Errors

| Message | Cause |
|---------|-------|
| `invalid parameter` | a method called with a `this` that is not a file, or `readToBuffer` / `writeFromBuffer` without a `Buffer` |
| `setPropertyBySymbol` | assigning a property to a file object |
| `Unable to open "File"` | the extension (or `Buffer`) library was not found and no internal one is registered |

## C++

Namespace `XYO::QuantumScript::Extension::File`, umbrella header
`<XYO/QuantumScript.Extension/File.hpp>`.

### Library (`File/Library.hpp`)

| Symbol | Notes |
|--------|-------|
| `void registerInternalExtension(Executive *executive)` | register `"File"` as an internal extension |
| `void initExecutive(Executive *executive, void *extensionId)` | extension init, run by the engine |
| `extern "C" void quantumScriptExtension(Executive *, void *)` | DLL entry point (not in static builds) |

### `VariableFile` (`File/VariableFile.hpp`)

| Symbol | Notes |
|--------|-------|
| `XYO::System::File value` | public stream |
| `static Variable *newVariable()` | new, closed file object |
| `String getVariableType()` | `"File"` |
| `Variable *instancePrototype()` | `File.prototype` of the current thread |
| `bool toBoolean()` | `true` |
| `String toString()` | `"File"` |
| `void activeDestructor()` | closes the stream when the object is recycled |

### `FileContext` (`File/Context.hpp`)

| Symbol | Notes |
|--------|-------|
| `Symbol symbolFunctionFile` | symbol of the `File` global |
| `TPointerX<Prototype> prototypeFile` | `File.prototype` holder |
| `FileContext *getContext()` | per thread singleton |

### Metadata

| Symbol | Notes |
|--------|-------|
| `Version::version()`, `Version::build()`, `Version::versionWithBuild()`, `Version::datetime()` | from `version.json` |
| `Copyright::copyright()`, `Copyright::publisher()`, `Copyright::company()`, `Copyright::contact()` | |
| `License::license()`, `License::shortLicense()` | MIT text |

`Version`, `Copyright` and `License` exist in every XYO library: qualify them
(`Extension::File::Version::versionWithBuild()`).

### Build configuration

| Name | Meaning |
|------|---------|
| `quantum-script--file` | fabricare project, DLL / shared library; depends on `quantum-script`, `quantum-script--console`, `quantum-script--buffer` |
| `quantum-script--file.static` | fabricare project, static library, static CRT |
| `XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT` | export / import macro |
| `XYO_QUANTUMSCRIPT_EXTENSION_FILE_INTERNAL` | defined while building the DLL (from `QUANTUM_SCRIPT__FILE_INTERNAL`) |
| `XYO_QUANTUMSCRIPT_EXTENSION_FILE_LIBRARY` | static build: empty export macro, no DLL entry point |
