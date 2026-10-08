# File model

## A File object

`File()` (or `new File()`, the same thing) returns a new, **closed** file
object. An object holds at most one open stream at a time:

- every `open*` function **closes the current stream first**, then opens the
  new one; on failure the object stays closed;
- `close()` releases the stream; the object can be opened again;
- when the object is garbage collected the stream is closed automatically,
  but data still buffered is only guaranteed on disk after `close()` or
  `flush()`: always close files you wrote.

On a closed object nothing fails loudly: `read` / `readLn` return
`undefined`, `write` returns `0`, `seek*` return `false`, `seekTell()` returns
`18446744073709551616` (the C `-1` error value seen as an unsigned 64-bit
number). Check the result of `open*`.

## Open modes

All files are opened in **binary mode**: bytes are read and written
unchanged, on Windows too (no `"\r\n"` ↔ `"\n"` translation).

| Function | C mode | File must exist | Truncates | Read | Write | Start position |
|----------|--------|-----------------|-----------|------|-------|----------------|
| `openReadOnly(name)` | `"rb"` | yes | no | yes | no | 0 |
| `openWrite(name)` | `"wb"` | no, created | **yes** | no | yes | 0 |
| `openReadAndWrite(name)` | `"r+b"` | yes | no | yes | yes | 0 |
| `openAppend(name)` | `"ab"` | no, created | no | no | yes, **always at the end** | end |
| `openStdIn()` | process `stdin` | — | — | yes | no | — |
| `openStdOut()` | process `stdout` | — | — | no | yes | — |
| `openStdErr()` | process `stderr` | — | — | no | yes | — |

Each returns `true` on success, `false` otherwise (missing file for read
modes, missing folder, no permission, a directory name, ...). The functions
do not say why; use `Shell.fileExists(name)` /
`Shell.directoryExists(path)` to tell the cases apart.

- `openAppend`: seeking is allowed but **every write still goes to the end
  of the file** (C append mode). To change existing bytes use
  `openReadAndWrite`.
- `openReadAndWrite` does not create the file. To create-or-update, test
  with `Shell.fileExists` and call `openWrite` the first time.
- Writing through a read-only object does nothing: `write` returns `0`.
- **Do not read a write-only object** (`openWrite`, `openAppend`,
  `openStdOut`). Before any write `read` returns `undefined`, but after a
  write the C runtime behavior is undefined: on Windows a read was seen to
  return a stray byte and add it to the file.

## The file position

Reads and writes start at the current position and move it forward by the
bytes transferred.

| Function | Moves to | Argument |
|----------|----------|----------|
| `seekFromBegin(pos)` | `pos` | `>= 0` |
| `seek(delta)` | current position `+ delta` | **may be negative** |
| `seekFromEnd(pos)` | `pos` bytes **before** the end of file | `>= 0`: `0` is the end, the file size is the beginning |
| `seekTell()` | — | returns the current position |

- The last `n` bytes: `f.seekFromEnd(n); f.read(n);`.
- File size: `f.seekFromEnd(0); var size = f.seekTell();`.
- `seekFromEnd(pos)` with `pos` larger than the file returns `false`; the
  position is then unspecified (MSVC: the end of the file), seek again.
- Seeking past the end is allowed with `seekFromBegin` / `seek`; a write
  there extends the file and the gap reads as zero bytes.
- Fractions are truncated. `NaN`, `±Infinity`, and negative values for
  `seekFromBegin` / `seekFromEnd` return `undefined` without moving.
- Offsets are 64-bit: files over 2 GB work (positions are exact up to
  2^53, the integer limit of a script number).

### Switching between reading and writing

With `openReadAndWrite`, the C runtime requires a `seek*` or `flush()`
between a write and a following read, and a `seek*` between a read and a
following write (`seek(0)` is enough). Without it the result is undefined.

```javascript
f.openReadAndWrite("data.bin");
f.write("HEAD");
f.seek(0);              // required before reading
var rest = f.read(16);
f.seek(0);              // required before writing again
f.write("TAIL");
f.close();
```

## Text, bytes and strings

Quantum Script strings are byte strings (UTF-8 by convention, not
validated), so `read` / `write` move bytes unchanged in both directions:

- `read(size)` returns up to `size` **bytes** as a string; a UTF-8 character
  may be split between two reads. Zero bytes are kept.
- `write(str)` writes the bytes of `str` (`str` converted with `toString`
  first: `write(12)` writes `"12"`, a `Buffer` writes its first `length`
  bytes).
- For binary data prefer `readToBuffer` / `writeFromBuffer` with a `Buffer`:
  one allocation reused for every chunk, and `getU8` / `setU8` access to the
  bytes.

There is no encoding conversion: a UTF-16 file reads as its raw bytes.

## Lines

`readLn(size)` reads one line, one byte at a time, and **keeps the line
end**:

| Bytes in the file | `readLn()` returns |
|-------------------|--------------------|
| `abc\r\n` | `"abc\r\n"` |
| `abc\n` | `"abc\n"` |
| `ab\rc\n` | `"ab\rc\n"` (a lone `\r` is part of the line) |
| `abc` then end of file | `"abc"` (last line without line end) |
| end of file | `undefined` |

- An empty line is `"\n"` or `"\r\n"`, so it is a non-empty string: test
  the end of file with `Script.isString(line)` / `Script.isUndefined(line)`,
  not with `if (line)`.
- Remove the line end with `line.trim()` (also removes spaces and tabs) or
  check the last bytes with `substring`.
- `size` (default 32768) limits the characters **before** the line end. A
  longer line is returned in pieces: the first call returns `size` bytes
  without a line end, the next call continues the same line.
- `writeLn(str)` writes `str` + `"\r\n"` on every platform. For `"\n"`
  files use `write(str + "\n")`.

## Standard streams

`openStdIn()`, `openStdOut()` and `openStdErr()` attach the object to the
process streams; they always return `true`.

- `close()` only detaches the object: the process stream stays open and
  other objects (and `Console`) keep using it.
- Unlike files, the standard streams keep the C runtime's default mode. On
  Windows that is **text mode**: `"\n"` written to `stdout` / `stderr`
  becomes `"\r\n"`, so `writeLn("x")` produces `x\r\r\n`; write
  `str + "\n"` to get proper line ends. On Linux nothing is translated.
- `stdout` is buffered: `flush()` before waiting for input or before the
  output must be seen by another process.
- `read(size)` on `stdin` returns what one read delivers (a console line, a
  pipe chunk); loop until `undefined`.

## Errors

The extension throws only for programming errors:

| Thrown | When |
|--------|------|
| `Error: invalid parameter` | a method called on something that is not a `File` (`File.prototype.read.call({})`), or `readToBuffer` / `writeFromBuffer` without a `Buffer` |
| `Error: setPropertyBySymbol` | assigning a property to a file object (`f.name = "x"`) |
| `Unable to open "File"` | the extension (or `Buffer`) library was not found and no internal one is registered |

I/O problems are return values:

| Function | Failure / end |
|----------|---------------|
| `open*` | `false` |
| `read`, `readLn` | `undefined` at end of file or on a closed stream; `undefined` for an invalid size |
| `write`, `writeLn`, `writeFromBuffer` | fewer bytes than requested (`0` when not open) |
| `readToBuffer` | `0` |
| `seekFromBegin`, `seek`, `seekFromEnd` | `false`; `undefined` for an invalid number |
| `seekTell` | `18446744073709551616` when not open |

## Values and prototype

| Expression | Result |
|------------|--------|
| `typeof(f)` | `"File"` |
| `f instanceof File` | `true` |
| `File.isFile(f)` | `true` (`false` for anything else) |
| `"" + f`, `Convert.toString(f)` | `"File"` |
| `if (f)` | always `true`, open or not |
| `var g = f;` | the same object (reference) |

Methods live on `File.prototype`, which scripts can extend:

```javascript
File.prototype.readAll = function() {
	var out = "";
	var chunk;
	while (Script.isString(chunk = this.read(65536))) {
		out += chunk;
	};
	return out;
};
```

Own properties cannot be added to a file object; keep extra data (the file
name, a line counter) next to it in an object.
