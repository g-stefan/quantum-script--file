# Getting started

## 1. Build and install

The extension is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `quantum-script` (and everything
below it: `xyo-system`, `xyo-encoding`, ...), `quantum-script--console` and
`quantum-script--buffer` must be installed to the SDK first. From the
repository root:

```bash
fabricare make       # build into output/
fabricare test       # build and run test/test.01 (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

Two libraries are produced:

| Project                       | Kind                                | Use it when                                         |
|-------------------------------|-------------------------------------|-----------------------------------------------------|
| `quantum-script--file`        | DLL / shared library (`dll-or-lib`) | scripts run by `quantum-script`, or a host using the engine DLL |
| `quantum-script--file.static` | static library, static CRT          | self-contained hosts built with `quantum-script.static` |

After `fabricare install`, `quantum-script--file.dll` (Windows) /
`libquantum-script--file.so` (Linux) sits in the SDK `bin` folder next to
`quantum-script.exe` and `quantum-script--buffer.dll`, which is where
`Script.requireExtension("File")` finds it.

## 2. Use it from a script

```javascript
Script.requireExtension("Console");
Script.requireExtension("File");

var out = new File();
if (!out.openWrite("hello.txt")) {
	throw "cannot create hello.txt";
};
out.writeLn("Hello");
out.writeLn("World");
out.close();

var inp = new File();
inp.openReadOnly("hello.txt");
var line;
while (Script.isString(line = inp.readLn())) {
	Console.write("> " + line);   // line keeps its "\r\n"
};
inp.close();
```

Run it with:

```bash
quantum-script hello-file.js
```

`Script.requireExtension("File")` looks for an external
`quantum-script--file` library first (the file as named, then every include
path folder: next to the interpreter, next to the script), then for an
internal extension registered by the host. Loading twice does nothing. A
missing extension throws `Unable to open "File"`.

Loading `File` also runs `Script.requireExtension("Buffer")`, so the
`quantum-script--buffer` library must be available too, and the `Buffer`
global exists afterwards.

## 3. fabricare build scripts

`fabricare` registers `File` (and `Buffer`, `Shell`, ...) as internal
extensions, so build scripts can use it without any DLL:

```javascript
Script.requireExtension("File");

var header = new File();
if (header.openWrite("temp/Version.hpp")) {
	header.writeLn("// generated, do not edit");
	header.writeLn("#define APP_VERSION \"1.0.0\"");
	header.close();
};
```

`magnet` registers it as internal as well.

## 4. Register it in a C++ host

A host that embeds Quantum Script makes `File` available as an internal
extension by registering it, together with `Buffer`, in the init callback
(this is what `test/test.01.cpp` does):

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/Buffer.hpp>
#include <XYO/QuantumScript.Extension/File.hpp>

using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	Extension::Buffer::registerInternalExtension(executive);   // File requires Buffer
	Extension::File::registerInternalExtension(executive);
};

int main(int cmdN, char *cmdS[]) {
	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		if (!ExecutiveX::executeFile("main.js")) {
			printf("%s\n", (ExecutiveX::getError()).value());
			printf("%s", (ExecutiveX::getStackTrace()).value());
		};
		ExecutiveX::endProcessing();
	};
	return 0;
};
```

Registering only makes the extension *available*: scripts still call
`Script.requireExtension("File")`. With the DLL build of the engine an
external `quantum-script--file.dll` found on the include path wins over
the internal one for `requireExtension`; use
`Script.requireInternalExtension("File")` to force the internal one.

In the host's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--file"
	]
}
```

`quantum-script--file` brings `quantum-script`, `quantum-script--console`
and `quantum-script--buffer` with it.

## 5. Static builds

For a self-contained executable depend on the static variants and the static
CRT:

```json
{
	"name": "my-host.static",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script.static",
		"quantum-script--console.static",
		"quantum-script--buffer.static",
		"quantum-script--file.static"
	],
	"crt": "static"
}
```

`quantum-script--file.static` exports the define
`XYO_QUANTUMSCRIPT_EXTENSION_FILE_LIBRARY` to its consumers, which turns
`XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT` into nothing and leaves out the
`quantumScriptExtension` DLL entry point. A static host must register the
extension (and `Buffer`) with `registerInternalExtension` (section 4):
external DLLs cannot be loaded into a host that does not use the engine DLL.

## 6. File names

The name is passed to the C runtime `fopen` as given, after conversion to a
string (`openReadOnly(undefined)` opens a file named `undefined`).

- Relative names are relative to the **current directory** of the process,
  not to the script. Build them from `Script.getIncludedFile()` /
  `Shell.getFilePath(...)` when the file sits next to the script.
- Forward slashes work on Windows and Linux.
- On Windows, non-ASCII (UTF-8) names work in programs whose manifest sets
  the UTF-8 active code page, as `quantum-script.exe` does; a custom host
  needs the same manifest entry.
- The folder must exist: `openWrite("missing/x.txt")` returns `false`. Use
  `Shell.mkdirRecursivelyIfNotExists(path)` first.

## 7. Threads

Each thread that runs scripts has its own engine and its own `File`
prototype (`FileContext` is a per thread singleton), so every thread loads
the extension itself with `Script.requireExtension("File")`. A `File`
object belongs to the thread that created it and cannot be passed to
another thread (it is not cloneable); pass the file name and open it there.
