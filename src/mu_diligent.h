#pragma once

#include <mu_stdlib.h>

namespace mu
{
	// TODO: glfw error type using glfwGetError(const char** description);

	struct diligent_globals
	{
		diligent_globals()
		{
		}

		~diligent_globals()
		{
		}
	};

	struct diligent_window
	{
		std::shared_ptr<diligent_globals>				  m_globals;

		[[nodiscard]] auto create_resources(int sizeX, int sizeY) noexcept -> mu::leaf::result<void>
		try
		{
			return {};
		}
		catch (...)
		{
			return MU_LEAF_NEW_ERROR(mu::gfx_error::not_specified{});
		}

		[[nodiscard]] auto clear() noexcept -> mu::leaf::result<void>
		try
		{
			return {};
		}
		catch (...)
		{
			return MU_LEAF_NEW_ERROR(mu::gfx_error::not_specified{});
		}

		[[nodiscard]] auto present() noexcept -> mu::leaf::result<void>
		try
		{
			return {};
		}
		catch (...)
		{
			return MU_LEAF_NEW_ERROR(mu::gfx_error::not_specified{});
		}

		diligent_window(void* native_wnd, std::shared_ptr<diligent_globals> globals) : m_globals(globals)
		{
		}

		~diligent_window()
		{
			try
			{
				m_globals.reset();
			}
			catch (...)
			{
				MU_LEAF_LOG_ERROR(mu::gfx_error::not_specified{});
			}
		}

		bool ready() const
		{
			return true;
		}
	};
} // namespace mu