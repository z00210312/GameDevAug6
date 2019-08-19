#include "ObjInventory.h"

void ObjInventory::setUp(
	textureObj* _texObj, 
	unsigned short _MAX_ROW, unsigned short _MAX_COL, 
	const char* _emptyIconFilePath, const char* _emptyPortraitFilePath, 
	float _row_distance, float _col_distance, float gridWidth, float gridHeight)
{
	MAX_ROW = _MAX_ROW;
	MAX_COL = _MAX_COL;
	MAX_INVENTORY_ITEM_SIZE = MAX_ROW * MAX_COL;

	inventoryGrid = new ObjComponent[MAX_INVENTORY_ITEM_SIZE];

	float temp_x = 0.0f;
	float temp_y = 0.0f;
	for (int colIndex = 0; colIndex < MAX_COL; colIndex++) {
		for (int rowIndex = 0; rowIndex < MAX_ROW; rowIndex++) {
			inventoryGrid[colIndex*MAX_ROW + rowIndex].init(_texObj, _emptyIconFilePath, _emptyPortraitFilePath);
			inventoryGrid[colIndex*MAX_ROW + rowIndex].Once_setSpriteIcon(temp_x, temp_y, gridWidth, gridHeight, 0.0f, 0.0f, 1.0f, 1.0f);
			temp_x += _row_distance;
		}
		temp_x = 0.0f;
		temp_y -= _col_distance;
	}
}

bool ObjInventory::insertInventoryItem(int _index, ObjComponent * _item)
{
	if (_index < 0 || _index >= MAX_INVENTORY_ITEM_SIZE)
		return false;
	else if (findInventoryItem(_index) == nullptr) {
		std::pair<int, ObjComponent*> newItem(_index, _item);
		InventoryItemList.insert(newItem);
		return true;
	}
	return false;
}

ObjComponent * ObjInventory::findInventoryItem(int _index)
{
	if(InventoryItemList.find(_index) == InventoryItemList.end())
		return nullptr;
	else
		return *&(InventoryItemList.find(_index)->second);
}

bool ObjInventory::eraseInventoryItem(int _index)
{
	if (InventoryItemList.erase(_index) == 1)
		return true;
	else
		return false;
}

ObjComponent * ObjInventory::getInventoryGrid()
{
	return inventoryGrid;
}

void ObjInventory::setBorder(textureObj * _texObj, const char * _borderFilePath, float borderWidth, float borderHeight)
{
	borderID = _texObj->setPNGTextureID(_borderFilePath);
	borderSprite[0].setSprite(0.0f, 0.0f,								0.1f, 0.1f, (float)0 / 3, (float)0 / 3, (float)1 / 3, (float)1 / 3);
	borderSprite[1].setSprite(0.1f, 0.0f,								borderWidth - 0.2f, 0.1f, (float)1 / 3, (float)0 / 3, (float)2 / 3, (float)1 / 3);
	borderSprite[2].setSprite(borderWidth - 0.1f, 0.0f,					0.1f, 0.1f, (float)2 / 3, (float)0 / 3, (float)3 / 3, (float)1 / 3);

	borderSprite[3].setSprite(0.0f, 0.2f - borderHeight,				0.1f, borderHeight - 0.2f, (float)0 / 3, (float)1 / 3, (float)1 / 3, (float)2 / 3);
	borderSprite[4].setSprite(0.1f, 0.2f - borderHeight,				borderWidth - 0.2f, borderHeight - 0.2f, (float)1 / 3, (float)1 / 3, (float)2 / 3, (float)2 / 3);
	borderSprite[5].setSprite(borderWidth - 0.1f, 0.2f - borderHeight,	0.1f, borderHeight - 0.2f, (float)2 / 3, (float)1 / 3, (float)3 / 3, (float)2 / 3);

	borderSprite[6].setSprite(0.0f, 0.1f - borderHeight,					0.1f, 0.1f, (float)0 / 3, (float)2 / 3, (float)1 / 3, (float)3 / 3);
	borderSprite[7].setSprite(0.1f, 0.1f - borderHeight,					borderWidth - 0.2f, 0.1f, (float)1 / 3, (float)2 / 3, (float)2 / 3, (float)3 / 3);
	borderSprite[8].setSprite(borderWidth - 0.1f, 0.1f - borderHeight,	0.1f, 0.1f, (float)2 / 3, (float)2 / 3, (float)3 / 3, (float)3 / 3);
}

void ObjInventory::displayBorder(textureObj * _texObj, GLSL * _colorProgram, float _transPosx, float _transPosy)
{
	for (int i = 0; i < 9; i++) {
		_colorProgram->enable();
		_colorProgram->setTranslateMatrix(_transPosx, _transPosy, 0.0f);
		_texObj->enable(_colorProgram->getUniformLocation("sampler_1"), borderID);
		borderSprite[i].drawSprite();
		_texObj->disable();
		_colorProgram->disable();
	}
}

void ObjInventory::displayInventoryGrid(textureObj * _texObj, GLSL * _colorProgram, float _transPosx, float _transPosy)
{
	for (int i = 0; i < getMaxInventorySize(); i++) {
		_colorProgram->enable();
		_colorProgram->setTranslateMatrix(_transPosx, _transPosy, 0.0f);
		_texObj->enable(_colorProgram->getUniformLocation("sampler_1"), getInventoryGrid()[i].getIconID());
		getInventoryGrid()[i].Once_drawSpriteIcon();
		_texObj->disable();
		_colorProgram->disable();
	}
}

void ObjInventory::displayInventoryItem(textureObj * _texObj, GLSL * _colorProgram, float _transPosx, float _transPosy)
{
	for (int i = 0; i < getMaxInventorySize(); i++) {
		if (findInventoryItem(i) != nullptr && findInventoryItem(i)->isInUse()) {
			_colorProgram->enable();
			_colorProgram->setTranslateMatrix(_transPosx, _transPosy, 0.0f);
			_texObj->enable(_colorProgram->getUniformLocation("sampler_1"), findInventoryItem(i)->getIconID());
			getInventoryGrid()[i].Once_drawSpriteIcon();
			_texObj->disable();
			_colorProgram->disable();
		}
	}
}
