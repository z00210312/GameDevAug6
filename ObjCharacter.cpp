#include "ObjCharacter.h"

void ObjCharacter::setUp(textureObj * _texObj)
{
	CharacterEquipment.init(_texObj, "Shader/border_0.png", "Shader/border_0.png");
	CharacterEquipment.setUp(_texObj, 2, 3, "Shader/BlankSpot_1.png", "Shader/BlankSpot_1.png", 0.2f, 0.1f, 0.1f, 0.1f);
	CharacterEquipment.setBorder(_texObj, "Shader/border_sprite2.png", 0.7f, 0.5f);
}

ObjInventory* ObjCharacter::getCharacterEquipment()
{
	return &CharacterEquipment;
}
