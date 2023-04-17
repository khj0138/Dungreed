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
	DungeonNiflheim,
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
	Ground,
	Tile,
	Player,
	Monster,
	Weapon,
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
	EMPTY,
	END,
};

enum class eEffectType {
	SWORD,
	GUN,
	END,
};
