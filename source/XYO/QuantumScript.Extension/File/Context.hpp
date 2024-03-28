// Quantum Script Extension File
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_FILE_CONTEXT_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_FILE_CONTEXT_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_FILE_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/File/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::File {

	class FileContext : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(FileContext);

		public:
			Symbol symbolFunctionFile;
			TPointerX<Prototype> prototypeFile;

			XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT FileContext();
	};

	XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT FileContext *getContext();
};

#endif
