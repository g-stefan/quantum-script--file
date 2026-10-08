# Recipes

All examples start with:

```javascript
Script.requireExtension("Console");
Script.requireExtension("File");      // also loads Buffer
```

## Read a text file line by line

```javascript
function forEachLine(fileName, fn) {
	var f = new File();
	if (!f.openReadOnly(fileName)) {
		return false;
	};
	var line;
	var lineNumber = 0;
	while (Script.isString(line = f.readLn())) {
		++lineNumber;
		fn(line.trim(), lineNumber);       // trim drops "\r\n" / "\n"
	};
	f.close();
	return true;
};

forEachLine("config.txt", function(line, n) {
	if (line.length == 0 || line.indexOf("#") == 0) {
		return;
	};
	Console.writeLn(n + ": " + line);
});
```

Test the end with `Script.isString(line)`: an empty line is `"\r\n"`, not
`""`.

## Read a whole file

```javascript
function readAll(fileName) {
	var f = new File();
	if (!f.openReadOnly(fileName)) {
		return undefined;
	};
	var out = "";
	var chunk;
	while (Script.isString(chunk = f.read(65536))) {
		out += chunk;
	};
	f.close();
	return out;
};
```

`Shell.fileGetContents(fileName)` from `quantum-script--shell` does the
same in one call.

## Write a text file

```javascript
var f = new File();
if (!f.openWrite("report.txt")) {
	throw "cannot create report.txt";
};
f.writeLn("name,value");          // "\r\n" line ends
f.writeLn("a," + 1);
f.write("b,2\n");                 // or choose "\n" yourself
f.close();
```

Create the folder first if needed:
`Shell.mkdirRecursivelyIfNotExists("output/reports");`.

## Append to a log

```javascript
function log(message) {
	var f = new File();
	if (f.openAppend("app.log")) {
		f.writeLn(message);
		f.close();
	};
};
```

Opening and closing for every message keeps the log complete even if the
script stops; for many messages keep one object open and call `flush()`
after each `writeLn`.

## Copy a binary file in chunks

```javascript
function copyFile(from, to) {
	var src = new File();
	var dst = new File();
	if (!src.openReadOnly(from)) {
		return false;
	};
	if (!dst.openWrite(to)) {
		src.close();
		return false;
	};
	var chunk = Buffer(65536);
	while (src.readToBuffer(chunk, chunk.size) > 0) {
		dst.writeFromBuffer(chunk);   // writes chunk.length bytes
	};
	src.close();
	dst.close();
	return true;
};
```

## File size

```javascript
function fileSize(fileName) {
	var f = new File();
	if (!f.openReadOnly(fileName)) {
		return -1;
	};
	f.seekFromEnd(0);
	var size = f.seekTell();
	f.close();
	return size;
};
```

## Read the last bytes of a file

`seekFromEnd(n)` moves `n` bytes before the end (and fails if the file is
shorter, so clip `n` to the size first):

```javascript
function tail(fileName, n) {
	var f = new File();
	if (!f.openReadOnly(fileName)) {
		return undefined;
	};
	f.seekFromEnd(0);
	var size = f.seekTell();
	if (n > size) {
		n = size;
	};
	f.seekFromEnd(n);
	var data = f.read(n);
	f.close();
	return data;
};
```

## Read a binary header

```javascript
var f = new File();
f.openReadOnly("image.png");
var head = Buffer(8);
f.readToBuffer(head, 8);
f.close();
if (head.toHex() == "89504e470d0a1a0a") {
	Console.writeLn("PNG");
};
```

## Patch bytes in place

```javascript
var f = new File();
if (f.openReadAndWrite("data.bin")) {       // the file must exist
	f.seekFromBegin(16);
	f.writeFromBuffer(Buffer.fromHex("cafe"));
	f.close();
};
```

`openWrite` would truncate the file; `openAppend` would write at the end.

## Update a counter stored in a file

```javascript
function nextNumber(fileName) {
	var f = new File();
	var value = 0;
	if (f.openReadAndWrite(fileName)) {
		value = Convert.toNumber(f.readLn());
		f.seekFromBegin(0);                 // seek between read and write
	} else {
		f.openWrite(fileName);
	};
	++value;
	f.write("" + value + "\n");
	f.close();
	return value;
};
```

The new text must be at least as long as the old one, or the old tail
remains: write fixed width values, or rewrite the whole file with
`openWrite`.

## A stdin to stdout filter

```javascript
var input = new File();
var output = new File();
input.openStdIn();
output.openStdOut();

var line;
while (Script.isString(line = input.readLn())) {
	output.write(line.trim().toUpperCaseASCII() + "\n");   // "\n": stdout is text mode on Windows
};
output.flush();
```

```bash
quantum-script upper.js < input.txt > output.txt
```

## Errors to stderr

```javascript
var err = new File();
err.openStdErr();
err.write("error: input.txt not found\n");
Script.setExitCode(1);
```

## Generate a source file from a build script

```javascript
// fabricare script: File is registered as internal
Script.requireExtension("File");
Script.requireExtension("Shell");

Shell.mkdirRecursivelyIfNotExists("temp");
var h = new File();
if (h.openWrite("temp/Resources.hpp")) {
	h.writeLn("// generated, do not edit");
	var files = ["logo.png", "icon.ico"];
	var k;
	for (k = 0; k < files.length; ++k) {
		h.writeLn("// " + files[k]);
	};
	h.close();
};
```
