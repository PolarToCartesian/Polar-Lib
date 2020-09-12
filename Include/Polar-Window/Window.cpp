#include <Polar-Window/Window.hpp>

namespace PL {

    namespace details {
#ifdef __POLAR__TARGET_WINDOWS
        LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
#ifdef __POLAR__TARGET_64_BIT
			Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
#else
			Window* pWindow = reinterpret_cast<Window*>(GetWindowLong(hwnd, GWLP_USERDATA));
#endif

			if (pWindow != nullptr) {
				switch (msg) {
				case WM_DESTROY:
					pWindow->Close();
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
        const HINSTANCE hInstance = GetModuleHandle(NULL);

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

			if (this->m_handle == NULL) {
				return;
			}

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
#ifdef __POLAR__TARGET_WINDOWS
		MSG msg = { };
		while (PeekMessageA(&msg, this->m_handle, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
#endif // __POLAR__TARGET_WINDOWS
	}

}; // namespace PL