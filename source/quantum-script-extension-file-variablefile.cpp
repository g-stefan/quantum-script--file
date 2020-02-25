//
// Quantum Script Extension File
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-context.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"

#include "quantum-script-extension-file.hpp"
#include "quantum-script-extension-file-variablefile.hpp"


namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace File {


				using namespace XYO;

				const char *VariableFile::typeFileKey = "{81FA9545-8011-4BE0-9268-90383DDF8793}";
				const void *VariableFile::typeFile;
				const char *VariableFile::strTypeFile = "File";

				String VariableFile::getType() {
					return strTypeFile;
				};

				Variable *VariableFile::newVariable() {
					return (Variable *) TMemory<VariableFile>::newMemory();
				};

				Variable &VariableFile::operatorReference(Symbol symbolId) {
					return operatorReferenceX(symbolId, (Extension::File::getContext())->prototypeFile->prototype);
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
		};
	};
};


