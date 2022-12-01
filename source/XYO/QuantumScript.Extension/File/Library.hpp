// Quantum Script Extension File
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_FILE_LIBRARY_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_FILE_LIBRARY_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_FILE_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/File/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::File {

	XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT void initExecutive(Executive *executive, void *extensionId);
	XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT void registerInternalExtension(Executive *executive);

};

#endif
