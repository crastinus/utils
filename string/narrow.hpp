#pragma once

#ifdef _MSC_VER 

#include <codecvt>
#include <locale>
#include <string>
#include <Windows.h>

namespace utils {

	inline std::string narrow(LPTSTR buffer) {
#ifdef UNICODE
		std::string result;
		size_t length = std::char_traits<TCHAR>::length(buffer);

		// empty string
		if (length == 0)
			return result;

		std::locale  loc("rus");
		
		result.resize(length);

		std::use_facet<std::ctype<wchar_t> >(loc).narrow(buffer, buffer + length, '?', &result[0]);

		return result;
#else
		return buffer;
#endif;
	}

    inline std::wstring widen (LPTSTR buffer) {
#ifdef UNICODE
        return buffer;
#else
        std::wstring result;
        size_t length = std::char_traits<TCHAR>::length (buffer);

        if (length == 0) return result;

        std::locale loc ("rus");

        result.resize (length);

        std::use_facet<std::ctype<wchar_t>> (loc).widen (buffer, buffer + length, &result[0]);

        return result;

#endif
    }

	inline std::basic_string<TCHAR> native(LPTSTR buffer) {
#ifdef UNICODE
		return widen(buffer);
#else
		return narrow(buffer);
#endif
	}

    inline std::basic_string<TCHAR> native(std::string src) 
    {
#ifdef UNICODE
        return widen(src.c_str());
#else
        return src;
#endif

    }
}
#endif
