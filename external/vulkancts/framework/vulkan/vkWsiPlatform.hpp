#ifndef _VKWSIPLATFORM_HPP
#define _VKWSIPLATFORM_HPP
/*-------------------------------------------------------------------------
 * Vulkan CTS Framework
 * --------------------
 *
 * Copyright (c) 2016 Google Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice(s) and this permission notice shall be
 * included in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *//*!
 * \file
 * \brief WSI Platform Abstraction.
 *//*--------------------------------------------------------------------*/

#include "vkDefs.hpp"
#include "tcuVector.hpp"
#include "tcuMaybe.hpp"

namespace vk
{
namespace wsi
{

class Window
{
public:
	virtual				~Window			(void) {}

	virtual void		resize			(const tcu::UVec2& newSize);

protected:
						Window			(void) {}

private:
						Window			(const Window&); // Not allowed
	Window&				operator=		(const Window&); // Not allowed
};

class Display
{
public:
	virtual				~Display		(void) {}

	virtual Window*		createWindow	(const tcu::Maybe<tcu::UVec2>& initialSize = tcu::nothing<tcu::UVec2>()) const = 0;

protected:
						Display			(void) {}

private:
						Display			(const Display&); // Not allowed
	Display&			operator=		(const Display&); // Not allowed
};

// WSI implementation-specific APIs

template<int WsiType>
struct TypeTraits;
// {
//		typedef <NativeDisplayType>	NativeDisplayType;
//		typedef <NativeWindowType>	NativeWindowType;
// };

template<int WsiType>
struct DisplayInterface : public Display
{
public:
	typedef typename TypeTraits<WsiType>::NativeDisplayType	NativeType;

	NativeType			getNative			(void) const { return m_native; }

protected:
						DisplayInterface	(NativeType nativeDisplay)
							: m_native(nativeDisplay)
						{}

	const NativeType	m_native;
};

template<int WsiType>
struct WindowInterface : public Window
{
public:
	typedef typename TypeTraits<WsiType>::NativeWindowType	NativeType;

	NativeType			getNative			(void) const { return m_native; }

protected:
						WindowInterface	(NativeType nativeDisplay)
							: m_native(nativeDisplay)
						{}

	const NativeType	m_native;
};

// VK_KHR_xlib_surface

template<>
struct TypeTraits<TYPE_XLIB>
{
	typedef pt::XlibDisplayPtr			NativeDisplayType;
	typedef pt::XlibWindow				NativeWindowType;
};

typedef DisplayInterface<TYPE_XLIB>		XlibDisplayInterface;
typedef WindowInterface<TYPE_XLIB>		XlibWindowInterface;

// VK_KHR_xcb_surface

template<>
struct TypeTraits<TYPE_XCB>
{
	typedef pt::XcbConnectionPtr		NativeDisplayType;
	typedef pt::XcbWindow				NativeWindowType;
};

typedef DisplayInterface<TYPE_XCB>		XcbDisplayInterface;
typedef WindowInterface<TYPE_XCB>		XcbWindowInterface;

// VK_KHR_wayland_surface

template<>
struct TypeTraits<TYPE_WAYLAND>
{
	typedef pt::WaylandDisplayPtr		NativeDisplayType;
	typedef pt::WaylandSurfacePtr		NativeWindowType;
};

typedef DisplayInterface<TYPE_WAYLAND>	WaylandDisplayInterface;
typedef WindowInterface<TYPE_WAYLAND>	WaylandWindowInterface;

// VK_KHR_mir_surface

template<>
struct TypeTraits<TYPE_MIR>
{
	typedef pt::MirConnectionPtr		NativeDisplayType;
	typedef pt::MirSurfacePtr			NativeWindowType;
};

typedef DisplayInterface<TYPE_MIR>		MirDisplayInterface;
typedef WindowInterface<TYPE_MIR>		MirWindowInterface;

// VK_KHR_android_surface

template<>
struct TypeTraits<TYPE_ANDROID>
{
	typedef pt::AndroidNativeWindowPtr	NativeWindowType;
};

typedef WindowInterface<TYPE_ANDROID>	AndroidWindowInterface;

// VK_KHR_win32_surface

template<>
struct TypeTraits<TYPE_WIN32>
{
	typedef pt::Win32InstanceHandle		NativeDisplayType;
	typedef pt::Win32WindowHandle		NativeWindowType;
};

typedef DisplayInterface<TYPE_WIN32>	Win32DisplayInterface;
typedef WindowInterface<TYPE_WIN32>		Win32WindowInterface;

} // wsi
} // vk

#endif // _VKWSIPLATFORM_HPP
