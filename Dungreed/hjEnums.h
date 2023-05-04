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
	Dungeon1_0,
	Dungeon1_1,
	Dungeon1_2,
	Dungeon1_3,
	Dungeon1_4,
	End,
};

enum class eLayerType
{
	BackBG,
	FrontBG,
	Ground,
	Tile,
	BGobject,
	Player,
	Monster,
	Weapon_Player,
	Weapon_Monster,
	Bullet_Player,
	Bullet_Monster,
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
	COSMOSSWORD,
	GUN,
	DASHWEAPON,
	DASHWEAPON_MON,
	EMPTY_MON,
	BOW,
	SWORD_MON,
	BOW_MON,
	END,
};

enum class eEffectType {
	SWORD,
	GUN,
	END,
};
