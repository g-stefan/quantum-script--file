// Quantum Script Extension File
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_FILE_VARIABLEFILE_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_FILE_VARIABLEFILE_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_FILE_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/File/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::File {

	class VariableFile;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::File::VariableFile> : public TMemoryPoolActive<XYO::QuantumScript::Extension::File::VariableFile> {};
};

namespace XYO::QuantumScript::Extension::File {

	using namespace XYO;

	class VariableFile : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableFile);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT, VariableFile);

		protected:
			XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT static const char *strTypeFile;

		public:
			XYO::System::File value;

			XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT VariableFile();

			XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT void activeDestructor();

			XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT String toString();
	};
};

#endif
