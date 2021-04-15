#include <mu_gfx.h>

static auto all_error_handlers = std::tuple_cat(mu::error_handlers, mu::only_gfx_error_handlers);

auto update_window(std::shared_ptr<mu::gfx_window>& wnd) noexcept -> mu::leaf::result<mu::gfx_window::renderer_ref>
{
	if (wnd)
	{
		MU_LEAF_AUTO(wants_to_close, wnd->wants_to_close());

		if (!wants_to_close)
		{
			return wnd->begin_window();
		}
		else
		{
			wnd.reset();
		}
	}

	return mu::gfx_window::renderer_ref{};
};

auto draw_window(std::shared_ptr<mu::gfx_window>& wnd, mu::gfx_window::renderer_ref renderer) noexcept -> mu::leaf::result<void>
{
	MU_LEAF_CHECK(renderer->test());

	return {};
};

namespace mu
{
	template<typename T_CONTAINER, typename T_FUNC, typename T_FUNC_RET>
	auto for_some_return(T_CONTAINER& container, T_FUNC func, T_FUNC_RET&& true_return_value, T_FUNC_RET&& false_return_value) noexcept -> T_FUNC_RET
	{
		for (auto& itor : container)
		{
			if (func(itor))
			{
				return true_return_value;
			}
		}

		return false_return_value;
	}

	template<typename T_CONTAINER, typename T_FUNC, typename T_FUNC_RET>
	auto for_some_optional_return(T_CONTAINER& container, T_FUNC func, T_FUNC_RET&& false_return_value) noexcept -> T_FUNC_RET
	{
		for (auto& itor : container)
		{
			if (auto res = func(itor))
			{
				return *res;
			}
		}

		return false_return_value;
	}
} // namespace mu

auto main(int, char**) -> int
{
	if (
		[]() -> mu::leaf::result<void>
		{
			auto logger = mu::debug::logger()->stdout_logger();
			logger->info("Hello world");

			std::vector<std::shared_ptr<mu::gfx_window>> windows;
			MU_LEAF_EMPLACE_BACK(windows, mu::gfx()->open_window(100, 100, 1280, 800));
			MU_LEAF_EMPLACE_BACK(windows, mu::gfx()->open_window(200, 200, 640, 480));

			for (auto& wnd : windows)
			{
				MU_LEAF_CHECK(wnd->show());
			}

			while (mu::for_some_return(
				windows,
				[](std::shared_ptr<mu::gfx_window>& wnd) -> bool
				{
					return wnd ? true : false;
				},
				true,
				false))
			{
				MU_LEAF_AUTO(pumper, mu::gfx()->pump());

				for (auto& wwnd : windows)
				{
					if (wwnd)
					{
						MU_LEAF_AUTO(wants_to_close, wwnd->wants_to_close());

						if (!wants_to_close) [[likely]]
						{
							if (auto wnd = wwnd->begin_window(); wnd) [[likely]]
							{
								if (auto& r = *wnd; r) [[likely]]
								{
									MU_LEAF_CHECK(r->test());
								}
							}
						}
						else
						{
							wwnd.reset();
						}
					}
				}
			}
			return {};
		}())
	{
		return 0;
	}
	else
	{
		return mu::leaf::current_error().value();
	}
}
