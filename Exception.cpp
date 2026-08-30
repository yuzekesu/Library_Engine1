#pragma comment(lib, "d3dcompiler.lib")
#include "Exception.h"
#include <sstream>

/// # CONSTRUCTION	
/// 1. Format the error message line by line.
/// # PARAM
/// l: Leave it empty, we have default value.
Exception::Exception(std::source_location l) {
	std::stringstream s;
	s << "File: " << l.file_name() << "\n";
	s << "Function: " << l.function_name() << "\n";
	s << "Line: " << l.line() << "; ";
	s << "Column: " << l.column() << "\n";
	this->_msg = s.str();
}
/// # CONSTRUCT	
/// 1. Lookup and convert the HRESULT to error messages.
/// # REMARK
/// 1. Chains the Exception::Exception(std::source_location)
/// # SEEALSO
/// 1. Exception::Exception(std::source_location) constructor.
/// # PARAM
/// h: The HRESULT from the win32 related API.
/// l: Leave it empty, we have default value.
Exception::Exception(HRESULT h, std::source_location l) : Exception(l) {
	std::stringstream s;
	char* temp;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, h, 0, (char*)&temp, 0, nullptr);
	s << this->_msg;
	s << "\n" << "Description: " << temp << "\n";
	LocalFree(temp);
	this->_msg = s.str();
}
Exception::Exception(ID3DBlob& blob, std::source_location l) : Exception(l) {
	std::string error{ static_cast<const char*>(blob.GetBufferPointer()), blob.GetBufferSize() };
	this->_msg.append("\n Description: " + error + "\n");
}
Exception::Exception(std::wstring wstring, std::source_location l) : Exception(l) {
	const int requiredSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstring.data(), static_cast<int>(wstring.size()), nullptr, 0, nullptr, nullptr);
	std::string result(static_cast<std::size_t>(requiredSize), '\0');
	const int convertedSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstring.data(), static_cast<int>(wstring.size()), result.data(), requiredSize, nullptr, nullptr);
	this->_msg.append("\n Description: " + result + "\n");
}
/// # DESCRIPTION	
/// 1. Inherited method from parent std::exception.
/// # REMARK
/// 1. Will be called inside the catch-block.
const char* Exception::what() const noexcept {
	return this->_msg.c_str();
}
