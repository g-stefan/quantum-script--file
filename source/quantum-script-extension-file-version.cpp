//
// Quantum Script Extension File
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "quantum-script-extension-file-version.hpp"

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace File {
				namespace Version {

					static const char *version_ = "2.10.0";
					static const char *build_ = "21";
					static const char *versionWithBuild_ = "2.10.0.21";
					static const char *datetime_ = "2022-01-01 21:19:41";

					const char *version() {
						return version_;
					};
					const char *build() {
						return build_;
					};
					const char *versionWithBuild() {
						return versionWithBuild_;
					};
					const char *datetime() {
						return datetime_;
					};

				};
			};
		};
	};
};



