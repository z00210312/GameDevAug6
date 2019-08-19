#pragma once
#include "ObjComponent.h"
#include "GLSL.h"
#include <unordered_map>
class ObjInventory: public ObjComponent
{
private:
	// class variables
	unsigned short MAX_ROW;
	unsigned short MAX_COL;
	unsigned short MAX_INVENTORY_ITEM_SIZE;

	// class objects
	std::unordered_map <int, ObjComponent*> InventoryItemList;

	// inventory grid
	ObjComponent* inventoryGrid;
	GLuint borderID;
	spriteObj borderSprite[9];
public:
	// class init functions
	void setUp(
		textureObj* _texObj, 
		unsigned short _MAX_ROW, unsigned short _MAX_COL, 
		const char* _emptyIconFilePath, const char* _emptyPortraitFilePath, 
		float _row_distance, float _col_distance, float gridWidth, float gridHeight);

	// class variables functions
	unsigned short getMaxRow() { return MAX_ROW; };
	unsigned short getMaxCol() { return MAX_COL; };
	unsigned short getMaxInventorySize() { return MAX_INVENTORY_ITEM_SIZE; };

	// class objects functions
	bool insertInventoryItem(int _index, ObjComponent* _item);
	ObjComponent* findInventoryItem(int _index);
	bool eraseInventoryItem(int _index);
	std::unordered_map <int, ObjComponent*>* getInventoryItemList() { return &InventoryItemList; };

	// inventory grid
	ObjComponent* getInventoryGrid();
	void setBorder(textureObj* _texObj, const char* _borderFilePath, float borderWidth, float borderHeight);
	void displayBorder(textureObj* _texObj, GLSL* _colorProgram, float _transPosx, float _transPosy);
	void displayInventoryGrid(textureObj* _texObj, GLSL* _colorProgram, float _transPosx, float _transPosy);
	void displayInventoryItem(textureObj* _texObj, GLSL* _colorProgram, float _transPosx, float _transPosy);
};

