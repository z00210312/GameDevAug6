#pragma once
#include "ObjInventory.h"
class ObjCharacter: public ObjComponent
{
private:
	ObjInventory CharacterEquipment;
public:
	void setUp(textureObj * _texObj);
	ObjInventory* getCharacterEquipment();
};

