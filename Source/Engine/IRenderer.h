// Missile Command, Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

const int kViewportWidth = 800;
const int kViewportHeight = 600;

class IRenderer {
  public:
	virtual void DrawLine(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& color, int nLayer) = 0;
	virtual void DrawBox(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& color, int nLayer) = 0;
	virtual void DrawLineBox(const Zeron::Vec2& p1, const Zeron::Vec2& p2, const Zeron::Color& color, int nLayer) = 0;
	virtual void DrawCircle(const Zeron::Vec2& center, float radius, const Zeron::Color& color, int nLayer) = 0;
	virtual void DrawLineCircle(const Zeron::Vec2& center, float radius, const Zeron::Color& color, int nLayer) = 0;
	virtual int LoadFont(const char* file, int size) = 0;
	virtual void DrawText(int fontId, const Zeron::Vec2& pos, const char* text, const Zeron::Color& color, int nLayer) = 0;
};
