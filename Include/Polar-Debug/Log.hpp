#ifndef __POLAR__FILE_LOG_HPP
#define __POLAR__FILE_LOG_HPP

#include <mutex>
#include <iostream>

#include <Polar-Build/Polar-Build.hpp>

#ifdef __POLAR__TARGET_WINDOWS
    #include <Windows.h>
#endif // __POLAR__TARGET_WINDOWS

namespace PL {

	enum class LOG_TYPE {
		LOG_NORMAL,
		LOG_SUCCESS,
		LOG_WARNING,
		LOG_ERROR
	}; // enum class LOG_TYPE

    class LOG {
	private:
		static std::mutex m_sPrintMutex;

	private:
		template <typename T, typename ...Args>
		static void __Print(const LOG_TYPE logType, const T& message0, Args... args) noexcept {
#ifdef __POLAR__TARGET_WINDOWS

			WORD textAttributes;

			switch (logType) {
			case LOG_TYPE::LOG_NORMAL:
				textAttributes = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				break;
			case LOG_TYPE::LOG_SUCCESS:
				textAttributes = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
				break;
			case LOG_TYPE::LOG_WARNING:
				textAttributes = FOREGROUND_RED | FOREGROUND_GREEN;
				break;
			case LOG_TYPE::LOG_ERROR:
				textAttributes = FOREGROUND_INTENSITY | FOREGROUND_RED;
				break;
			}

			// Set Text Color
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textAttributes);

			std::cout << message0;

			(std::cout << ... << args);

			// Reset Text Color To White
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	#else // end of #ifdef __POLAR__TARGET_WINDOWS

			switch (logType) {
			case LOG_TYPE::LOG_NORMAL:
				std::cout << "\x1B[1;37m";
				break;
			case LOG_TYPE::LOG_SUCCESS:
				std::cout << "\x1B[1;32m";
				break;
			case LOG_TYPE::LOG_WARNING:
				std::cout << "\x1B[0;33m";
				break;
			case LOG_TYPE::LOG_ERROR:
				std::cout << "\x1B[1;31m";
				break;
			}

			std::cout << message0;
			(std::cout << ... << args);
			std::cout << "\x1B[0m";

	#endif
		}

	public:
		template <typename T, typename ...Args>
		static void Print(const LOG_TYPE logType,   const T& message0, Args... args) noexcept {
			// Lock The Mutex
			std::lock_guard<std::mutex> lock(LOG::m_sPrintMutex);

			PL::LOG::__Print(logType, message0, args...);
		}

		template <typename T, typename ...Args>
		static void Println(const LOG_TYPE logType, const T& message0, Args... args) noexcept {
			// Lock The Mutex
			std::lock_guard<std::mutex> lock(LOG::m_sPrintMutex);

			PL::LOG::__Print(logType, message0, args..., '\n');
		}
	}; // class LOG

}; // namespace PL

#endif // __POLAR__FILE_LOG_HPP