//
// Quantum Script Extension File
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
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
					protected:
						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT static const char *strTypeFile;
						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT static const char *typeFileKey;
						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT static const void *typeFile;
					public:

						XYO::File value;

						inline VariableFile() {
							variableType = registerType(typeFile, typeFileKey);
						};

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT void activeDestructor();

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT String getType();

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT Variable &operatorReference(Symbol symbolId);
						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT Variable *clone(SymbolList &inSymbolList);

						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT String toString();
						//
						inline static bool isVariableFile(const Variable *value) {
							if(typeFile == nullptr) {
								typeFile = registerType(typeFile, typeFileKey);
							};
							return (value->variableType == typeFile);
						};

				};

			};
		};
	};
};


#endif

