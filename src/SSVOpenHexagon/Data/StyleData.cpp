// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVOpenHexagon/Data/StyleData.h"
#include "SSVOpenHexagon/Utils/Utils.h"
#include "SSVOpenHexagon/Global/Config.h"

using namespace std;
using namespace sf;
using namespace ssvs;
using namespace ssvs::Utils;
using namespace hg::Utils;
using namespace ssvu;
using namespace ssvuj;

namespace hg
{
	Color StyleData::calculateColor(const ColorData& mColorData) const
	{
		Color color{mColorData.color};

		if(mColorData.dynamic)
		{
			const auto& dynamicColor(getColorFromHue((currentHue + mColorData.hueShift) / 360.0f));

			if(!mColorData.main)
			{
				if(mColorData.dynamicOffset)
				{
					const auto& offset(mColorData.offset);
					color.r += dynamicColor.r / offset;
					color.g += dynamicColor.g / offset;
					color.b += dynamicColor.b / offset;
					color.a += dynamicColor.a;
				}
				else color = getColorDarkened(dynamicColor, mColorData.dynamicDarkness);
			}
			else color = dynamicColor;
		}

		const auto& pulse(mColorData.pulse);
		return Color(getClamped(color.r + pulse.r * pulseFactor, 0.f, 255.f),
					 getClamped(color.g + pulse.g * pulseFactor, 0.f, 255.f),
					 getClamped(color.b + pulse.b * pulseFactor, 0.f, 255.f),
					 getClamped(color.a + pulse.a * pulseFactor, 0.f, 255.f));
	}

	void StyleData::update(float mFrameTime, float mMult)
	{
		currentSwapTime += mFrameTime * mMult;
		if(currentSwapTime > maxSwapTime) currentSwapTime = 0;

		currentHue += hueIncrement * mFrameTime * mMult;

		if(currentHue < hueMin)
		{
			if(huePingPong) { currentHue = hueMin; hueIncrement *= -1.f; }
			else currentHue = hueMax;
		}
		if(currentHue > hueMax)
		{
			if(huePingPong) { currentHue = hueMax; hueIncrement *= -1.f; }
			else currentHue = hueMin;
		}

		pulseFactor += pulseIncrement * mFrameTime;

		if(pulseFactor < pulseMin) { pulseIncrement *= -1.f; pulseFactor = pulseMin; }
		if(pulseFactor > pulseMax) { pulseIncrement *= -1.f; pulseFactor = pulseMax; }
	}

	void StyleData::computeColors()
	{
		currentMainColor = calculateColor(mainColorData);
		current3DOverrideColor = _3dOverrideColor.a != 0 ? _3dOverrideColor : getMainColor();
		currentColors.clear();
		for(const auto& cd : colorDatas) currentColors.push_back(calculateColor(cd));
		rotate(currentColors.begin(), currentColors.begin() + currentSwapTime / (maxSwapTime / 2.f), currentColors.end());
	}

	void StyleData::drawBackground(RenderTarget& mRenderTarget, Vec2f mCenterPos, int mSides)
	{
		float div{360.f / mSides * 1.0001f}, distance{4500};

		VertexArray vertices{PrimitiveType::Triangles, 3};
		const auto& colors(getColors());

		for(int i{0}; i < mSides; ++i)
		{
			float angle{div * i};
			Color currentColor{colors[i % colors.size()]};
			if(Config::getBlackAndWhite()) currentColor = Color::Black;

			if(i % 2 == 0) if(i == mSides - 1) currentColor = getColorDarkened(currentColor, 1.4f);

			vertices.append(Vertex{mCenterPos, currentColor});
			vertices.append(Vertex{getOrbitFromDegrees(mCenterPos, angle + div * 0.5f, distance), currentColor});
			vertices.append(Vertex{getOrbitFromDegrees(mCenterPos, angle - div * 0.5f, distance), currentColor});
		}

		mRenderTarget.draw(vertices);
	}
}

