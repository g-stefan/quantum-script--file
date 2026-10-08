# C++ API

For hosts that embed Quantum Script and for extensions that work with
`File` objects. Read the `quantum-script` repository's `docs/embedding.md`
and `docs/writing-extensions.md` first: native functions, `Variable` and
`TPointer` work the same way here. This extension is also the reference
example of "objects with methods: a custom Variable type" in
`writing-extensions.md`.

## Headers and namespace

```cpp
#include <XYO/QuantumScript.Extension/File.hpp>                 // Library.hpp: initExecutive, registerInternalExtension
#include <XYO/QuantumScript.Extension/File/VariableFile.hpp>    // the value type
#include <XYO/QuantumScript.Extension/File/Context.hpp>         // FileContext (rarely needed)

using namespace XYO::QuantumScript;
using Extension::File::VariableFile;
```

Namespace: `XYO::QuantumScript::Extension::File`. Export macro:
`XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT` (empty when
`XYO_QUANTUMSCRIPT_EXTENSION_FILE_LIBRARY` is defined, i.e. static builds).

Inside that namespace `File` is the namespace itself: name the stream class
`XYO::System::File` in full.

## Registering the extension

```cpp
void Extension::File::registerInternalExtension(Executive *executive);
void Extension::File::initExecutive(Executive *executive, void *extensionId);
```

- `registerInternalExtension` registers `"File"` as an internal extension;
  call it from the host's init callback, together with
  `Extension::Buffer::registerInternalExtension` (see
  [Getting started](getting-started.md#4-register-it-in-a-c-host)).
- `initExecutive` is the extension's init function, run by the engine when a
  script first requires `File` in a thread. It sets the extension name, info
  (license text) and version, creates the `File` global function and its
  prototype, runs `Script.requireExtension("Buffer")`, and registers the
  native methods. Do not call it directly.
- The DLL build also exports
  `extern "C" void quantumScriptExtension(Executive *, void *)`, which
  forwards to `initExecutive`; it is what `Script.requireExtension` looks up
  in `quantum-script--file.dll`.

## `VariableFile`

```cpp
class VariableFile : public Variable {
	public:
		XYO::System::File value;          // the stream: openRead, openWrite, read, write, seek..., close

		VariableFile();
		static Variable *newVariable();   // a new, closed file object

		void activeDestructor();          // value.close() when the object is recycled
		String getVariableType();         // "File"
		Variable *instancePrototype();    // File.prototype of the current thread
		bool toBoolean();                 // always true
		String toString();                // "File"
};
```

`value` is a public `XYO::System::File` (see the `xyo-system` repository,
`docs/`): `openRead`, `openWrite`, `openReadAndWrite`, `openAppend`,
`openStdIn` / `openStdOut` / `openStdErr`, `operator bool` (is open),
`read(void *, size_t)`, `write(const void *, size_t)`, `seekFromBegin`,
`seek`, `seekFromEnd`, `seekTell`, `flush`, `close`. It implements `IRead`,
`IWrite` and `ISeek`, so the `XYO::System::Stream` helpers (`read`,
`readLn`, `write`, `writeLn`, `readToBuffer`, `writeFromBuffer`,
`LineRead`) accept it.

`VariableFile` objects come from an active memory pool
(`TMemoryPoolActive`): when one is recycled, `activeDestructor` closes the
stream. Always create them with `VariableFile::newVariable()` and hold them
in `TPointer<Variable>`.

`VariableFile` does not override `clone`, so it cannot be copied to another
thread, and does not accept own properties (`setPropertyBySymbol` throws).

## Taking a File argument

Check the type with `TIsType<VariableFile>`, then use the stream directly:

```cpp
static TPointer<Variable> writeHeader(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	TPointerX<Variable> &fileV(arguments->index(0));
	if (!TIsType<VariableFile>(fileV)) {
		throw(Error("invalid parameter"));
	};
	XYO::System::File &file = ((VariableFile *)fileV.value())->value;
	if (!file) {
		return VariableBoolean::newVariable(false);   // not open
	};
	return VariableBoolean::newVariable(XYO::System::Stream::writeLn(file, "HEADER") == 8);
};
```

## Returning an open File

```cpp
static TPointer<Variable> openLog(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	TPointer<Variable> retV(VariableFile::newVariable());
	if (((VariableFile *)retV.value())->value.openAppend((arguments->index(0))->toString())) {
		return retV;
	};
	return Context::getValueUndefined();
};
```

The script gets an object with all `File.prototype` methods, as long as
`File` was loaded in that thread (see below).

## Depending on File from another extension

1. In `fabricare.json`, add `"quantum-script--file"` to the DLL project's
   dependencies and `"quantum-script--file.static"` to the static one.
2. Include `<XYO/QuantumScript.Extension/File/VariableFile.hpp>`.
3. In your `initExecutive`, load the extension before anything that may
   create or receive file objects, so `File.prototype` exists in that
   thread:

```cpp
void initExecutive(Executive *executive, void *extensionId) {
	executive->setExtensionName(extensionId, "Log");
	// ...
	executive->compileStringX("Script.requireExtension(\"File\");");
	executive->compileStringX("var Log={};");
	executive->setFunction2("Log.open(name)", openLog);
	executive->setFunction2("Log.writeHeader(file)", writeHeader);
};
```

A `VariableFile` created in a thread where `File` was never loaded has no
prototype: reading a method on it dereferences an empty
`FileContext::prototypeFile`.

## `FileContext`

```cpp
class FileContext : public Object {
	public:
		Symbol symbolFunctionFile;            // the "File" global
		TPointerX<Prototype> prototypeFile;   // File.prototype holder
};
FileContext *getContext();                    // TSingleton<FileContext>: one per thread
```

Filled by `initExecutive`, cleared by the extension's delete-context hook
when the engine ends. Use `getContext()->prototypeFile->prototype` if you
need `File.prototype` from C++.

## Notes for maintainers

- Native methods live in `File/Library.cpp` as
  `static TPointer<Variable> name(VariableFunction *, Variable *this_, VariableArray *arguments)`,
  check `TIsType<VariableFile>(this_)` first and throw
  `Error("invalid parameter")` otherwise, then call `value` or the
  `XYO::System::Stream` helpers. They are registered in `initExecutive` with
  `executive->setFunction2("File.prototype.name(args)", name)`; static
  functions as `"File.name(args)"`.
- Number arguments are validated with `isnan` / `isinf` / `signbit`; invalid
  values return `undefined` (or `0` / an empty buffer for `readToBuffer`)
  instead of throwing.
- `readToBuffer` is registered with the signature `readToBuffer(buffer)` but
  also reads a second argument, `ln`: the signature only names parameters,
  every passed argument reaches the native function.
- When adding a method, update `README.md`, `docs/script-api.md`,
  `docs/reference.md` and the skill in `.claude/skills/quantum-script--file`.
