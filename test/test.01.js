// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");
Script.requireExtension("File");

function check(condition, message) {
	if (!condition) {
		throw(new Error("test.01: " + message));
	};
};

var file = new File();
check(file.openWrite("test.01.seek.txt"), "openWrite");
file.write("0123456789");
file.close();

check(file.openReadOnly("test.01.seek.txt"), "openReadOnly");
check(file.seekFromEnd(0) && file.seekTell() == 10, "seekFromEnd(0) is the end");
check(file.seekFromEnd(3) && file.seekTell() == 7, "seekFromEnd(3) is 3 bytes before the end");
check(file.read() == "789", "read after seekFromEnd(3)");
check(file.seekFromEnd(10) && file.read(2) == "01", "seekFromEnd(size) is the beginning");
check(!file.seekFromEnd(11), "seekFromEnd past the beginning fails");
check(Script.isUndefined(file.seekFromEnd(-1)), "seekFromEnd(-1) is invalid");
file.close();
check(!file.seekFromEnd(0), "seekFromEnd not open");
