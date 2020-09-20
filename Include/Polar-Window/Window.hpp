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
#include <functional>

namespace PL {

    namespace details {
        struct WindowState {
            bool m_bRunning = false;

            // Mouse
            std::int32_t m_wheelDelta = 0;
            ::PL::Point2<std::uint16_t> m_cursorPosition;
            bool m_bLeftButton  = false;
            bool m_bRightButton = false;

            union {
                ::PL::Vec2<std::uint16_t> m_clientDims;
                struct {
                    std::uint16_t m_clientWidth;
                    std::uint16_t m_clientHeight;
                };
            };
            

            // Keyboard
            std::array<bool, 0xFE> m_downKeys = { false };

            // Events
            std::vector<std::function<void(const Vec2<std::uint16_t>)>> m_resizeFunctors;
            std::vector<std::function<void(const Vec2<std::uint16_t>, const Vec2<std::int16_t>)>> m_mouseMoveFunctors;
        
            WindowState() {  }
        };

#ifdef __POLAR__TARGET_WINDOWS
        LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
#endif // __POLAR__TARGET_WINDOWS
    }; // namespace details

    class Window : private PL::details::WindowState {
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

public:
        Window() = default;
        Window(const char* title, const std::uint16_t width, const std::uint16_t height) noexcept;

        void Update() noexcept;

        // Window

        void Show() const noexcept;
        void Hide() const noexcept;

        inline bool IsRunning() const noexcept { return this->m_bRunning; }

        inline std::uint16_t GetClientWidth()  const noexcept { return this->m_clientWidth;  }
        inline std::uint16_t GetClientHeight() const noexcept { return this->m_clientHeight; }

        // Window Event Hooking
        inline void OnResize(const std::function<void(const Vec2<std::uint16_t>)>& functor) noexcept { this->m_resizeFunctors.push_back(functor); }

        // Mouse
        inline bool IsLeftMouseButtonDown()  const noexcept { return this->m_bLeftButton;   }
        inline bool IsLeftMouseButtonUp()    const noexcept { return !this->m_bLeftButton;  }
        inline bool IsRightMouseButtonDown() const noexcept { return this->m_bRightButton;  }
        inline bool IsRightMouseButtonUp()   const noexcept { return !this->m_bRightButton; }

        inline std::int32_t                GetMouseWheelDelta() const noexcept { return this->m_wheelDelta;     }
        inline ::PL::Point2<std::uint16_t> GetCursorPosition()  const noexcept { return this->m_cursorPosition; }

        // Mouse Event Hooking
        inline void OnMouseMove(const std::function<void(const Vec2<std::uint16_t>, const Vec2<std::int16_t>)>& functor) noexcept { this->m_mouseMoveFunctors.push_back(functor); }

        // Keyboard
        inline bool IsKeyDown(const char key) const noexcept { return this->m_downKeys[key];  }
        inline bool IsKeyup  (const char key) const noexcept { return !this->m_downKeys[key]; }

        // Keyboard Event Hooking

#ifdef __POLAR__TARGET_WINDOWS
        inline HWND GetHandle() const noexcept { return this->m_handle; }
#endif // __POLAR__TARGET_WINDOWS

        void Close() noexcept;

        inline ~Window() noexcept { this->Close(); }
    }; // class Window

}; // namespace PL

#endif // __POLAR__FILE_WINDOW_HPP