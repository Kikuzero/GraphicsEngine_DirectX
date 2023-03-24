#pragma once
#include <SimpleMath.h>
#include "../ECS/IComponent.h"
#include "../MySuper3dApp/Sampler.h"
#include "../MySuper3dApp/Texture.h"

struct TextureComponent : IComponent
{
	Texture* texture;
	Sampler* sampler;
};