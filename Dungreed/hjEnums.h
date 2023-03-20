#pragma once

enum class eSceneType
{
	Title,
	Play,
	Ending,
	End,
};

enum class eLayerType
{
	BackBG,
	BGobject,
	FrontBG,
	Platform,
	Ground,
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
	Rigidbody,
	Audio,
	End,
};

enum class eWeaponType {
	SWORD,
	GUN,
	END,
};
