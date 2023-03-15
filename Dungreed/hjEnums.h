#pragma once

enum class eSceneType
{
	Title,
	Play,
	Ending,
	Max,
};

enum class eLayerType
{
	BackBG,
	BGobject,
	FrontBG,
	Platform,
	Monster,
	Player,
	Bullet,
	Test,
	Effect,
	UI,
	End = 16,
};

enum class eComponentType
{
	Transform,
	SpriteRenderer,
	Animator,
	Collider,
	Audio,
	End,
};

enum class eWeaponType {
	SWORD,
	GUN,
	END,
};
