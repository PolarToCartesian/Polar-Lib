#include <Polar-Window/Window.hpp>

namespace PL {

    namespace details {
#ifdef __POLAR__TARGET_WINDOWS
        LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
		    return DefWindowProcA(hwnd, msg, wParam, lParam);
        }
#endif // __POLAR__TARGET_WINDOWS
    }; // namespace details

    Window::Window(const char* title, const std::uint16_t x, const std::uint16_t y, const std::uint16_t width, const std::uint16_t height) noexcept
    {
#ifdef __POLAR__TARGET_WINDOWS
        const HINSTANCE hInstance = GetModuleHandle(NULL);

        // Step 1 : Register Window Class
		WNDCLASSA wc = { };
		{
			wc.lpfnWndProc   = ::PL::details::WindowProc;
			wc.hInstance     = hInstance;
			wc.lpszClassName = "Weiss's Windows Window Class";

			RegisterClassA(&wc);
		}

        { // Step 2 : Create Window
			// Get The Desired Window Rect From The desired Client Rect
			RECT rect{ 0, 0, width, height };
			{
				if (!AdjustWindowRect(&rect, NULL, false))
					WS_THROW("[WS] --> [WIN 32] Failed To Ajust Window Rect");
			}

			this->m_windowHandle = CreateWindowEx(
				0, L"Weiss's Windows Window Class",
				titleW.get(), WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
				NULL, NULL, hInstance, NULL
			);

			if (this->m_windowHandle == NULL) {
				WS_THROW("[WS] --> [WIN 32] Window Creation Failed");
			} else {
				this->m_bIsRunning = true;
			}
		}

#endif // __POLAR__TARGET_WINDOWS
    }

}; // namespace PL