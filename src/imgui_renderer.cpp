/*
 *  Copyright 2019-2021 Diligent Graphics LLC
 *  Copyright 2015-2019 Egor Yusov
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */

#include <cstddef>
#include "imgui_renderer.h"

namespace Diligent
{
	bool m_device_objects_created{false};
	bool m_font_objects_created{false};

	imgui_shared_resources::imgui_shared_resources(float scale) : m_scale(scale) { }

	auto imgui_shared_resources::invalidate_device_objects() noexcept -> mu::leaf::result<void>
	{
		invalidate_font_objects();
		m_device_objects_created = false;
		return {};
	}

	auto imgui_shared_resources::invalidate_font_objects() noexcept -> mu::leaf::result<void>
	{
		m_font_objects_created = false;
		return {};
	}

	auto imgui_shared_resources::create_device_objects(float scale, bool force) noexcept -> mu::leaf::result<void>
	{
		bool scale_matches = scale == m_scale;

		if (!force && m_device_objects_created && m_font_objects_created && scale_matches) [[likely]]
		{
			return {};
		}

		if (force || !m_device_objects_created)
		{
			MU_LEAF_CHECK(invalidate_device_objects());
			MU_LEAF_CHECK(create_device_objects());
		}
		else if (!scale_matches || !m_font_objects_created)
		{
			MU_LEAF_CHECK(invalidate_font_objects());
		}

		MU_LEAF_CHECK(create_fonts_texture(scale));

		return {};
	}

	auto imgui_shared_resources::create_device_objects() noexcept -> mu::leaf::result<void>
	{
		m_device_objects_created = true;

		return {};
	}

	auto imgui_shared_resources::create_fonts_texture(float scale) noexcept -> mu::leaf::result<void>
	{
		if (m_font_objects_created)
		{
			return {};
		}

		m_font_objects_created = true;

		// Build texture atlas
		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->ClearFonts();

		ImFontConfig cfg;
		cfg.SizePixels = 13 * scale;
		io.Fonts->AddFontDefault(&cfg);

		unsigned char* pixels = nullptr;
		int			   width = 0, height = 0;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		m_scale = scale;

		return {};
	}

} // namespace Diligent

namespace Diligent
{
	imgui_renderer::imgui_renderer(std::shared_ptr<imgui_shared_resources> shared_resources, float scale) : m_shared_resources(shared_resources) { }

	imgui_renderer::~imgui_renderer() { }

	auto imgui_renderer::render_draw_data(int render_surface_width, int render_surface_height, ImDrawData* draw_data) noexcept -> mu::leaf::result<void>
	{
		// Avoid rendering when minimized
		if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f)
		{
			return {};
		}

		return {};
	}

} // namespace Diligent
