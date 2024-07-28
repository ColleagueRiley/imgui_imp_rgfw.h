/*
    dear imgui RGFW backend
    This needs to be used along with a Renderer (e.g. OpenGL3, Vulkan, WebGPU..)
*/ 

#pragma once

#ifndef RGFW_IMGUI_H
#include "imgui.h"      // IMGUI_IMPL_API

#if !defined(u8)
	#if defined(_MSC_VER) || defined(__SYMBIAN32__) /* MSVC might not have stdint.h */
		typedef unsigned char 	u8;
		typedef signed long		i64;
	#else /* use stdint standard types instead of c ""standard"" types */
		#include <stdint.h>

		typedef uint8_t     u8;
		typedef uint32_t   u32;
	#endif
#endif

#ifndef IMGUI_DISABLE

#define RGFW_IMGUI_H

struct RGFW_window;

/* basic api */
IMGUI_IMPL_API bool     ImGui_ImplRgfw_InitForOpenGL(RGFW_window* window, bool install_callbacks);
IMGUI_IMPL_API bool     ImGui_ImplRgfw_InitForVulkan(RGFW_window* window, bool install_callbacks);
IMGUI_IMPL_API bool     ImGui_ImplRgfw_InitForOther(RGFW_window* window, bool install_callbacks);
IMGUI_IMPL_API void     ImGui_ImplRgfw_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplRgfw_NewFrame();

// RGFW callbacks install
// - When calling Init with 'install_callbacks=true': ImGui_ImplRgfw_InstallCallbacks() is called. RGFW callbacks will be installed for you. They will chain-call user's previously installed callbacks, if any.
// - When calling Init with 'install_callbacks=false': RGFW callbacks won't be installed. You will need to call individual function yourself from your own RGFW callbacks.
IMGUI_IMPL_API void     ImGui_ImplRgfw_InstallCallbacks(RGFW_window* window);
IMGUI_IMPL_API void     ImGui_ImplRgfw_RestoreCallbacks(RGFW_window* window);

// RGFW callbacks options:
// - Set 'chain_for_all_windows=true' to enable chaining callbacks for all windows (including secondary viewports created by backends or by user)
IMGUI_IMPL_API void     ImGui_ImplRgfw_SetCallbacksChainForAllWindows(bool chain_for_all_windows);

typedef struct {int x; int y;} impoint;
#define RGFW_point impoint

// RGFW callbacks (individual callbacks to call yourself if you didn't install callbacks)
IMGUI_IMPL_API void     ImGui_ImplRgfw_WindowFocusCallback(RGFW_window* window, u8 inFocus);        // Since 1.84
IMGUI_IMPL_API void     ImGui_ImplRgfw_CursorEnterCallback(RGFW_window* window, RGFW_point point, u8 status);        // Since 1.84
IMGUI_IMPL_API void     ImGui_ImplRgfw_CursorPosCallback(RGFW_window* window, RGFW_point p);   // Since 1.87
IMGUI_IMPL_API void     ImGui_ImplRgfw_MouseButtonCallback(RGFW_window* window, u8 button, double scroll, u8 pressed);
IMGUI_IMPL_API void     ImGui_ImplRgfw_ScrollCallback(RGFW_window* window, double xoffset, double yoffset);
IMGUI_IMPL_API void     ImGui_ImplRgfw_KeyCallback(RGFW_window* window, u32 keycode, char keyName[16], u8 lockState, u8 pressed);
IMGUI_IMPL_API void     ImGui_ImplRgfw_CharCallback(RGFW_window* window, unsigned int c);
#endif /* ifndef RGFW_IMGUI_H */

#ifdef RGFW_IMGUI_IMPLEMENTATION

#define RGFWDEF
#include "RGFW.h"

// RGFW data
enum RgfwClientApi
{
    RgfwClientApi_Unknown,
    RgfwClientApi_OpenGL,
    RgfwClientApi_Vulkan,
};

struct ImGui_ImplRgfw_Data
{
    RGFW_window*             Window;
    RgfwClientApi           ClientApi;
    double                  Time;
    RGFW_window*             MouseWindow;
    ImVec2                  LastValidMousePos;
    bool                    InstalledCallbacks;
    bool                    CallbacksChainForAllWindows;

    // Chain RGFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
    RGFW_focusfunc      PrevUserCallbackWindowFocus;
    RGFW_mouseposfunc        PrevUserCallbackCursorPos;
    RGFW_mouseNotifyfunc      PrevUserCallbackCursorEnter;
    RGFW_mousebuttonfunc      PrevUserCallbackMousebutton;
    RGFW_keyfunc              PrevUserCallbackKey;

    ImGui_ImplRgfw_Data()   { memset((void*)this, 0, sizeof(*this)); }
};

static ImGui_ImplRgfw_Data* ImGui_ImplRgfw_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplRgfw_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
}

char* clipboard_str = nullptr;

// Functions
static const char* ImGui_ImplRgfw_GetClipboardText(void* user_data)
{
    RGFW_UNUSED(user_data);
    if (clipboard_str != nullptr)
        free(clipboard_str);
    
    size_t size;
    clipboard_str = RGFW_readClipboard(&size);

    return (const char*)clipboard_str;
}

static void ImGui_ImplRgfw_SetClipboardText(void* user_data, const char* text)
{
    RGFW_UNUSED(user_data);
    RGFW_UNUSED(text);
    //RGFW_writeClipboard(text, strlen(text));
RGFW_writeClipboard("DOWN", 4);
}

static ImGuiKey ImGui_ImplRgfw_KeyToImGuiKey(int key)
{
    static const ImGuiKey map[] = {
        ImGuiKey_None,
        ImGuiKey_Escape,
        ImGuiKey_F1,
        ImGuiKey_F2,
        ImGuiKey_F3,
        ImGuiKey_F4,
        ImGuiKey_F5,
        ImGuiKey_F6,
        ImGuiKey_F7,
        ImGuiKey_F8,
        ImGuiKey_F9,
        ImGuiKey_F10,
        ImGuiKey_F11,
        ImGuiKey_F12,
        ImGuiKey_GraveAccent,
        ImGuiKey_0,
        ImGuiKey_1,
        ImGuiKey_2,
        ImGuiKey_3,
        ImGuiKey_4,
        ImGuiKey_5,
        ImGuiKey_6,
        ImGuiKey_7,
        ImGuiKey_8,
        ImGuiKey_9,
        ImGuiKey_Minus,
        ImGuiKey_Equal,
        ImGuiKey_Backspace,
        ImGuiKey_Tab,
        ImGuiKey_CapsLock,
        ImGuiKey_LeftShift,
        ImGuiKey_LeftCtrl,
        ImGuiKey_LeftAlt,
        ImGuiKey_LeftSuper,
        ImGuiKey_RightShift,
        ImGuiKey_RightCtrl,
        ImGuiKey_RightAlt,
        ImGuiKey_RightSuper,
        ImGuiKey_Space,
        ImGuiKey_A,
        ImGuiKey_B,
        ImGuiKey_C,
        ImGuiKey_D,
        ImGuiKey_E,
        ImGuiKey_F,
        ImGuiKey_G,
        ImGuiKey_H,
        ImGuiKey_I,
        ImGuiKey_J,
        ImGuiKey_K,
        ImGuiKey_L,
        ImGuiKey_M,
        ImGuiKey_N,
        ImGuiKey_O,
        ImGuiKey_P,
        ImGuiKey_Q,
        ImGuiKey_R,
        ImGuiKey_S,
        ImGuiKey_T,
        ImGuiKey_U,
        ImGuiKey_V,
        ImGuiKey_W,
        ImGuiKey_X,
        ImGuiKey_Y,
        ImGuiKey_Z,
        ImGuiKey_Period,
        ImGuiKey_Comma,
        ImGuiKey_Slash,
        ImGuiKey_LeftBracket,
        ImGuiKey_RightBracket,
        ImGuiKey_Semicolon,
        ImGuiKey_Enter,
        ImGuiKey_Apostrophe,
        ImGuiKey_Backslash,
        ImGuiKey_UpArrow,
        ImGuiKey_DownArrow,
        ImGuiKey_LeftArrow,
        ImGuiKey_RightArrow,
        ImGuiKey_Delete,
        ImGuiKey_Insert,
        ImGuiKey_End,
        ImGuiKey_Home,
        ImGuiKey_PageUp,
        ImGuiKey_PageDown,
        ImGuiKey_NumLock,
        ImGuiKey_KeypadDivide,
        ImGuiKey_KeypadMultiply,
        ImGuiKey_KeypadSubtract,
        ImGuiKey_Keypad1,
        ImGuiKey_Keypad2,
        ImGuiKey_Keypad3,
        ImGuiKey_Keypad4,
        ImGuiKey_Keypad5,
        ImGuiKey_Keypad6,
        ImGuiKey_Keypad7,
        ImGuiKey_Keypad8,
        ImGuiKey_Keypad9,
        ImGuiKey_Keypad0,
        ImGuiKey_KeypadDecimal,
        ImGuiKey_KeypadEnter,
    };

    return map[key];
}

static void ImGui_ImplRgfw_UpdateKeyModifiers(RGFW_window* window)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(ImGuiMod_Ctrl,  (RGFW_isPressed(window, RGFW_ControlL) == RGFW_TRUE) || (RGFW_isPressed(window, RGFW_ControlL) == RGFW_TRUE));
    io.AddKeyEvent(ImGuiMod_Shift, (RGFW_isPressed(window, RGFW_ShiftL)   == RGFW_TRUE) || (RGFW_isPressed(window, RGFW_ShiftR)   == RGFW_TRUE));
    io.AddKeyEvent(ImGuiMod_Alt,   (RGFW_isPressed(window, RGFW_AltL)     == RGFW_TRUE) || (RGFW_isPressed(window, RGFW_AltR)     == RGFW_TRUE));
    io.AddKeyEvent(ImGuiMod_Super, (RGFW_isPressed(window, RGFW_SuperL)   == RGFW_TRUE) || (RGFW_isPressed(window, RGFW_SuperR)   == RGFW_TRUE));
}

static bool ImGui_ImplRgfw_ShouldChainCallback(RGFW_window* window)
{
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    return bd->CallbacksChainForAllWindows ? true : (window == bd->Window);
}

void ImGui_ImplRgfw_MouseButtonCallback(RGFW_window* window, u8 button, double scroll, u8 pressed)
{
    if (button >= RGFW_mouseScrollUp) {
        return ImGui_ImplRgfw_ScrollCallback(window, 0, scroll);
    }
    

    if (button == RGFW_mouseMiddle) button = RGFW_mouseRight;
    else if (button == RGFW_mouseRight) button = RGFW_mouseMiddle;
    button--;

    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    if (bd->PrevUserCallbackMousebutton != nullptr && ImGui_ImplRgfw_ShouldChainCallback(window))
        bd->PrevUserCallbackMousebutton(window, button, scroll, pressed);

    ImGui_ImplRgfw_UpdateKeyModifiers(window);

    ImGuiIO& io = ImGui::GetIO();
    if (button < ImGuiMouseButton_COUNT) {
        io.AddMouseButtonEvent(button, pressed);
    }
}

void ImGui_ImplRgfw_ScrollCallback(RGFW_window* window, double xoffset, double yoffset)
{
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    if (bd->PrevUserCallbackMousebutton != nullptr && ImGui_ImplRgfw_ShouldChainCallback(window))
        bd->PrevUserCallbackMousebutton(window, RGFW_mouseScrollUp + (yoffset > 0), yoffset, RGFW_TRUE);

    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent((float)xoffset, (float)yoffset);
}

char RGFW_keyCodeToChar(u32 keyCode, b8 caps) {
    static const char map[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        '`', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', 
        '9', '-', '=', 0, '\t', 
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
        'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
        'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
        'x', 'y', 'z', '.', ',', '/', '[', ']', 
        ';', '\n', '\'', '\\', 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        '/', '*', '-', '1', '2', '3', 
        '3', '5', '6', '7', '8',  '9', '0', '\n'
    };

    static const char mapCaps[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        '~', ')', '!', '@', '#',
        '$', '%', '^', '&', '*', 
        '(', '_', '+', 0, '0', 
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
        'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
        'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
        'X', 'Y', 'Z', '>', '<', '?', '{', '}', 
        ':', '\n', '"', '|', 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        '?', '*', '-', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    if (caps == RGFW_FALSE)
        return map[keyCode];
    
    return mapCaps[keyCode];
}

void ImGui_ImplRgfw_KeyCallback(RGFW_window* window, u32 keycode, char keyName[16], u8 lockState, b8 pressed)
{
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    if (bd->PrevUserCallbackKey != nullptr && ImGui_ImplRgfw_ShouldChainCallback(window))
        bd->PrevUserCallbackKey(window, keycode, keyName, lockState, pressed);
    
    ImGui_ImplRgfw_UpdateKeyModifiers(window);
    
    #define RGFW_xor(x, y) (( (x) && (!(y)) ) ||  ((y) && (!(x)) ))
    b8 caps4caps = (lockState & RGFW_CAPSLOCK) && ((keycode >= RGFW_a) && (keycode <= RGFW_z));
    char ch = RGFW_keyCodeToChar(keycode, RGFW_xor((RGFW_isPressed(window, RGFW_ShiftL) || RGFW_isPressed(window, RGFW_ShiftR)), caps4caps));
    #undef RGFW_xor

    if (pressed == RGFW_TRUE)
        ImGui_ImplRgfw_CharCallback(window, ch);

    ImGuiIO& io = ImGui::GetIO();
    ImGuiKey imgui_key = ImGui_ImplRgfw_KeyToImGuiKey(keycode);
    io.AddKeyEvent(imgui_key, pressed);
    io.SetKeyEventNativeData(imgui_key, keycode, keycode); // To support legacy indexing (<1.87 user code)
}

void ImGui_ImplRgfw_WindowFocusCallback(RGFW_window* window, b8 inFocus)
{
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    if (bd->PrevUserCallbackWindowFocus != nullptr && ImGui_ImplRgfw_ShouldChainCallback(window))
        bd->PrevUserCallbackWindowFocus(window, inFocus);

    ImGuiIO& io = ImGui::GetIO();
    io.AddFocusEvent(inFocus != 0);
}

void ImGui_ImplRgfw_CursorPosCallback(RGFW_window* window, RGFW_point p)
{
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    if (bd->PrevUserCallbackCursorPos != nullptr && ImGui_ImplRgfw_ShouldChainCallback(window))
        bd->PrevUserCallbackCursorPos(window, p);

    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)p.x, (float)p.y);
    bd->LastValidMousePos = ImVec2((float)p.x, (float)p.y);
}

// Workaround: X11 seems to send spurious Leave/Enter events which would make us lose our position,
// so we back it up and restore on Leave/Enter (see https://github.com/ocornut/imgui/issues/4984)
void ImGui_ImplRgfw_CursorEnterCallback(RGFW_window* window, RGFW_point point, b8 status)
{
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    if (bd->PrevUserCallbackCursorEnter != nullptr && ImGui_ImplRgfw_ShouldChainCallback(window))
        bd->PrevUserCallbackCursorEnter(window, point, status);

    ImGuiIO& io = ImGui::GetIO();
    if (status)
    {
        bd->MouseWindow = window;
        io.AddMousePosEvent(bd->LastValidMousePos.x, bd->LastValidMousePos.y);
    }
    else if (!status && bd->MouseWindow == window)
    {
        bd->LastValidMousePos = io.MousePos;
        bd->MouseWindow = nullptr;
        io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
    }
}

void ImGui_ImplRgfw_CharCallback(RGFW_window* window, unsigned int c)
{
    RGFW_UNUSED(window);
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(c);
}

void ImGui_ImplRgfw_InstallCallbacks(RGFW_window* window)
{
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    IM_ASSERT(bd->InstalledCallbacks == false && "Callbacks already installed!");
    IM_ASSERT(bd->Window == window);

    /* 
        TODO: RGFW doesn't have anyway to do this yet
        update this when I add it to RGFW
    */

    bd->PrevUserCallbackWindowFocus = RGFW_setFocusCallback(ImGui_ImplRgfw_WindowFocusCallback);
    bd->PrevUserCallbackCursorEnter = RGFW_setMouseNotifyCallBack(ImGui_ImplRgfw_CursorEnterCallback);
    bd->PrevUserCallbackCursorPos = RGFW_setMousePosCallback(ImGui_ImplRgfw_CursorPosCallback);
    bd->PrevUserCallbackMousebutton = RGFW_setMouseButtonCallback(ImGui_ImplRgfw_MouseButtonCallback);
    bd->PrevUserCallbackKey = RGFW_setKeyCallback(ImGui_ImplRgfw_KeyCallback);
    bd->InstalledCallbacks = true;
}

void ImGui_ImplRgfw_RestoreCallbacks(RGFW_window* window)
{
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    IM_ASSERT(bd->InstalledCallbacks == true && "Callbacks not installed!");
    IM_ASSERT(bd->Window == window);

    RGFW_setFocusCallback(bd->PrevUserCallbackWindowFocus);
    RGFW_setMouseNotifyCallBack(bd->PrevUserCallbackCursorEnter);
    RGFW_setMousePosCallback(bd->PrevUserCallbackCursorPos);
    RGFW_setMouseButtonCallback(bd->PrevUserCallbackMousebutton);
    RGFW_setKeyCallback(bd->PrevUserCallbackKey);
    bd->InstalledCallbacks = false;
    bd->PrevUserCallbackWindowFocus = nullptr;
    bd->PrevUserCallbackCursorEnter = nullptr;
    bd->PrevUserCallbackCursorPos = nullptr;
    bd->PrevUserCallbackMousebutton = nullptr;
    bd->PrevUserCallbackKey = nullptr;
}

// Set to 'true' to enable chaining installed callbacks for all windows (including secondary viewports created by backends or by user.
// This is 'false' by default meaning we only chain callbacks for the main viewport.
// We cannot set this to 'true' by default because user callbacks code may be not testing the 'window' parameter of their callback.
// If you set this to 'true' your user callback code will need to make sure you are testing the 'window' parameter.
void ImGui_ImplRgfw_SetCallbacksChainForAllWindows(bool chain_for_all_windows)
{
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    bd->CallbacksChainForAllWindows = chain_for_all_windows;
}

#ifdef __EMSCRIPTEN__
EM_JS(void, ImGui_ImplRgfw_EmscriptenOpenURL, (char const* url), { url = url ? UTF8ToString(url) : null; if (url) window.open(url, '_blank'); });
#endif

static bool ImGui_ImplRgfw_Init(RGFW_window* window, bool install_callbacks, RgfwClientApi client_api)
{
    ImGuiIO& io = ImGui::GetIO();
    IMGUI_CHECKVERSION();
    IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");

    // Setup backend capabilities flags
    ImGui_ImplRgfw_Data* bd = IM_NEW(ImGui_ImplRgfw_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_rgfw";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)

    bd->Window = window;
    bd->Time = 0.0;

    io.SetClipboardTextFn = ImGui_ImplRgfw_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplRgfw_GetClipboardText;
    io.ClipboardUserData = bd->Window;
#ifdef __EMSCRIPTEN__
      // io.PlatformOpenInShellFn = [](ImGuiContext*, const char* url) { ImGui_ImplRgfw_EmscriptenOpenURL(url); return true; };
#endif
    // Chain RGFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
    if (install_callbacks)
        ImGui_ImplRgfw_InstallCallbacks(window);

    // Set platform dependent data in viewport
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    main_viewport->PlatformHandle = (void*)bd->Window;
#if defined(_WIN32) || defined(__APPLE__)
    main_viewport->PlatformHandleRaw = bd->Window->src.window;
#else
    IM_UNUSED(main_viewport);
#endif

    bd->ClientApi = client_api;
    return true;
}

bool ImGui_ImplRgfw_InitForOpenGL(RGFW_window* window, bool install_callbacks)
{
    return ImGui_ImplRgfw_Init(window, install_callbacks, RgfwClientApi_OpenGL);
}

bool ImGui_ImplRgfw_InitForVulkan(RGFW_window* window, bool install_callbacks)
{
    return ImGui_ImplRgfw_Init(window, install_callbacks, RgfwClientApi_Vulkan);
}

bool ImGui_ImplRgfw_InitForOther(RGFW_window* window, bool install_callbacks)
{
    return ImGui_ImplRgfw_Init(window, install_callbacks, RgfwClientApi_Unknown);
}

void ImGui_ImplRgfw_Shutdown()
{
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    IM_ASSERT(bd != nullptr && "No platform backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();

    if (bd->InstalledCallbacks)
        ImGui_ImplRgfw_RestoreCallbacks(bd->Window);
    
    if (clipboard_str != nullptr) /* free clipboard if it was used */
        free(clipboard_str);
    
    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;
    io.BackendFlags &= ~(ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos | ImGuiBackendFlags_HasGamepad);
    IM_DELETE(bd);
}

static void ImGui_ImplRgfw_UpdateMouseData()
{
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    ImGuiIO& io = ImGui::GetIO();

    // (those braces are here to reduce diff with multi-viewports support in 'docking' branch)
    {
        RGFW_window* window = bd->Window;
        const bool is_window_focused = window->event.inFocus;
        if (is_window_focused)
        {
            // (Optional) Set OS mouse position from Dear ImGui if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
            if (io.WantSetMousePos)
                RGFW_window_moveMouse(window, RGFW_POINT((i32)io.MousePos.x, (i32)io.MousePos.y));

            // (Optional) Fallback to provide mouse position when focused (ImGui_ImplRgfw_CursorPosCallback already provides this when hovered or captured)
            if (bd->MouseWindow == nullptr)
            {
                RGFW_point point = RGFW_window_getMousePoint(window);
                bd->LastValidMousePos = ImVec2((float)point.x, (float)point.y);
                io.AddMousePosEvent((float)point.x, (float)point.y);
            }
        }
    }
}

static void ImGui_ImplRgfw_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || (bd->Window->_winArgs & (1L<<2)))
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    // (those braces are here to reduce diff with multi-viewports support in 'docking' branch)
    {
        RGFW_window* window = bd->Window;
        if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
        {
            // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
            RGFW_window_showMouse(window, 0);
        }
        else
        {
            // Show OS mouse cursor
            // FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with RGFW 3.2, but 3.3 works here.
            RGFW_window_setMouseStandard(window, (imgui_cursor + 1));
            
            RGFW_window_showMouse(window, 1);
        }
    }
}

// Update gamepad inputs
static inline float Saturate(float v) { return v < 0.0f ? 0.0f : v  > 1.0f ? 1.0f : v; }
static void ImGui_ImplRgfw_UpdateGamepads()
{
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0) // FIXME: Technically feeding gamepad shouldn't depend on this now that they are regular inputs.
        return;

    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    IM_ASSERT(bd != nullptr && "Context or backend not initialized! Did you call ImGui_ImplRgfw_InitForXXX()?");

    io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
    int axes_count = bd->Window->event.axisesCount;
    int buttons_count = 8;

    if (axes_count == 0 || buttons_count == 0)
        return;
    #define MAP_BUTTON(KEY_NO, _UNUSED, BUTTON_NO)          do { io.AddKeyEvent(KEY_NO, (buttons_count > BUTTON_NO && RGFW_isPressedJS(bd->Window, 1, BUTTON_NO) == RGFW_TRUE)); } while (0)
    #define MAP_ANALOG(KEY_NO, _UNUSED, AXIS_NO, V0, V1)    do { float v = (axes_count > AXIS_NO) ? axes[AXIS_NO] : V0; v = (v - V0) / (V1 - V0); io.AddKeyAnalogEvent(KEY_NO, v > 0.10f, Saturate(v)); } while (0)
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    MAP_BUTTON(ImGuiKey_GamepadStart,       RGFW_JS_START,          9);
    MAP_BUTTON(ImGuiKey_GamepadBack,        RGFW_JS_SELECT,           8);
    MAP_BUTTON(ImGuiKey_GamepadFaceLeft,    RGFW_JS_Y,              2);     // Xbox X, PS Square
    MAP_BUTTON(ImGuiKey_GamepadFaceRight,   RGFW_JS_B,              1);     // Xbox B, PS Circle
    MAP_BUTTON(ImGuiKey_GamepadFaceUp,      RGFW_JS_X,              3);     // Xbox Y, PS Triangle
    MAP_BUTTON(ImGuiKey_GamepadFaceDown,    RGFW_JS_A,              0);     // Xbox A, PS Cross
    MAP_BUTTON(ImGuiKey_GamepadDpadLeft,    RGFW_JS_LEFT,      15);
    MAP_BUTTON(ImGuiKey_GamepadDpadRight,   RGFW_JS_RIGHT,     16);
    MAP_BUTTON(ImGuiKey_GamepadDpadUp,      RGFW_JS_UP,        13);
    MAP_BUTTON(ImGuiKey_GamepadDpadDown,    RGFW_JS_DOWN,      14);
    MAP_BUTTON(ImGuiKey_GamepadL1,          RGFW_JS_L1,    4);
    MAP_BUTTON(ImGuiKey_GamepadR1,          RGFW_JS_R1,   6);
    #undef MAP_BUTTON
    #undef MAP_ANALOG
}

void ImGui_ImplRgfw_NewFrame()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplRgfw_Data* bd = ImGui_ImplRgfw_GetBackendData();
    IM_ASSERT(bd != nullptr && "Context or backend not initialized! Did you call ImGui_ImplRgfw_InitForXXX()?");

    // Setup display size (every frame to accommodate for window resizing)
    RGFW_rect size = bd->Window->r;
    io.DisplaySize = ImVec2((float)size.w, (float)size.h);

    // Setup time step
    // (Accept RGFW_getTime() not returning a monotonically increasing value. Seems to happens on disconnecting peripherals and probably on VMs and Emscripten, see #6491, #6189, #6114, #3644)
    double current_time = RGFW_getTime();
    if (current_time <= bd->Time)
        current_time = bd->Time + 0.00001f;
    io.DeltaTime = bd->Time > 0.0 ? (float)(current_time - bd->Time) : (float)(1.0f / 60.0f);
    bd->Time = current_time;

    ImGui_ImplRgfw_UpdateMouseData();
    ImGui_ImplRgfw_UpdateMouseCursor();

    // Update game controllers (if enabled and available)
    ImGui_ImplRgfw_UpdateGamepads();
}

//-----------------------------------------------------------------------------

#endif // #ifndef IMGUI_DISABLE

#endif /* RGFW_IMGUI_IMPLEMENTATION */