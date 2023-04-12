#pragma once

enum class eSceneType
{
	Title,
	Play,
	Ending,

	Tool,
	End,
};

enum class ePSceneType
{
	Town,
	Dungeon,
	Dungeon1,
	Dungeon2,
	Dungeon3,
	Dungeon4,
	Dungeon5,
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
	Tile,
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
	Rigidbody,
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

enum class eEffectType {
	SWORD,
	GUN,
	END,
};
