// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Engine/Renderer2D.h>

#include <Graphics/Buffer.h>
#include <Graphics/CommandBuffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/Pipeline.h>
#include <Graphics/PipelineBinding.h>
#include <Graphics/ResourceLayout.h>
#include <Graphics/Shader.h>
#include <Graphics/VertexLayout.h>
#include <Platform/FileSystem.h>
#include <Render/Camera/Camera.h>
#include <Render/Font/FontAtlas.h>
#include <Render/Mesh/MeshInfo.h>

namespace
{
	float _getPosFromLayer(int layer)
	{
		// Use Z position for layering
		return 0.5f + 0.01f * static_cast<float>(layer);
	}
}

Renderer2D::Renderer2D(Zeron::Gfx::Graphics& graphics, Zeron::Gfx::RenderPass& renderPass)
	: mGraphics(graphics)
	, mProjection(Zeron::Math::Orthographic(0.f, static_cast<float>(kViewportWidth), static_cast<float>(kViewportHeight), 0.f, 0.f, 1.f))
	, mAtlasFontSize(60)
{
	const auto vertexShaderBuffer = Zeron::FileSystem::ReadBinaryFile(
		Zeron::Path("Resources/Shaders") / mGraphics.GetCompiledShaderName("Shape2D", Zeron::Gfx::ShaderType::Vertex)
	);
	ZE_ASSERT(!vertexShaderBuffer.HasError(), "Unable to load vertex shader!");
	const auto fragmentShaderBuffer = Zeron::FileSystem::ReadBinaryFile(
		Zeron::Path("Resources/Shaders") / mGraphics.GetCompiledShaderName("Shape2D", Zeron::Gfx::ShaderType::Fragment)
	);
	ZE_ASSERT(!fragmentShaderBuffer.HasError(), "Unable to load fragment shader!");

	auto font = Zeron::FileSystem::ReadBinaryFile(Zeron::Path("Resources/Fonts/GALACTIC.ttf"));
	ZE_ASSERT(!font.HasError(), "Unable to load font");
	Zeron::Render::FontAtlasProps fontAtlasProps;
	fontAtlasProps.mFontSize = mAtlasFontSize;
	mFontAtlas = std::make_unique<Zeron::Render::FontAtlas>("GALACTIC", fontAtlasProps);
	mFontAtlas->Load(mGraphics, font.Value());

	mSampler = graphics.CreateSampler(Zeron::Gfx::SamplerAddressMode::Clamp, false);
	mViewUniformBuffer = mGraphics.CreateUniformBuffer(Zeron::Mat4{});
	mShaderProgram = mGraphics.CreateShaderProgram(
		"Shape2D",
		Zeron::Gfx::VertexLayout{
			{ "POSITION", Zeron::Gfx::VertexFormat::Float3 },
			{ "UV", Zeron::Gfx::VertexFormat::Float2 },
			{ "COLOR", Zeron::Gfx::VertexFormat::Color },
		},
		Zeron::Gfx::ResourceLayout{
			{ Zeron::Gfx::PipelineResourceType::UniformBuffer, Zeron::Gfx::ShaderType::Vertex, 0 },
			{ Zeron::Gfx::PipelineResourceType::Texture, Zeron::Gfx::ShaderType::Fragment, 1 },
			{ Zeron::Gfx::PipelineResourceType::Sampler, Zeron::Gfx::ShaderType::Fragment, 2 },
		},
		vertexShaderBuffer.Value(),
		fragmentShaderBuffer.Value()
	);

	mPipelineSolid = mGraphics.CreatePipeline(
		mShaderProgram.get(), &renderPass, mGraphics.GetMultiSamplingLevel(), Zeron::Gfx::PrimitiveTopology::TriangleList, true, Zeron::Gfx::FaceCullMode::None
	);
	mPipelineLine = mGraphics.CreatePipeline(
		mShaderProgram.get(), &renderPass, mGraphics.GetMultiSamplingLevel(), Zeron::Gfx::PrimitiveTopology::LineList, true, Zeron::Gfx::FaceCullMode::None
	);

	mBinding = mGraphics.CreatePipelineBinding(
		*mPipelineSolid,
		{ {
			Zeron::Gfx::UniformBindingHandle{ mViewUniformBuffer.get() },
			Zeron::Gfx::TextureBindingHandle{ mFontAtlas->GetTexture() },
			Zeron::Gfx::SamplerBindingHandle{ mSampler.get() },
		} }
	);
}

Renderer2D::~Renderer2D() = default;

void Renderer2D::PrepareBatch()
{
	const size_t vertexCount = mSolidBatch.mVertices.size() + mLineBatch.mVertices.size();
	const size_t indexCount = mSolidBatch.mIndices.size() + mLineBatch.mIndices.size();
	if ((!mVertexBuffer && vertexCount > 0) || (mVertexBuffer && vertexCount != mVertexBuffer->GetCount())) {
		mVertexBuffer = mGraphics.CreateBuffer(
			Zeron::Gfx::BufferType::Vertex, static_cast<uint32_t>(vertexCount), sizeof(Vertex), nullptr, Zeron::Gfx::BufferUsageType::Dynamic
		);
	}
	if ((!mIndexBuffer && indexCount > 0) || (mIndexBuffer && indexCount != mIndexBuffer->GetCount())) {
		mIndexBuffer = mGraphics.CreateBuffer(
			Zeron::Gfx::BufferType::Index, static_cast<uint32_t>(indexCount), sizeof(uint32_t), nullptr, Zeron::Gfx::BufferUsageType::Dynamic
		);
	}
}

void Renderer2D::RenderBatched(Zeron::Gfx::CommandBuffer& cmd, const Zeron::Render::Camera& camera)
{
	if (!mVertexBuffer || mVertexBuffer->GetCount() == 0) {
		return;
	}

	const size_t vertexOffset = mSolidBatch.mVertices.size();
	const size_t indexOffset = mSolidBatch.mIndices.size();
	// Update Batch buffer
	cmd.UpdateBuffer(
		*mVertexBuffer,
		mSolidBatch.mVertices.data(),
		static_cast<uint32_t>(mSolidBatch.mVertices.size()) * sizeof(Vertex),
		0,
		Zeron::Gfx::BufferUpdateRule::KeepMappedMemoryIfAllowed
	);
	cmd.UpdateBuffer(
		*mIndexBuffer,
		mSolidBatch.mIndices.data(),
		static_cast<uint32_t>(mSolidBatch.mIndices.size()) * sizeof(uint32_t),
		0,
		Zeron::Gfx::BufferUpdateRule::KeepMappedMemoryIfAllowed
	);
	// Adjust line indices based on previous batch
	std::for_each(mLineBatch.mIndices.begin(), mLineBatch.mIndices.end(), [vertexOffset](uint32_t& i) {
		i += vertexOffset;
	});
	cmd.UpdateBuffer(
		*mVertexBuffer,
		mLineBatch.mVertices.data(),
		static_cast<uint32_t>(mLineBatch.mVertices.size()) * sizeof(Vertex),
		static_cast<uint32_t>(vertexOffset),
		Zeron::Gfx::BufferUpdateRule::KeepMappedMemoryIfAllowed
	);
	cmd.UpdateBuffer(
		*mIndexBuffer,
		mLineBatch.mIndices.data(),
		static_cast<uint32_t>(mLineBatch.mIndices.size()) * sizeof(uint32_t),
		static_cast<uint32_t>(indexOffset),
		Zeron::Gfx::BufferUpdateRule::KeepMappedMemoryIfAllowed
	);
	// Update projection
	cmd.UpdateBuffer<Zeron::Mat4>(*mViewUniformBuffer, &mProjection);

	// Setup
	cmd.SetVertexBuffer(*mVertexBuffer);
	cmd.SetIndexBuffer(*mIndexBuffer);

	cmd.SetPipeline(*mPipelineSolid);
	cmd.SetPipelineBinding(*mBinding);
	cmd.DrawIndexed(mSolidBatch.mIndices.size());

	cmd.SetPipeline(*mPipelineLine);
	cmd.DrawIndexed(mLineBatch.mIndices.size(), mSolidBatch.mIndices.size());

	_clearBatches();
}

void Renderer2D::_clearBatches()
{
	mLineBatch.Clear();
	mSolidBatch.Clear();
}

void Renderer2D::_drawLine(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& color, int layer)
{
	const uint32_t index = mLineBatch.mVertices.size();
	mLineBatch.mVertices.emplace_back(Vertex{ Zeron::Vec3{ p1.X, p1.Y, _getPosFromLayer(layer) }, Zeron::Vec2{}, color });
	mLineBatch.mVertices.emplace_back(Vertex{ Zeron::Vec3{ p2.X, p2.Y, _getPosFromLayer(layer) }, Zeron::Vec2{}, color });
	mLineBatch.mIndices.emplace_back(0 + index);
	mLineBatch.mIndices.emplace_back(1 + index);
}

void Renderer2D::_drawBox(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& color, int layer, bool isSolid)
{
	auto& batch = isSolid ? mSolidBatch : mLineBatch;
	const uint32_t index = batch.mVertices.size();
	batch.mVertices.emplace_back(Vertex{ Zeron::Vec3{ p1.X, p1.Y, _getPosFromLayer(layer) }, Zeron::Vec2{}, color });
	batch.mVertices.emplace_back(Vertex{ Zeron::Vec3{ p1.X, p2.Y, _getPosFromLayer(layer) }, Zeron::Vec2{}, color });
	batch.mVertices.emplace_back(Vertex{ Zeron::Vec3{ p2.X, p2.Y, _getPosFromLayer(layer) }, Zeron::Vec2{}, color });
	batch.mVertices.emplace_back(Vertex{ Zeron::Vec3{ p2.X, p1.Y, _getPosFromLayer(layer) }, Zeron::Vec2{}, color });
	batch.mIndices.emplace_back(0 + index);
	batch.mIndices.emplace_back(1 + index);
	batch.mIndices.emplace_back(2 + index);
	batch.mIndices.emplace_back(0 + index);
	batch.mIndices.emplace_back(2 + index);
	batch.mIndices.emplace_back(3 + index);
}

void Renderer2D::_drawCircle(const Zeron::Vec2& center, float radius, const Zeron::Color& color, int layer, bool isSolid)
{
	auto& batch = isSolid ? mSolidBatch : mLineBatch;
	const uint32_t index = static_cast<uint32_t>(batch.mVertices.size());
	constexpr int segmentCount = 24;
	const float angle = 2.0f * Zeron::Math::PI<float>() / segmentCount;
	batch.mVertices.push_back({ { center.X, center.Y, _getPosFromLayer(layer) }, {}, color });
	for (int i = 0; i < segmentCount; ++i) {
		const float segmentAngle = angle * static_cast<float>(i);
		float x = cos(segmentAngle) * radius + center.X;
		float y = sin(segmentAngle) * radius + center.Y;
		batch.mVertices.push_back({ { x, y, _getPosFromLayer(layer) }, {}, color });

		if (i != segmentCount - 1) {
			batch.mIndices.push_back(index);
			batch.mIndices.push_back(i + index + 1);
			batch.mIndices.push_back(i + index + 2);
		}
	}
	batch.mIndices.push_back(index);
	batch.mIndices.push_back(index + segmentCount);
	batch.mIndices.push_back(index + 1);
}

void Renderer2D::_drawText(int fontId, const Zeron::Vec2& pos, const char* text, const Zeron::Color& color, int layer)
{
	const uint32_t index = static_cast<uint32_t>(mSolidBatch.mVertices.size());
	Zeron::Render::MeshInfo mesh = mFontAtlas->GenerateText(text);
	const Zeron::Render::MeshAttributeData<float>& posData = mesh.GetAttribute<Zeron::Render::MeshAttribute::Position>();
	const Zeron::Render::MeshAttributeData<float>& uvData = mesh.GetAttribute<Zeron::Render::MeshAttribute::UV>();
	const Zeron::Render::MeshAttributeData<uint32_t>& indexData = mesh.GetAttribute<Zeron::Render::MeshAttribute::Index>();

	const float fontSizeModifier = _getFontSizeModifier(fontId);
	for (size_t i = 0; i < posData.GetSize(); ++i) {
		mSolidBatch.mVertices.emplace_back(Vertex{
			Zeron::Vec3{ posData.GetData()[i * 2] * fontSizeModifier + pos.X, posData.GetData()[(i * 2) + 1] * fontSizeModifier + pos.Y, _getPosFromLayer(layer) },
			Zeron::Vec2{ uvData.GetData()[i * 2], uvData.GetData()[i * 2 + 1] },
			color });
	}

	for (size_t i = 0; i < indexData.GetSize(); ++i) {
		mSolidBatch.mIndices.emplace_back(indexData.GetData()[i] + index);
	}
}

float Renderer2D::_getFontSizeModifier(int fontId) const
{
	switch (fontId) {
		case 0: {
			return 25.f / mAtlasFontSize;
		}
		case 1: {
			return 60.f / mAtlasFontSize;
		}
		case 2: {
			return 14.f / mAtlasFontSize;
		}
		default: {
			return 1.f;
		}
	}
}

void Renderer2D::DrawLine(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& c, int layer)
{
	_drawLine(p1, p2, c, layer);
}

void Renderer2D::DrawBox(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& c, int layer)
{
	_drawBox(p1, p2, c, layer, true);
}

void Renderer2D::DrawLineBox(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& c, int layer)
{
	_drawBox(p1, p2, c, layer, false);
}

void Renderer2D::DrawCircle(const Zeron::Vec2& center, float radius, const Zeron::Color& c, int layer)
{
	_drawCircle(center, radius, c, layer, true);
}

void Renderer2D::DrawLineCircle(const Zeron::Vec2& center, float radius, const Zeron::Color& c, int layer)
{
	_drawCircle(center, radius, c, layer, false);
}

int Renderer2D::LoadFont(const char* file, int fontSize)
{
	return 0;
}

void Renderer2D::DrawText(int fontId, const Zeron::Vec2& pos, const char* text, const Zeron::Color& c, int layer)
{
	_drawText(fontId, pos, text, c, layer);
}
