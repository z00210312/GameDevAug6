#pragma once
#include "ObjCharacter.h"
#include "GLSL.h"
#include <unordered_map>
class menuCharacter
{
private:
	// class variables
	struct TranslateInvenotory {
		float x;
		float y;
		float borderx;
		float bordery;
	}translateInvenotory;
	struct TranslateEquipment {
		float x;
		float y;
		float borderx;
		float bordery;
	}translateEquipment;
	struct TranslateCharacter {
		float x;
		float y;
		float borderx;
		float bordery;
	}translateCharacter;
	struct MouseCord {
		float x;
		float y;
	}mouseCord;

	// class objects
	ObjInventory currInventory;
	ObjCharacter* currCharacter;
	std::unordered_map <int, ObjCharacter*> CharacterList;
	GLuint characterBorderID;

	spriteObj movingSprite;
	std::pair<int, int> movingItem;

	float slidingDistance;
	float slidingDistanceDefualt;
	float slidingSpeed;

	// characterGrid
	ObjInventory characterGrid;
public:
	// class init
	void init(textureObj* _texObj);
	void setUp(textureObj* _texObj);

	// class variables functions
	void updateMouseCord(float mousex, float mousey);
	void setTranslateValue(float windowx, float windowy);

	// class objects functions
	ObjInventory* getCurrInventory();
	ObjCharacter* getCurrCharacter();
	
	bool setCurrCharacter(int _index);

	bool insertCharacterToList(int _characterIndex, ObjCharacter* _character);
	ObjCharacter* findCharacterFromList(int _characterIndex);
	bool eraseCharacterFromList(int _characterIndex, std::string& _err_str);

	void setMovingSprite();
	void setMovingItem(int _Objtype, int _ObjIndex);

	int getMovingItemType();
	int getMovingItemIndex();

	void resetSlidingDistance();

	void inventorySorting();
	void characterSorting();

	// inventory grid
	void displayCharacters(textureObj * _texObj, GLSL * _colorProgram, float _transPosx, float _transPosy);
	void displayAllObjects(textureObj * _texObj, GLSL * _colorProgram);
	
	// collision with objects
	int isCollisionWithInventory();
	int isCollisionWithEquipment();
	int isCollisionWithCharacter();

	void moveItemToInventory();
	void moveItemToEquipment();
};

/*
 *---WorkPath---*
 think?
 start to work on team menu

 *---LESS IMPORTANT---*
 add item type 
 need to fix the background_Sprite sliding when change the size of window
 add item portrait
 add equipment portrait
*/
