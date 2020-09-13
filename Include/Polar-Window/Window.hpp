#ifndef __POLAR__FILE_WINDOW_HPP
#define __POLAR__FILE_WINDOW_HPP

#include <Polar-Math/Vector.hpp>
#include <Polar-Build/Polar-Build.hpp>

#ifdef __POLAR__TARGET_WINDOWS
    #include <Windows.h>
    #include <windowsx.h>
#endif // __POLAR__TARGET_WINDOWS

#ifdef __POLAR__TARGET_LINUX
    #include <X11/Xos.h>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
#endif // __POLAR__TARGET_LINUX

#include <array>
#include <cstdint>

namespace PL {

    namespace details {
#ifdef __POLAR__TARGET_WINDOWS
        LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
#endif // __POLAR__TARGET_WINDOWS
    }; // namespace details

    class Window {
#ifdef __POLAR__TARGET_WINDOWS
        friend LRESULT CALLBACK ::PL::details::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
#endif // __POLAR__TARGET_WINDOWS
    private:
#ifdef __POLAR__TARGET_WINDOWS
        HWND m_handle = NULL;
#endif // __POLAR__TARGET_WINDOWS

#ifdef __POLAR__TARGET_LINUX
        ::Display* m_pDisplayHandle;
        ::Window   m_handle;

        ::Atom m_deleteMessage;
#endif // __POLAR__TARGET_LINUX

        bool m_bRunning = false;

        // Mouse
        std::int32_t m_wheelDelta = 0;
        ::PL::Point2<std::uint32_t> m_cursorPosition;
        bool m_bLeftButton  = false;
        bool m_bRightButton = false;

        // Keyboard
        std::array<bool, 0xFE> m_downKeys = { false };

public:
        Window() = default;
        Window(const char* title, const std::uint16_t width, const std::uint16_t height) noexcept;

        void Show() const noexcept;
        void Hide() const noexcept;

        void Update() noexcept;

        inline bool IsRunning() const noexcept { return this->m_bRunning; }

        // Mouse
        inline bool IsLeftMouseButtonDown()  const noexcept { return this->m_bLeftButton;   }
        inline bool IsLeftMouseButtonUp()    const noexcept { return !this->m_bLeftButton;  }
        inline bool IsRightMouseButtonDown() const noexcept { return this->m_bRightButton;  }
        inline bool IsRightMouseButtonUp()   const noexcept { return !this->m_bRightButton; }

        inline std::int32_t                GetMouseWheelDelta() const noexcept { return this->m_wheelDelta;     }
        inline ::PL::Point2<std::uint32_t> GetCursorPosition()  const noexcept { return this->m_cursorPosition; }

        // Keyboard
        inline bool IsKeyDown(const char key) const noexcept { return this->m_downKeys[key];  }
        inline bool IsKeyup  (const char key) const noexcept { return !this->m_downKeys[key]; }

#ifdef __POLAR__TARGET_WINDOWS
        inline HWND GetHandle() const noexcept { return this->m_handle;   }
#endif // __POLAR__TARGET_WINDOWS

        void Close() noexcept;

        inline ~Window() noexcept { this->Close(); }
    }; // class Window

}; // namespace PL

#endif // __POLAR__FILE_WINDOW_HPP