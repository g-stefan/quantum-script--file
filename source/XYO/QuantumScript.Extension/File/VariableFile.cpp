// Quantum Script Extension File
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript.Extension/File/Context.hpp>
#include <XYO/QuantumScript.Extension/File/VariableFile.hpp>

namespace XYO::QuantumScript::Extension::File {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableFile, "{81FA9545-8011-4BE0-9268-90383DDF8793}");

	const char *VariableFile::strTypeFile = "File";

	VariableFile::VariableFile() {
		XYO_DYNAMIC_TYPE_PUSH(VariableFile);
	};

	String VariableFile::getVariableType() {
		return strTypeFile;
	};

	Variable *VariableFile::newVariable() {
		return (Variable *)TMemory<VariableFile>::newMemory();
	};

	Variable *VariableFile::instancePrototype() {
		return (Extension::File::getContext())->prototypeFile->prototype;
	};

	void VariableFile::activeDestructor() {
		value.close();
	};

	Variable *VariableFile::clone(SymbolList &inSymbolList) {
		VariableFile *out = (VariableFile *)newVariable();
		out->value.becomeOwner(value);
		return out;
	};

	bool VariableFile::toBoolean() {
		return true;
	};

	String VariableFile::toString() {
		return strTypeFile;
	};

};
