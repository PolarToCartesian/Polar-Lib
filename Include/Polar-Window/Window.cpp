#include <Polar-Window/Window.hpp>

namespace PL {

    namespace details {
#ifdef __POLAR__TARGET_WINDOWS
        LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
#ifdef __POLAR__TARGET_64_BIT
			Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
#else
			Window* pWindow = reinterpret_cast<Window*>(GetWindowLongA(hwnd, GWLP_USERDATA));
#endif

			if (pWindow != nullptr) {
				switch (msg) {
				// Window Events
				case WM_DESTROY:
					pWindow->Close();
					return 0;
				// Mouse Events
				case WM_MOUSEMOVE:
					pWindow->m_cursorPosition = { static_cast<std::uint16_t>(GET_X_LPARAM(lParam)),
											   static_cast<std::uint16_t>(GET_Y_LPARAM(lParam)) };

					return 0;
				case WM_LBUTTONDOWN:
					pWindow->m_bLeftButton = true;

					return 0;
				case WM_LBUTTONUP:
					pWindow->m_bLeftButton = false;

					return 0;
				case WM_RBUTTONDOWN:
					pWindow->m_bRightButton = true;

					return 0;
				case WM_RBUTTONUP:
					pWindow->m_bRightButton = false;

					return 0;
				case WM_MOUSEWHEEL:
					pWindow->m_wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);

					return 0;

				// Keyboard Events
				case WM_KEYDOWN:
					pWindow->m_downKeys[static_cast<std::uint8_t>(wParam)] = true;

					return 0;

				case WM_KEYUP:
					pWindow->m_downKeys[static_cast<std::uint8_t>(wParam)] = false;

					return 0;
				}
			}

		    return DefWindowProcA(hwnd, msg, wParam, lParam);
        }
#endif // __POLAR__TARGET_WINDOWS
    }; // namespace details

    Window::Window(const char* title, const std::uint16_t width, const std::uint16_t height) noexcept
    {
#ifdef __POLAR__TARGET_WINDOWS
        const HINSTANCE hInstance = GetModuleHandleA(NULL);

        // Step 1 : Register Window Class
		WNDCLASSA wc = { };
		{
			wc.lpfnWndProc   = ::PL::details::WindowProc;
			wc.hInstance     = hInstance;
			wc.lpszClassName = "PL Window Class";

			RegisterClassA(&wc);
		}

        { // Step 2 : Create Window
			// Get The Desired Window Rect From The desired Client Rect
			RECT rect{ 0, 0, width, height };
			{
				if (!AdjustWindowRect(&rect, NULL, false))
					return;
			}

			this->m_handle = CreateWindowExA(
				0, "PL Window Class",
				title, WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
				NULL, NULL, hInstance, NULL
			);

			if (this->m_handle == NULL)
				return;
			
			this->m_bRunning = true;
		}

		// Show Window
		this->Show();

		// Step 4 : Set Window Pointer
		{
#ifdef __POLAR__TARGET_64_BIT
			SetWindowLongPtrA(this->m_handle, GWLP_USERDATA, (LONG_PTR)this);
#else
			SetWindowLongA(this->m_handle, GWLP_USERDATA, (LONG)this);
#endif
		}

#endif // __POLAR__TARGET_WINDOWS
    }

    void Window::Update() noexcept {
		this->m_wheelDelta = 0;

#ifdef __POLAR__TARGET_WINDOWS
		MSG msg = { };
		while (PeekMessageA(&msg, this->m_handle, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
#endif // __POLAR__TARGET_WINDOWS
	}

}; // namespace PL