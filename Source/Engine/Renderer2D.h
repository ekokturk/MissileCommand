// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include <Engine/IRenderer.h>
#include <Graphics/Sampler.h>

namespace Zeron
{
	namespace Gfx
	{
		class PipelineBinding;
		class Pipeline;
		class ShaderProgram;
		class Buffer;
		class CommandBuffer;
		class RenderPass;
		class Graphics;
	}
	namespace Render
	{
		class MeshInfo;
		class FontAtlas;
		class Camera;
	}
}

class Renderer2D : public IRenderer {
	struct Vertex {
		Zeron::Vec3 mPos;
		Zeron::Vec2 mUV;
		Zeron::Color mColor = Zeron::Color::White;
	};

	struct BatchData {
		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;

		void Clear()
		{
			mVertices.clear();
			mIndices.clear();
		}
	};

  public:
	Renderer2D(Zeron::Gfx::Graphics& graphics, Zeron::Gfx::RenderPass& renderPass);
	~Renderer2D();

	void PrepareBatch();
	void RenderBatched(Zeron::Gfx::CommandBuffer& cmd, const Zeron::Render::Camera& camera);

	// IRenderer
	void DrawLine(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& color, int layer) override;
	void DrawBox(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& color, int layer) override;
	void DrawLineBox(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& color, int layer) override;
	void DrawCircle(const Zeron::Vec2& center, float radius, const Zeron::Color& color, int layer) override;
	void DrawLineCircle(const Zeron::Vec2& center, float radius, const Zeron::Color& color, int layer) override;
	int LoadFont(const char* file, int fontSize) override;
	void DrawText(int fontId, const Zeron::Vec2& pos, const char* text, const Zeron::Color& color, int layer) override;

  private:
	void _clearBatches();

	void _drawLine(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& color, int layer);
	void _drawBox(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& color, int layer, bool isSolid);
	void _drawCircle(const Zeron::Vec2& center, float radius, const Zeron::Color& color, int layer, bool isSolid);
	void _drawText(int fontId, const Zeron::Vec2& pos, const char* text, const Zeron::Color& color, int layer);

	float _getFontSizeModifier(int fontId) const;

	Zeron::Gfx::Graphics& mGraphics;

	Zeron::Mat4 mProjection;
	BatchData mSolidBatch;
	BatchData mLineBatch;

	std::unique_ptr<Zeron::Gfx::Buffer> mVertexBuffer;
	std::unique_ptr<Zeron::Gfx::Buffer> mIndexBuffer;
	std::unique_ptr<Zeron::Gfx::Buffer> mViewUniformBuffer;

	std::unique_ptr<Zeron::Gfx::ShaderProgram> mShaderProgram;
	std::unique_ptr<Zeron::Gfx::Pipeline> mPipelineSolid;
	std::unique_ptr<Zeron::Gfx::Pipeline> mPipelineLine;
	std::unique_ptr<Zeron::Gfx::PipelineBinding> mBinding;

	std::unique_ptr<Zeron::Render::FontAtlas> mFontAtlas;
	float mAtlasFontSize;
	std::unique_ptr<Zeron::Gfx::Sampler> mSampler;
};