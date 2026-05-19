- C++ library written in C style
- C++ comments are used in the code
- namespace is called ncore
- predefined system types; u8, u16, u32, u64, s8, s16, s32, s64, f32, f64
- no classes and virtual functions
- struct types are postfixed with _t
- enum types, enum { ... } are postfixed with _e
- function pointers are used for functions and callbacks and are postfixed with _fn
- members are prefixed with m_
- core of the library is allocation-less

Window Library
- Window Management
  CreateWindow(const char* title, int width, int height)
  DestroyWindow()
  ShowWindow()
  HideWindow()
  SetWindowTitle(const char* title)
  GetWindowWidth()
  GetWindowHeight()
- DPI Awareness
  SetDPIAwareness(bool enabled)
  GetDPIAwareness()
  GetDPIScaleFactor()
- Event Handling
  PollEvents()
  GetEventType()
- Resize Handling
  SetResizeCallback(void (*callback)(int newWidth, int newHeight))
- Surface Management (Metal & DirectX)
  CreateSurface(int width, int height)
  DestroySurface()
  GetSurfaceWidth()
  GetSurfaceHeight()

