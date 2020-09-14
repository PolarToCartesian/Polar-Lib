#include <Polar-Debug/Log.hpp>

namespace PL {

    template <typename T, typename ...Args>
	void LOG::__Print(const LOG_TYPE logType, const T& message0, Args... args) noexcept {
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

    template <typename T, typename ...Args>
	void LOG::Print(const LOG_TYPE logType,   const T& message0, Args... args) noexcept {
		// Lock The Mutex
		std::lock_guard<std::mutex> lock(LOG::m_sPrintMutex);

		PL::LOG::__Print(logType, message0, args...);
	}

	template <typename T, typename ...Args>
	void LOG::Println(const LOG_TYPE logType, const T& message0, Args... args) noexcept {
		// Lock The Mutex
		std::lock_guard<std::mutex> lock(LOG::m_sPrintMutex);

		PL::LOG::__Print(logType, message0, args..., '\n');
	}

}; // namespace PL