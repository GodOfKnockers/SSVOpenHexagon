// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef HG_GLOBAL_COMMON
#define HG_GLOBAL_COMMON

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SSVUtils/SSVUtils.h>
#include <SSVUtilsJson/SSVUtilsJson.h>
#include <SSVStart/SSVStart.h>
#include <SSVEntitySystem/SSVEntitySystem.h>
#include <SSVMenuSystem/SSVMenuSystem.h>
#include <SSVLuaWrapper/SSVLuaWrapper.h>

namespace hg
{
	// Typedefs
	using Path = ssvu::FileSystem::Path;
	template<typename T> using Vec2 = ssvs::Vec2<T>;
	template<typename T, typename TDeleter = std::default_delete<T>> using Uptr = ssvs::Uptr<T, TDeleter>;
	using Vec2i = ssvs::Vec2i;
	using Vec2f = ssvs::Vec2f;
	using Vec2u = ssvs::Vec2u;

	// Game enums
	enum HGGroup{Wall};
}

#endif
