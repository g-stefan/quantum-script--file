# Script API

Everything the extension defines after `Script.requireExtension("File")`.

Methods called on something that is not a file
(`File.prototype.read.call({})`) throw `Error: invalid parameter`. Every
`open*` closes the stream the object had open before. See
[File model](file-model.md) for modes, positions and lines.

## Constructor

### `File()` / `new File()`

Returns a new file object, not open. Arguments are ignored.

```javascript
var f = new File();
typeof(f);      // "File"
f.read();       // undefined: nothing open yet
```

## Static functions

### `File.isFile(x)`

`true` if `x` is a file object, `false` otherwise.

## Opening

All return `true` on success, `false` on failure. The name is converted
with `toString` and passed to `fopen`; relative names use the process
current directory.

### `openReadOnly(file)`

Opens an existing file for reading, binary mode (`"rb"`), position 0.

### `openWrite(file)`

Creates the file, or **truncates** an existing one, for writing (`"wb"`).
The folder must exist.

### `openReadAndWrite(file)`

Opens an existing file for reading and writing (`"r+b"`), position 0,
content kept. Fails if the file does not exist. Put a `seek*` or `flush()`
between a write and a read (see
[File model](file-model.md#switching-between-reading-and-writing)).

### `openAppend(file)`

Opens for appending (`"ab"`), creating the file if needed. Every write goes
to the end of the file, whatever the position.

### `openStdIn()`, `openStdOut()`, `openStdErr()`

Attach the object to the process `stdin`, `stdout` or `stderr`. Always
`true`. `close()` detaches without closing the process stream. On Windows
these streams are in text mode (`"\n"` written becomes `"\r\n"`).

## Reading

### `read(size)`

Reads up to `size` bytes and returns them as a string.

- `size` missing: **32768**. Fractions are truncated (`read(0.5)` is
  `read(0)`).
- Returns `""` for `size` 0.
- Returns fewer bytes when the end of the file is reached (or, for `stdin`
  and pipes, when one read delivers less).
- Returns **`undefined`** when nothing could be read: end of file, stream
  not open. Also `undefined` for a negative, `NaN` or infinite `size`.
  Never read a write-only stream (see
  [File model](file-model.md#open-modes)). To read everything, loop: `read()` does **not** read the
  whole file.

```javascript
var chunk;
while (Script.isString(chunk = f.read(65536))) {
	process(chunk);
};
```

### `readLn(size)`

Reads one line, **including its line end** (`"\r\n"` or `"\n"`).

- The line ends at `"\n"` or `"\r\n"`; a lone `"\r"` stays in the line.
- The last line of a file may have no line end.
- `size` (missing: **32768**) limits the bytes before the line end; a longer
  line is returned in pieces of `size` bytes, without line end, and the next
  call continues it. `size` 0 returns `""`.
- Returns **`undefined`** at the end of the file, on a stream that is not
  open, or for a negative, `NaN` or infinite `size`.
- Reads one byte at a time, so it never reads past the line: the position
  is just after the line end, and `read` / `readToBuffer` continue from
  there.

```javascript
var line;
while (Script.isString(line = f.readLn())) {
	line = line.trim();     // drop "\r\n" (and surrounding spaces)
	// ...
};
```

### `readToBuffer(buffer, ln)`

Reads bytes into an existing `Buffer`, from its index 0, with one read call.
Sets `buffer.length` to the bytes read and returns that number (`0` at the
end of the file).

- `buffer` must be a `Buffer`, otherwise throws `invalid parameter`.
- `ln` missing: 32768. `ln` is clipped to `buffer.size`; `Infinity` means
  `buffer.size`.
- `ln` `0`, negative or `NaN`: nothing is read, `buffer.length` becomes 0,
  returns 0.
- May return fewer than `ln` bytes before the end of the file (for example
  from a pipe); loop until it returns 0.

```javascript
var b = Buffer(65536);
var n;
while ((n = f.readToBuffer(b, b.size)) > 0) {
	// b.length == n
};
```

## Writing

### `write(str)`

Writes the bytes of `str` (converted with `toString`: numbers as text, a
`Buffer` as its first `length` bytes). Returns the number of bytes written,
`0` if the stream is not open or not writable.

### `writeLn(str)`

Writes `str` then `"\r\n"`. Returns the bytes written, line end included
(`writeLn("abc")` returns 5). The line end is `"\r\n"` on every platform;
use `write(str + "\n")` for `"\n"` lines.

### `writeFromBuffer(buffer)`

Writes the first `buffer.length` bytes of a `Buffer`. Returns the number of
bytes written. Throws `invalid parameter` if `buffer` is not a `Buffer`.

### `flush()`

Sends buffered data to the operating system. Returns `undefined`. Use it
before another program reads the file while it is still open, before
reading after a write in `openReadAndWrite` mode, and on `stdout` before
waiting for input.

### `close()`

Flushes and closes the stream (detaches, for the standard streams). Returns
`undefined`. Calling it on a closed object does nothing.

## Position

Position arguments are converted to numbers; fractions are truncated.

### `seekFromBegin(pos)`

Moves to byte `pos`. Returns `true` / `false`; `undefined` (no move) for a
negative, `NaN` or infinite `pos`. Positions past the end are allowed.

### `seek(delta)`

Moves `delta` bytes from the current position; **`delta` may be negative**.
Returns `true` / `false` (`false` before the start of the file); `undefined`
for `NaN` or infinite `delta`.

### `seekFromEnd(pos)`

Moves `pos` bytes **before** the end of the file: `seekFromEnd(0)` goes to
the end, `seekFromEnd(size)` to the beginning. Returns `true` / `false`.

- `pos` larger than the file size: returns `false`; the position is then
  unspecified (the C runtime decides: MSVC leaves it at the end of the
  file), so seek again before reading. Clip `pos` to the size to avoid it.
- Negative, `NaN` or infinite `pos` returns `undefined` without moving.
- Fails (`false`) on a stream that is not open or cannot seek (a pipe).

```javascript
f.seekFromEnd(3);
var last3 = f.read(3);
```

### `seekTell()`

Returns the current position in bytes. On a stream that is not open (or not
seekable) returns `18446744073709551616`, the C `-1` error value read as an
unsigned 64-bit number.

```javascript
f.seekFromEnd(0);
var size = f.seekTell();
f.seekFromBegin(0);
```
