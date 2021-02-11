#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <memory>

namespace STRB {

	// borrowed from hazel
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}

#include <chrono>
#include <fstream>
#include <SDL.h>
#include <iostream>
#include <SDL_syswm.h>
#include <vector>
#include <wrl/client.h>
#include <windows.h>
#include <d3d11.h>       // D3D interface
#include <dxgi.h>        // DirectX driver interface
#include <d3dcompiler.h> // shader compiler

#include <filesystem>
#include <exception>

namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) : result(hr) {}

        const char* what() const override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X",
                static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
        }
    }
}


#include "graphics/imgui/imgui.h"
#include "graphics/imgui/imgui_impl_dx11.h"
#include "graphics/imgui/imgui_impl_sdl.h"



#include "graphics/dxerr/dxerr.h"


#include "utils/StringConverter.h"
#include "utils/BinaryIO.h"


#include <stdexcept>
#include <DirectXMath.h>
#include <cmath>

#include <SpriteBatch.h>
#include <SpriteFont.h>


#include <SimpleMath.h>

#include "io/formats/Vertex.h"
#include "io/formats/Mesh.h"
#include "io/formats/Model.h"