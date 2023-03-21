// Quantum Script Extension File
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_FILE_COPYRIGHT_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_FILE_COPYRIGHT_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_FILE_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/File/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::File::Copyright {
	XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT std::string copyright();
	XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT std::string publisher();
	XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT std::string company();
	XYO_QUANTUMSCRIPT_EXTENSION_FILE_EXPORT std::string contact();
};

#endif
