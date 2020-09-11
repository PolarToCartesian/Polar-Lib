#ifndef __POLAR__FILE_LOG_HPP
#define __POLAR__FILE_LOG_HPP

#include <mutex>
#include <iostream>

#include <Polar-Build/Polar-Build.hpp>

#ifdef __POLAR__TARGET_WINDOWS
    #include <Windows.h>
#endif // __POLAR__TARGET_WINDOWS

enum class LOG_TYPE {
    LOG_NORMAL,
    LOG_SUCCESS,
    LOG_WARNING,
    LOG_ERROR
}; // enum class LOG_TYPE

namespace PL {

    class LOG {
	private:
		static std::mutex m_sPrintMutex;

	private:
		template <typename T, typename ...Args>
		static void __Print(const LOG_TYPE logType, const T& message0, Args... args) noexcept;

	public:
		template <typename T, typename ...Args>
		static void Print(const LOG_TYPE logType,   const T& message0, Args... args) noexcept;

		template <typename T, typename ...Args>
		static void Println(const LOG_TYPE logType, const T& message0, Args... args) noexcept;
	}; // class LOG

}; // namespace PL

#endif // __POLAR__FILE_LOG_HPP