#include <mu_stdlib.h>

#include <memory>

#include <Primitives/interface/BasicTypes.h>
#include <Common/interface/BasicMath.hpp>
#include <Common/interface/RefCntAutoPtr.hpp>
#include <Graphics/GraphicsEngine/interface/GraphicsTypes.h>

#include <imgui.h>

struct ImDrawData;

namespace Diligent
{
	struct IRenderDevice;
	struct IDeviceContext;
	struct IBuffer;
	struct IPipelineState;
	struct ITextureView;
	struct ITexture;
	struct IShader;
	struct IShaderResourceBinding;
	struct IShaderResourceVariable;
	enum TEXTURE_FORMAT : Uint16;
	enum SURFACE_TRANSFORM : Uint32;

	struct imgui_shared_resources
	{
		imgui_shared_resources(IRenderDevice* render_device, TEXTURE_FORMAT back_buffer_fmt, TEXTURE_FORMAT depth_buffer_fmt, float scale);

		auto invalidate_device_objects() noexcept -> mu::leaf::result<void>;
		auto invalidate_font_objects() noexcept -> mu::leaf::result<void>;
		auto create_device_objects(float scale, bool force) noexcept -> mu::leaf::result<void>;
		auto create_device_objects() noexcept -> mu::leaf::result<void>;
		auto create_fonts_texture(float scale) noexcept -> mu::leaf::result<void>;

		RefCntAutoPtr<IRenderDevice>		  m_device;
		RefCntAutoPtr<ITexture>				  m_font_tex;
		RefCntAutoPtr<IBuffer>				  m_vertex_constant_buffer;
		RefCntAutoPtr<IPipelineState>		  m_pso;
		RefCntAutoPtr<ITextureView>			  m_font_srv;
		RefCntAutoPtr<IShaderResourceBinding> m_srb;
		RefCntAutoPtr<IShader>				  m_vs;
		RefCntAutoPtr<IShader>				  m_ps;
		IShaderResourceVariable*			  m_texture_var = nullptr;

		float				 m_scale = 1.0f;
		const TEXTURE_FORMAT m_back_buffer_fmt;
		const TEXTURE_FORMAT m_depth_buffer_fmt;
	};

	struct imgui_renderer
	{
		imgui_renderer(
			IRenderDevice* render_device, TEXTURE_FORMAT back_buffer_fmt, TEXTURE_FORMAT depth_buffer_fmt, Uint32 initial_vertex_buffer_size, Uint32 initial_index_buffer_size,
			float scale);
		
		imgui_renderer(
			std::shared_ptr<imgui_shared_resources> shared_resources, Uint32 initial_vertex_buffer_size, Uint32 initial_index_buffer_size,
			float scale);

		~imgui_renderer();

		[[nodiscard]] auto new_frame(Uint32 render_surface_width, Uint32 render_surface_height, SURFACE_TRANSFORM surface_pretransform, float scale) noexcept
			-> mu::leaf::result<void>;
		[[nodiscard]] auto end_frame() noexcept -> mu::leaf::result<void>;
		[[nodiscard]] auto render_draw_data(IDeviceContext* ctx, ImDrawData* draw_data) noexcept -> mu::leaf::result<void>;
		[[nodiscard]] auto create_device_objects(float scale, bool force) noexcept -> mu::leaf::result<void>;

		inline float4 transform_clip_rect(const ImVec2& display_size, const float4& rect) const;

		std::shared_ptr<imgui_shared_resources> m_shared_resources;

		RefCntAutoPtr<IBuffer> m_vertex_buffer;
		RefCntAutoPtr<IBuffer> m_index_buffer;

		Uint32			  m_vertex_buffer_size	  = 0;
		Uint32			  m_index_buffer_size	  = 0;
		Uint32			  m_render_surface_width  = 0;
		Uint32			  m_render_surface_height = 0;
		SURFACE_TRANSFORM m_surface_pre_transform = SURFACE_TRANSFORM_IDENTITY;
		//float			  m_scale				  = 1.0f;
	};
} // namespace Diligent
