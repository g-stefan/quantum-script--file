//
// Quantum Script Extension File
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_FILE_COPYRIGHT_HPP
#define QUANTUM_SCRIPT_EXTENSION_FILE_COPYRIGHT_HPP

#define QUANTUM_SCRIPT_EXTENSION_FILE_COPYRIGHT            "Copyright (c) Grigore Stefan"
#define QUANTUM_SCRIPT_EXTENSION_FILE_PUBLISHER            "Grigore Stefan"
#define QUANTUM_SCRIPT_EXTENSION_FILE_COMPANY              QUANTUM_SCRIPT_EXTENSION_FILE_PUBLISHER
#define QUANTUM_SCRIPT_EXTENSION_FILE_CONTACT              "g_stefan@yahoo.com"
#define QUANTUM_SCRIPT_EXTENSION_FILE_FULL_COPYRIGHT       QUANTUM_SCRIPT_EXTENSION_FILE_COPYRIGHT " <" QUANTUM_SCRIPT_EXTENSION_FILE_CONTACT ">"

#ifndef XYO_RC

#ifndef QUANTUM_SCRIPT_EXTENSION_FILE__EXPORT_HPP
#include "quantum-script-extension-file--export.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace File {
				namespace Copyright {
					QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT const char *copyright();
					QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT const char *publisher();
					QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT const char *company();
					QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT const char *contact();
					QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT const char *fullCopyright();
				};
			};
		};
	};
};

#endif
#endif
