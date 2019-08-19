#include "menuCharacter.h"

void menuCharacter::init(textureObj * _texObj)
{
	currInventory.init(_texObj, "Shader/border_0.png", "Shader/border_0.png");
	//currCharacter.init(_texObj, "Shader/characterName.png", "Shader/characterName.png");
	movingItem.first = -1;
	movingItem.second = -1;

	slidingDistanceDefualt = 2.0f;
	slidingDistance = 0.8f;
	slidingSpeed = 0.06f;
}

void menuCharacter::setUp(textureObj* _texObj)
{
	currInventory.setUp(_texObj, 8, 4, "Shader/BlankSpot_1.png", "Shader/BlankSpot_1.png", 0.1f, 0.1f, 0.1f, 0.1f);
	currInventory.setBorder(_texObj, "Shader/border_sprite1.png", 1.2f, 2.0f);

	//setCharacterBorder(_texObj, "Shader/border_sprite1.png", 0.5f, 1.6f);

	//currCharacter.setUp(_texObj);
	//currCharacter.Once_setSpriteIcon(0.0f, 0.0f, 0.3f, 0.1f, 0.0f, 0.0f, 1.0f, (float)1 / 3);


	characterGrid.setUp(_texObj, 1, 4, "Shader/BlankSpot_1.png", "Shader/BlankSpot_1.png", 0.3f, 0.1f, 0.3f, 0.1f);
	characterGrid.setBorder(_texObj, "Shader/border_sprite1.png", 0.5f, 1.6f);
}

ObjInventory* menuCharacter::getCurrInventory()
{
	return &currInventory;
}

ObjCharacter* menuCharacter::getCurrCharacter()
{
	return currCharacter;
}

bool menuCharacter::setCurrCharacter(int _index)
{
	if (findCharacterFromList(_index) != nullptr) {
		currCharacter = findCharacterFromList(_index);
		return true;
	}
	return false;
}

bool menuCharacter::insertCharacterToList(int _characterIndex, ObjCharacter* _character)
{
	if (_characterIndex > characterGrid.getMaxInventorySize() || _characterIndex < 0)
		return false;
	else if (findCharacterFromList(_characterIndex) == nullptr) {
		std::pair<int, ObjCharacter*> newItem(_characterIndex, _character);
		CharacterList.insert(newItem);
		currCharacter = _character;
		characterGrid.getInventoryGrid()[_characterIndex].setSpriteIconUV(findCharacterFromList(_characterIndex)->getIconPosu(), findCharacterFromList(_characterIndex)->getIconPosv(), findCharacterFromList(_characterIndex)->getIconPosuvWidth(), findCharacterFromList(_characterIndex)->getIconPosuvHeight());
		return true;
	}
	return false;
}

ObjCharacter * menuCharacter::findCharacterFromList(int _characterIndex)
{
	std::unordered_map <int, ObjCharacter*>::iterator it = CharacterList.find(_characterIndex);
	if (it == CharacterList.end())
		return nullptr;
	else
		return *&it->second;
}

bool menuCharacter::eraseCharacterFromList(int _characterIndex, std::string& _err_str)
{
	if (CharacterList.size() <= 1) {
		_err_str = "Unable to remove if only one character exist";
		return false;
	}
	else if (CharacterList.erase(_characterIndex) == 1) {
		characterGrid.getInventoryGrid()[_characterIndex].setSpriteIconUV(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < characterGrid.getMaxInventorySize(); i++) {
			if(setCurrCharacter(i))
				return true;
		}
		_err_str = "Unable to set CurrCharacter";
		return false;
	}
	else {
		_err_str = "character is not exist";
		return false;
	}
	_err_str = "unknown error!";
	return false;
}

void menuCharacter::setMovingSprite()
{
	movingSprite.setSprite(-0.05f, -0.05f, 0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 1.0f);
}

void menuCharacter::setMovingItem(int _Objtype, int _ObjIndex)
{
	if (movingItem.first == -1) {
		movingItem.first = _Objtype;
		movingItem.second = _ObjIndex;

		if (_Objtype == 0) {
			currInventory.findInventoryItem(movingItem.second)->disable();
		}
		else if (_Objtype == 1) {
			currCharacter->getCharacterEquipment()->findInventoryItem(movingItem.second)->disable();
		}
	}
	else {
		if (movingItem.first == 0) {
			currInventory.findInventoryItem(movingItem.second)->enable();
		}
		else if (movingItem.first == 1) {
			currCharacter->getCharacterEquipment()->findInventoryItem(movingItem.second)->enable();
		}
		movingItem.first = _Objtype;
		movingItem.second = _ObjIndex;
	}
}

int menuCharacter::getMovingItemType()
{
	return movingItem.first;
}

int menuCharacter::getMovingItemIndex()
{
	return movingItem.second;
}

void menuCharacter::resetSlidingDistance()
{
	slidingDistance = slidingDistanceDefualt;
}

void menuCharacter::inventorySorting()
{
	std::map<int, ObjComponent*> ordered(currInventory.getInventoryItemList()->begin(), currInventory.getInventoryItemList()->end());
	int map_size = currInventory.getInventoryItemList()->size();
	currInventory.getInventoryItemList()->clear();
	int i = 0;
	for (auto it = ordered.begin(); it != ordered.end(); ++it) {
		currInventory.insertInventoryItem(i++, it->second);
	}
}

void menuCharacter::characterSorting()
{
	std::map<int, ObjCharacter*> ordered(CharacterList.begin(), CharacterList.end());
	int map_size = CharacterList.size();
	auto it = ordered.begin();
	std::string _errmsg;
	
	for (int i = 0; i<map_size; i++) {
		eraseCharacterFromList(it->first,_errmsg);
		insertCharacterToList(i, it->second);
		it++;
	}
}

void menuCharacter::displayCharacters(textureObj * _texObj, GLSL * _colorProgram, float _transPosx, float _transPosy)
{
	for (int i = 0; i < characterGrid.getMaxInventorySize(); i++) {
		if (findCharacterFromList(i) != nullptr) {
			_colorProgram->enable();
			_colorProgram->setTranslateMatrix(_transPosx, _transPosy, 0.0f);
			_texObj->enable(_colorProgram->getUniformLocation("sampler_1"), findCharacterFromList(i)->getIconID());
			characterGrid.getInventoryGrid()[i].Once_drawSpriteIcon();
			_texObj->disable();
			_colorProgram->disable();
		}
	}
}

void menuCharacter::displayAllObjects(textureObj * _texObj, GLSL * _colorProgram)
{
	//sliding the window
	if (slidingDistance > 0.0f) {
		slidingDistance -= slidingSpeed;
	}
	else {
		slidingDistance = 0.0f;
	}

	//display objects
	if (slidingDistance <= 0.0f) {
		characterGrid.displayBorder(_texObj, _colorProgram, translateCharacter.borderx, translateCharacter.bordery);
		currInventory.displayBorder(_texObj, _colorProgram, translateInvenotory.borderx, translateInvenotory.bordery);
		currCharacter->getCharacterEquipment()->displayBorder(_texObj, _colorProgram, translateEquipment.borderx, translateEquipment.bordery);

		currInventory.displayInventoryGrid(_texObj, _colorProgram, translateInvenotory.x, translateInvenotory.y);
		currInventory.displayInventoryItem(_texObj, _colorProgram, translateInvenotory.x, translateInvenotory.y);

		currCharacter->getCharacterEquipment()->displayInventoryGrid(_texObj, _colorProgram, translateEquipment.x, translateEquipment.y);
		currCharacter->getCharacterEquipment()->displayInventoryItem(_texObj, _colorProgram, translateEquipment.x, translateEquipment.y);

		characterGrid.displayInventoryGrid(_texObj, _colorProgram, translateCharacter.x, translateCharacter.y);
		displayCharacters(_texObj, _colorProgram, translateCharacter.x, translateCharacter.y);

		if (movingItem.second != -1) {
			if (movingItem.first == 0) {
				_colorProgram->enable();
				_colorProgram->setTranslateMatrix(mouseCord.x, mouseCord.y, 0.0f);
				_texObj->enable(_colorProgram->getUniformLocation("sampler_1"), currInventory.findInventoryItem(movingItem.second)->getIconID());
				movingSprite.drawSprite();
				_texObj->disable();
				_colorProgram->disable();
			}
			else if (movingItem.first == 1) {
				_colorProgram->enable();
				_colorProgram->setTranslateMatrix(mouseCord.x, mouseCord.y, 0.0f);
				_texObj->enable(_colorProgram->getUniformLocation("sampler_1"), currCharacter->getCharacterEquipment()->findInventoryItem(movingItem.second)->getIconID());
				movingSprite.drawSprite();
				_texObj->disable();
				_colorProgram->disable();
			}
		}
	}
	else {
		currInventory.displayBorder(_texObj, _colorProgram, translateInvenotory.borderx + slidingDistance, translateInvenotory.bordery);
	}
}

void menuCharacter::updateMouseCord(float mousex, float mousey)
{
	mouseCord.x = mousex;
	mouseCord.y = mousey;
}

void menuCharacter::setTranslateValue(float windowx, float windowy)
{
	translateInvenotory.x		= windowx / windowy - 1.0f;
	translateInvenotory.y		= -0.4f;
	translateInvenotory.borderx = windowx / windowy - 1.2f;
	translateInvenotory.bordery = 0.9f;

	translateEquipment.x		= windowx / windowy - 1.0f;
	translateEquipment.y		= 0.8f;
	translateEquipment.borderx	= windowx / windowy - 1.2f;
	translateEquipment.bordery	= 0.9f;

	translateCharacter.x		= windowx / windowy - 1.5f;
	translateCharacter.y		= 0.8f;
	translateCharacter.borderx	= windowx / windowy - 1.6f;
	translateCharacter.bordery	= 0.9f;
}

int menuCharacter::isCollisionWithInventory()
{
	float tempMouse_x = mouseCord.x - translateInvenotory.x;
	float tempMouse_y = mouseCord.y - translateInvenotory.y;

	for (int i = 0; i < currInventory.getMaxInventorySize(); i++) {
		if (tempMouse_x > currInventory.getInventoryGrid()[i].getIconPosx() && tempMouse_x < currInventory.getInventoryGrid()[i].getIconPosx() + currInventory.getInventoryGrid()[i].getIconPosWidth()) {
			if (tempMouse_y > currInventory.getInventoryGrid()[i].getIconPosy() && tempMouse_y < currInventory.getInventoryGrid()[i].getIconPosy() + currInventory.getInventoryGrid()[i].getIconPosHeight()) {
				//std::cout << i << std::endl;
				return i;
			}
		}
	}
	return -1;
}

int menuCharacter::isCollisionWithEquipment()
{
	float tempMouse_x = mouseCord.x - translateEquipment.x;
	float tempMouse_y = mouseCord.y - translateEquipment.y;

	for (int i = 0; i < currCharacter->getCharacterEquipment()->getMaxInventorySize(); i++) {
		if (tempMouse_x > currCharacter->getCharacterEquipment()->getInventoryGrid()[i].getIconPosx() && tempMouse_x < currCharacter->getCharacterEquipment()->getInventoryGrid()[i].getIconPosx() + currCharacter->getCharacterEquipment()->getInventoryGrid()[i].getIconPosWidth()) {
			if (tempMouse_y > currCharacter->getCharacterEquipment()->getInventoryGrid()[i].getIconPosy() && tempMouse_y < currCharacter->getCharacterEquipment()->getInventoryGrid()[i].getIconPosy() + currCharacter->getCharacterEquipment()->getInventoryGrid()[i].getIconPosHeight()) {
				//std::cout << i << std::endl;
				return i;
			}
		}
	}
	return -1;
}

int menuCharacter::isCollisionWithCharacter()
{
	float tempMouse_x = mouseCord.x - translateCharacter.x;
	float tempMouse_y = mouseCord.y - translateCharacter.y;

	for (int i = 0; i < characterGrid.getMaxInventorySize(); i++) {
		if (tempMouse_x > characterGrid.getInventoryGrid()[i].getIconPosx() && tempMouse_x < characterGrid.getInventoryGrid()[i].getIconPosx() + characterGrid.getInventoryGrid()[i].getIconPosWidth()) {
			if (tempMouse_y > characterGrid.getInventoryGrid()[i].getIconPosy() && tempMouse_y < characterGrid.getInventoryGrid()[i].getIconPosy() + characterGrid.getInventoryGrid()[i].getIconPosHeight()) {
				//std::cout << i << std::endl;
				return i;
			}
		}
	}
	return -1;
}

void menuCharacter::moveItemToInventory()
{
	int _movingType = getMovingItemType();
	int _movingIndex = getMovingItemIndex();
	int _collisionIndex;
	ObjComponent* Item1;
	ObjComponent* Item2;
	//if the item is in the inventory
	if ((_collisionIndex = isCollisionWithInventory()) >= 0) {
		//if no item found on distination
		if (currInventory.findInventoryItem(_collisionIndex) == nullptr) {
			setMovingItem(-1, -1);
			if (_movingType == 0) {
				currInventory.insertInventoryItem(
					_collisionIndex,
					currInventory.findInventoryItem(_movingIndex)
				);
				currInventory.eraseInventoryItem(_movingIndex);
			}
			else if (_movingType == 1) {
				currInventory.insertInventoryItem(
					_collisionIndex,
					currCharacter->getCharacterEquipment()->findInventoryItem(_movingIndex)
				);
				currCharacter->getCharacterEquipment()->eraseInventoryItem(_movingIndex);
			}
		}
		//if item found on distination
		else {
			setMovingItem(-1, -1);

			if (_movingType == 0) {
				Item1 = currInventory.findInventoryItem(_movingIndex);
				Item2 = currInventory.findInventoryItem(_collisionIndex);

				currInventory.eraseInventoryItem(_movingIndex);
				currInventory.eraseInventoryItem(_collisionIndex);

				currInventory.insertInventoryItem(_collisionIndex, Item1);
				currInventory.insertInventoryItem(_movingIndex, Item2);
			}
			else if (_movingType == 1) {
				Item1 = currCharacter->getCharacterEquipment()->findInventoryItem(_movingIndex);
				Item2 = currInventory.findInventoryItem(_collisionIndex);

				currCharacter->getCharacterEquipment()->eraseInventoryItem(_movingIndex);
				currInventory.eraseInventoryItem(_collisionIndex);

				currInventory.insertInventoryItem(_collisionIndex, Item1);
				currCharacter->getCharacterEquipment()->insertInventoryItem(_movingIndex, Item2);
			}
		}
	}
}

void menuCharacter::moveItemToEquipment()
{
	int _movingType = getMovingItemType();
	int _movingIndex = getMovingItemIndex();
	int _collisionIndex;
	ObjComponent* Item1;
	ObjComponent* Item2;
	if ((_collisionIndex = isCollisionWithEquipment()) >= 0) {
		//if no item found on distination
		if (currCharacter->getCharacterEquipment()->findInventoryItem(_collisionIndex) == nullptr) {
			setMovingItem(-1, -1);
			if (_movingType == 0) {
				currCharacter->getCharacterEquipment()->insertInventoryItem(
					_collisionIndex,
					currInventory.findInventoryItem(_movingIndex)
				);
				currInventory.eraseInventoryItem(_movingIndex);
			}
			else if (_movingType == 1) {
				currCharacter->getCharacterEquipment()->insertInventoryItem(
					_collisionIndex,
					currCharacter->getCharacterEquipment()->findInventoryItem(_movingIndex)
				);
				currCharacter->getCharacterEquipment()->eraseInventoryItem(_movingIndex);
			}
		}
		//if item found on distination
		else {
			setMovingItem(-1, -1);

			if (_movingType == 0) {
				Item1 = currInventory.findInventoryItem(_movingIndex);
				Item2 = currCharacter->getCharacterEquipment()->findInventoryItem(_collisionIndex);

				currInventory.eraseInventoryItem(_movingIndex);
				currCharacter->getCharacterEquipment()->eraseInventoryItem(_collisionIndex);

				currCharacter->getCharacterEquipment()->insertInventoryItem(_collisionIndex, Item1);
				currInventory.insertInventoryItem(_movingIndex, Item2);
			}
			else if (_movingType == 1) {
				Item1 = currCharacter->getCharacterEquipment()->findInventoryItem(_movingIndex);
				Item2 = currCharacter->getCharacterEquipment()->findInventoryItem(_collisionIndex);

				currCharacter->getCharacterEquipment()->eraseInventoryItem(_movingIndex);
				currCharacter->getCharacterEquipment()->eraseInventoryItem(_collisionIndex);

				currCharacter->getCharacterEquipment()->insertInventoryItem(_collisionIndex, Item1);
				currCharacter->getCharacterEquipment()->insertInventoryItem(_movingIndex, Item2);
			}
		}
	}
}
