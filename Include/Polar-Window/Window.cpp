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
				case WM_SIZE:
					pWindow->m_clientWidth  = GET_X_LPARAM(lParam);
					pWindow->m_clientHeight = GET_Y_LPARAM(lParam);
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
		}
#endif // __POLAR__TARGET_WINDOWS

#ifdef __POLAR__TARGET_LINUX
		// Fetch Display
		this->m_pDisplayHandle = ::XOpenDisplay(0);

		// Create Window
		this->m_handle = ::XCreateSimpleWindow(this->m_pDisplayHandle, DefaultRootWindow(this->m_pDisplayHandle),
											   0, 0, width, height, 0,
											   BlackPixel(this->m_pDisplayHandle, 0), WhitePixel(this->m_pDisplayHandle, 0));

		// Receive WM_DELETE_WINDOW messages
		this->m_deleteMessage = ::XInternAtom(this->m_pDisplayHandle, "WM_DELETE_WINDOW", False);
		::XSetWMProtocols(this->m_pDisplayHandle, this->m_handle, &this->m_deleteMessage, 1);

		// Set Title
		::XSetStandardProperties(this->m_pDisplayHandle, this->m_handle, title, title, None, NULL, 0, NULL);

		// Select Input Masks
		constexpr long xEventMasks = ExposureMask      | StructureNotifyMask					   // Window
									 PointerMotionMask | ButtonPressMask     | ButtonReleaseMask | // Mouse
									 KeyPressMask	   | KeyReleaseMask;						   // Keyboard

		::XSelectInput(this->m_pDisplayHandle, this->m_handle, xEventMasks);
#endif // __POLAR__TARGET_LINUX

		this->m_bRunning     = true;
		this->m_clientWidth  = width;
		this->m_clientHeight = height;

		// Show Window
		this->Show();

#ifdef __POLAR__TARGET_WINDOWS
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

	void Window::Show() const noexcept {
#ifdef __POLAR__TARGET_WINDOWS
		ShowWindow(this->m_handle, SW_SHOW);
#endif // __POLAR__TARGET_WINDOWS
#ifdef __POLAR__TARGET_LINUX
		::XMapWindow(this->m_pDisplayHandle, this->m_handle);
		::XFlush(this->m_pDisplayHandle);
#endif // __POLAR__TARGET_LINUX
	}

	void Window::Hide() const noexcept {
#ifdef __POLAR__TARGET_WINDOWS
		ShowWindow(this->m_handle, SW_HIDE);
#endif // __POLAR__TARGET_WINDOWS
#ifdef __POLAR__TARGET_LINUX
		::XUnmapWindow(this->m_pDisplayHandle, this->m_handle);
		::XFlush(this->m_pDisplayHandle);
#endif // __POLAR__TARGET_LINUX
	}

	void Window::Close() noexcept {
#ifdef __POLAR__TARGET_WINDOWS
		CloseWindow(this->m_handle);

		this->m_handle = NULL;
#endif // __POLAR__TARGET_WINDOWS
#ifdef __POLAR__TARGET_LINUX
		// Destroy Window & Close Display
		::XDestroyWindow(this->m_pDisplayHandle, this->m_handle);
		::XCloseDisplay(this->m_pDisplayHandle);

		this->m_pDisplayHandle = nullptr;
#endif // __POLAR__TARGET_LINUX

		this->m_bRunning = false;
	}

    void Window::Update() noexcept {
		this->m_wheelDelta = 0;

		const ::PL::details::WindowState prevState = *reinterpret_cast<::PL::details::WindowState*>(this);

#ifdef __POLAR__TARGET_WINDOWS
		MSG msg = { };
		while (PeekMessageA(&msg, this->m_handle, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
#endif // __POLAR__TARGET_WINDOWS
#ifdef __POLAR__TARGET_LINUX
		::XEvent xEvent;
		while (XPending(this->m_pDisplayHandle))
		{
			XNextEvent(this->m_pDisplayHandle, &xEvent);

			if (XFilterEvent(&xEvent, this->m_handle))
				continue;

			switch (xEvent.type) {
				// Window Events
			case DestroyNotify:
				this->Close();
				return;

			case ConfigureNotify:
				this->m_clientWidth  = xEvent.xconfigure.width;
				this->m_clientHeight = xEvent.xconfigure.height;
				break;

			case ClientMessage:
				if ((::Atom)xEvent.xclient.data.l[0] == this->m_deleteMessage) {
					this->Close();
					return;
				}

				break;
				// Mouse Events
			case ButtonPress:
				switch (xEvent.xbutton.button) {
				case Button1:
					this->m_bLeftButton = true;
					break;
				case Button3:
					this->m_bRightButton = true;
					break;
				case Button4:
					this->m_wheelDelta += xEvent.xbutton.y;
					break;
				case Button5:
					this->m_wheelDelta -= xEvent.xbutton.y;
					break;
				}

				this->m_cursorPosition = { static_cast<uint16_t>(xEvent.xmotion.x),
									       static_cast<uint16_t>(xEvent.xmotion.y) };

				break;
			case ButtonRelease:
				switch (xEvent.xbutton.button) {
				case 1:
					this->m_bLeftButton = false;
					break;
				case 3:
					this->m_bRightButton = false;
					break;
				}

				this->m_cursorPosition = { static_cast<uint16_t>(xEvent.xmotion.x),
									       static_cast<uint16_t>(xEvent.xmotion.y) };

				break;
			case MotionNotify:
				this->m_cursorPosition = { static_cast<uint16_t>(xEvent.xmotion.x),
									       static_cast<uint16_t>(xEvent.xmotion.y) };

				break;

				// Keyboard Events
			case KeyPress:
				this->m_downKeys[std::toupper(XLookupKeysym(&xEvent.xkey, 0))] = true;
				break;
			case KeyRelease:
				this->m_downKeys[std::toupper(XLookupKeysym(&xEvent.xkey, 0))] = false;
				break;
			}
		}
#endif // __POLAR__TARGET_LINUX

		if (this->m_clientDims != prevState.m_clientDims) {
			for (auto& functor : this->m_resizeFunctors)
				functor(this->m_clientDims);
		}

		if (this->m_cursorPosition != prevState.m_cursorPosition) {
			for (auto& functor : this->m_mouseMoveFunctors) {
				Vec2<std::int16_t> delta = this->m_cursorPosition;
				delta -= prevState.m_cursorPosition;

				functor(this->m_cursorPosition, delta);
			}
		}
	}

}; // namespace PL