// Quantum Script Extension File
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/File/License.hpp>

namespace XYO::QuantumScript::Extension::File::License {

		static const char *license_ =
	    "        MIT License\r\n"
	    "\r\n"
	    "Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>\r\n"
	    "\r\n"
	    "Permission is hereby granted, free of charge, to any person obtaining a copy\r\n"
	    "of this software and associated documentation files (the \" Software \"), to deal\r\n"
	    "in the Software without restriction, including without limitation the rights\r\n"
	    "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\r\n"
	    "copies of the Software, and to permit persons to whom the Software is\r\n"
	    "furnished to do so, subject to the following conditions:\r\n"
	    "\r\n"
	    "The above copyright notice and this permission notice shall be included in all\r\n"
	    "copies or substantial portions of the Software.\r\n"
	    "\r\n"
	    "THE SOFTWARE IS PROVIDED \" AS IS \", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\r\n"
	    "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\r\n"
	    "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\r\n"
	    "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\r\n"
	    "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\r\n"
	    "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\r\n"
	    "SOFTWARE.\r\n";

	static const char *shortLicense_ =
	    "Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>\r\n"
	    "MIT License (MIT) <http://opensource.org/licenses/MIT>\r\n";

	const char *license() {
		return license_;
	};

	const char *shortLicense() {
		return shortLicense_;
	};


};
