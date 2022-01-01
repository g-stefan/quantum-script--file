//
// Quantum Script Extension File
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEFILE_HPP
#define QUANTUM_SCRIPT_VARIABLEFILE_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace File {


				class VariableFile;

			};
		};

	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Extension::File::VariableFile>:
			public TMemoryPoolActive<Quantum::Script::Extension::File::VariableFile> {};
	};
};


namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace File {

				using namespace XYO;

				class VariableFile :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableFile);
						XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT, VariableFile);
					protected:
						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT static const char *strTypeFile;
					public:
						XYO::File value;

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT VariableFile();

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT void activeDestructor();

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT String getVariableType();

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT Variable *clone(SymbolList &inSymbolList);

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT String toString();

				};

			};
		};
	};
};


#endif

