#include <mu_stdlib.h>

#include <memory>

#include <imgui.h>

struct ImDrawData;

namespace Diligent
{
	struct imgui_shared_resources
	{
		imgui_shared_resources(float scale);

		auto invalidate_device_objects() noexcept -> mu::leaf::result<void>;
		auto invalidate_font_objects() noexcept -> mu::leaf::result<void>;
		auto create_device_objects(float scale, bool force) noexcept -> mu::leaf::result<void>;
		auto create_device_objects() noexcept -> mu::leaf::result<void>;
		auto create_fonts_texture(float scale) noexcept -> mu::leaf::result<void>;

		float m_scale = 1.0f;
	};

	struct imgui_renderer
	{
		imgui_renderer(std::shared_ptr<imgui_shared_resources> shared_resources, float scale);

		~imgui_renderer();

		[[nodiscard]] auto render_draw_data(int render_surface_width, int render_surface_height, ImDrawData* draw_data) noexcept -> mu::leaf::result<void>;

		std::shared_ptr<imgui_shared_resources> m_shared_resources;
	};
} // namespace Diligent
