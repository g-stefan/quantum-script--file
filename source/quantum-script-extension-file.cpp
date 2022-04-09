//
// Quantum Script Extension File
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-extension-file-license.hpp"
#include "quantum-script-extension-file.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_FILE_NO_VERSION
#	include "quantum-script-extension-file-version.hpp"
#endif
//
#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"
//
#include "quantum-script-extension-buffer-variablebuffer.hpp"
//
#include "quantum-script-extension-file-variablefile.hpp"

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace File {

				using namespace XYO;
				using namespace Quantum::Script;

				FileContext::FileContext() {
					symbolFunctionFile = 0;
					prototypeFile.pointerLink(this);
				};

				FileContext *getContext() {
					return TSingleton<FileContext>::getValue();
				};

				static TPointer<Variable> functionFile(VariableFunction *function, Variable *this_, VariableArray *arguments) {
					return VariableFile::newVariable();
				};

				static void deleteContext() {
					FileContext *fileContext = getContext();
					fileContext->prototypeFile.deleteMemory();
					fileContext->symbolFunctionFile = 0;
				};

				static void newContext(Executive *executive, void *extensionId) {
					VariableFunction *defaultPrototypeFunction;

					FileContext *fileContext = getContext();
					executive->setExtensionDeleteContext(extensionId, deleteContext);

					fileContext->symbolFunctionFile = Context::getSymbol("File");
					fileContext->prototypeFile.newMemory();

					defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionFile, nullptr, nullptr);
					(Context::getGlobalObject())->setPropertyBySymbol(fileContext->symbolFunctionFile, defaultPrototypeFunction);
					fileContext->prototypeFile = defaultPrototypeFunction->prototype;
				};

				static TPointer<Variable> isFile(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- script-is-file\n");
#endif
					return VariableBoolean::newVariable(TIsType<VariableFile>(arguments->index(0)));
				};

				static TPointer<Variable> fileOpenReadOnly(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-open-read-only\n");
#endif
					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableFile *)this_)->value.openRead((arguments->index(0))->toString()));
				};

				static TPointer<Variable> fileOpenWrite(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-open-write\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableFile *)this_)->value.openWrite((arguments->index(0))->toString()));
				};

				static TPointer<Variable> fileOpenReadAndWrite(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-open-read-and-write\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableFile *)this_)->value.openReadAndWrite((arguments->index(0))->toString()));
				};

				static TPointer<Variable> fileOpenAppend(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-open-append\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableFile *)this_)->value.openAppend((arguments->index(0))->toString()));
				};

				static TPointer<Variable> fileOpenStdIn(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-open-stdin\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableFile *)this_)->value.openStdIn());
				};

				static TPointer<Variable> fileOpenStdOut(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-open-stdout\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableFile *)this_)->value.openStdOut());
				};

				static TPointer<Variable> fileOpenStdErr(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-open-stder\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableFile *)this_)->value.openStdErr());
				};

				static TPointer<Variable> fileRead(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-read\n");
#endif
					String retV;
					Number ln;

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					if (TIsTypeExact<VariableUndefined>(arguments->index(0))) {
						ln = 16384;
					} else {
						ln = (arguments->index(0))->toNumber();
						if (isnan(ln) || isinf(ln) || signbit(ln)) {
							return Context::getValueUndefined();
						};
					};

					if (StreamX::read(((VariableFile *)this_)->value, retV, ln)) {
						return VariableString::newVariable(retV);
					};

					return Context::getValueUndefined();
				};

				static TPointer<Variable> fileReadLn(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-read-ln\n");
#endif
					String retV;
					Number ln;

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					if (TIsTypeExact<VariableUndefined>(arguments->index(0))) {
						ln = 16384;
					} else {
						ln = (arguments->index(0))->toNumber();
						if (isnan(ln) || isinf(ln) || signbit(ln)) {
							return Context::getValueUndefined();
						};
					};

					if (StreamX::readLn(((VariableFile *)this_)->value, retV, ln)) {
						return VariableString::newVariable(retV);
					};

					return Context::getValueUndefined();
				};

				static TPointer<Variable> fileWrite(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-write\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable((Number)StreamX::write(((VariableFile *)this_)->value, (arguments->index(0))->toString()));
				};

				static TPointer<Variable> fileWriteLn(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-write-ln\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable((Number)StreamX::writeLn(((VariableFile *)this_)->value, (arguments->index(0))->toString()));
				};

				static TPointer<Variable> fileClose(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-close\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					((VariableFile *)this_)->value.close();

					return Context::getValueUndefined();
				};

				static TPointer<Variable> fileFlush(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-flush\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					((VariableFile *)this_)->value.flush();

					return Context::getValueUndefined();
				};

				static TPointer<Variable> fileSeekFromBegin(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-seek-from-begin\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number ln;

					ln = (arguments->index(0))->toNumber();
					if (isnan(ln) || isinf(ln) || signbit(ln)) {
						return Context::getValueUndefined();
					};

					return VariableBoolean::newVariable(
					    ((VariableFile *)this_)->value.seekFromBegin((Integer)ln));
				};

				static TPointer<Variable> fileSeek(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-seek\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number ln;

					ln = (arguments->index(0))->toNumber();
					if (isnan(ln) || isinf(ln)) {
						return Context::getValueUndefined();
					};
					return VariableBoolean::newVariable(
					    ((VariableFile *)this_)->value.seek((Integer)ln));
				};

				static TPointer<Variable> fileSeekFromEnd(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-seek-from-end\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number ln;

					ln = (arguments->index(0))->toNumber();
					if (isnan(ln) || isinf(ln) || signbit(ln)) {
						return Context::getValueUndefined();
					};

					return VariableBoolean::newVariable(
					    ((VariableFile *)this_)->value.seekFromEnd((Integer)ln));
				};

				static TPointer<Variable> fileSeekTell(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-seek-tell\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(
					    (Number)((VariableFile *)this_)->value.seekTell());
				};

				static TPointer<Variable> fileReadToBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-read-to-buffer\n");
#endif
					size_t readLn;
					Number ln;

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &buffer(arguments->index(0));

					if (!TIsType<Extension::Buffer::VariableBuffer>(buffer)) {
						throw(Error("invalid parameter"));
					};

					if (TIsTypeExact<VariableUndefined>(arguments->index(1))) {
						ln = 16384;
					} else {
						ln = (arguments->index(1))->toNumber();
						if (isnan(ln) || signbit(ln) || ln == 0.0) {
							((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.length = 0;
							return VariableNumber::newVariable(0);
						};
						if (isinf(ln)) {
							ln = ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.size;
						};
					};

					if (ln > ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.size) {
						ln = ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.size;
					};

					readLn = ((VariableFile *)this_)->value.read(((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.buffer, ln);
					((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.length = readLn;
					return VariableNumber::newVariable(readLn);
				};

				static TPointer<Variable> fileWriteFromBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-write-from-buffer\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &buffer(arguments->index(0));

					if (!TIsType<Extension::Buffer::VariableBuffer>(buffer)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable((Number)(((VariableFile *)this_)->value.write(((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.buffer, ((Extension::Buffer::VariableBuffer *)buffer.value())->buffer.length)));
				};

				static TPointer<Variable> fileBecomeOwner(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-become-owner\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &value = arguments->index(0);

					if (!TIsType<VariableFile>(value)) {
						throw(Error("invalid parameter"));
					};

					((VariableFile *)this_)->value.becomeOwner(((VariableFile *)(value.value()))->value);

					return Context::getValueUndefined();
				};

				static TPointer<Variable> fileLinkOwner(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-link-owner\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &value = arguments->index(0);

					if (!TIsType<VariableFile>(value)) {
						throw(Error("invalid parameter"));
					};

					((VariableFile *)this_)->value.linkOwner(((VariableFile *)(value.value()))->value);

					return Context::getValueUndefined();
				};

				static TPointer<Variable> fileUnLinkOwner(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-unlink-owner\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					((VariableFile *)this_)->value.unLinkOwner();

					return Context::getValueUndefined();
				};

				static TPointer<Variable> fileTransferOwner(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- file-transfer-owner\n");
#endif

					if (!TIsType<VariableFile>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &value = arguments->index(0);

					if (!TIsType<VariableFile>(value)) {
						throw(Error("invalid parameter"));
					};

					((VariableFile *)this_)->value.transferOwner(((VariableFile *)(value.value()))->value);

					return Context::getValueUndefined();
				};

				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("File", initExecutive);
				};

				void initExecutive(Executive *executive, void *extensionId) {

					String info = "File\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "File");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_FILE_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::File::Version::versionWithBuild());
#endif
					executive->setExtensionPublic(extensionId, true);

					newContext(executive, extensionId);

					executive->compileStringX("Script.requireExtension(\"Buffer\");");

					executive->setFunction2("File.isFile(x)", isFile);
					executive->setFunction2("File.prototype.openReadOnly(file)", fileOpenReadOnly);
					executive->setFunction2("File.prototype.openWrite(file)", fileOpenWrite);
					executive->setFunction2("File.prototype.openReadAndWrite(file)", fileOpenReadAndWrite);
					executive->setFunction2("File.prototype.openAppend(file)", fileOpenAppend);
					executive->setFunction2("File.prototype.openStdIn()", fileOpenStdIn);
					executive->setFunction2("File.prototype.openStdOut()", fileOpenStdOut);
					executive->setFunction2("File.prototype.openStdErr()", fileOpenStdErr);
					executive->setFunction2("File.prototype.read(size)", fileRead);
					executive->setFunction2("File.prototype.readLn(size)", fileReadLn);
					executive->setFunction2("File.prototype.write(str)", fileWrite);
					executive->setFunction2("File.prototype.writeLn(str)", fileWriteLn);
					executive->setFunction2("File.prototype.close()", fileClose);
					executive->setFunction2("File.prototype.flush()", fileFlush);
					executive->setFunction2("File.prototype.seekFromBegin(pos)", fileSeekFromBegin);
					executive->setFunction2("File.prototype.seekFromEnd(pos)", fileSeekFromEnd);
					executive->setFunction2("File.prototype.seek(pos)", fileSeek);
					executive->setFunction2("File.prototype.seekTell()", fileSeekTell);
					executive->setFunction2("File.prototype.readToBuffer(buffer)", fileReadToBuffer);
					executive->setFunction2("File.prototype.writeFromBuffer(buffer)", fileWriteFromBuffer);
					executive->setFunction2("File.prototype.becomeOwner(file)", fileBecomeOwner);
					executive->setFunction2("File.prototype.linkOwner(file)", fileLinkOwner);
					executive->setFunction2("File.prototype.unLinkOwner(file)", fileUnLinkOwner);
					executive->setFunction2("File.prototype.transferOwner(file)", fileTransferOwner);
				};

			};
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_FILE_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::File::initExecutive(executive, extensionId);
};
#endif
