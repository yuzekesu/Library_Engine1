#pragma once
#include <Windows.h>
#include <d3dcommon.h>
#include <exception>
#include <source_location>
#include <string>

/// # DESCRIPTION
/// 1. My own exception class.
/// 2. It has all the debugging information I need.
/// # REMARK
/// 1. Use either with HRESULT or just call the empty constructor.
/// # SEEALSO
/// 1. Exception::Exception(...) constructor. See how the layout is designed.
class Exception : public std::exception {
public:
	Exception() = delete;
	Exception(std::source_location = std::source_location::current());
	Exception(HRESULT, std::source_location = std::source_location::current());
	Exception(ID3DBlob&, std::source_location = std::source_location::current());
	Exception(std::wstring wstring, std::source_location = std::source_location::current());
	const char* what() const noexcept override;
private:
	std::string _msg;
};


