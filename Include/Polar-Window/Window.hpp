#ifndef __POLAR__FILE_WINDOW_HPP
#define __POLAR__FILE_WINDOW_HPP

#include <Polar-Build/Polar-Build.hpp>

#ifdef __POLAR__TARGET_WINDOWS
    #include <Windows.h>
#endif // __POLAR__TARGET_WINDOWS

#include <cstdint>

namespace PL {

    namespace details {
#ifdef __POLAR__TARGET_WINDOWS
        LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
#endif // __POLAR__TARGET_WINDOWS
    }; // namespace details

    class Window {
    private:
#ifdef __POLAR__TARGET_WINDOWS
        HWND m_handle = NULL;
#endif // __POLAR__TARGET_WINDOWS

        bool m_bRunning = false;

    public:
        Window() = default;
        Window(const char* title, const std::uint16_t width, const std::uint16_t height) noexcept;

        inline void Show() const noexcept {
#ifdef __POLAR__TARGET_WINDOWS
            ShowWindow(this->m_handle, SW_SHOW);
#endif // __POLAR__TARGET_WINDOWS
        }
        inline void Hide() const noexcept {
#ifdef __POLAR__TARGET_WINDOWS
            ShowWindow(this->m_handle, SW_HIDE);
#endif // __POLAR__TARGET_WINDOWS
        }

        void Update() noexcept;

        inline bool IsRunning() const noexcept { return this->m_bRunning; }

#ifdef __POLAR__TARGET_WINDOWS
        inline HWND GetHandle() const noexcept { return this->m_handle;   }
#endif // __POLAR__TARGET_WINDOWS

        inline void Close() noexcept {
#ifdef __POLAR__TARGET_WINDOWS
            CloseWindow(this->m_handle);

            this->m_bRunning = false;
#endif // __POLAR__TARGET_WINDOWS
        }

        inline ~Window() noexcept { this->Close(); }
    }; // class Window

}; // namespace PL

#endif // __POLAR__FILE_WINDOW_HPP