#pragma once
#include "spriteObj.h"
#include "textureObj.h"
class ObjComponent
{
private:
	unsigned short m_ObjID;

	GLuint m_spriteIconID;
	spriteObj m_spriteIcon;
	GLuint m_spritePortraitID;
	spriteObj m_spritePortrait;

	bool m_isInUse;

	textureObj* texObj;
public:
	void init(textureObj* _texObj, const char* _spriteIconFilePath, const char* _spritePortraitFilePath);

	void Once_setSpriteIcon(float posx, float posy, float posWidth, float posHeight, float u, float v, float uvWidth, float uvHeight);
	void Once_setSpritePortrait(float posx, float posy, float posWidth, float posHeight, float u, float v, float uvWidth, float uvHeight);
	void Once_drawSpriteIcon();
	void Once_drawSpritePortrait();

	void setObjID(unsigned short _ObjID) { m_ObjID = _ObjID; };
	unsigned short getObjID() { return m_ObjID; };
	
	void enable() { m_isInUse = true; };
	void disable() { m_isInUse = false; };
	bool isInUse() { return m_isInUse; };

	GLuint getIconID() { return m_spriteIconID; };
	float getIconPosx()			{ return m_spriteIcon.getVexPosx(); };
	float getIconPosy()			{ return m_spriteIcon.getVexPosy(); };
	float getIconPosWidth()		{ return m_spriteIcon.getVexWidth(); };
	float getIconPosHeight()	{ return m_spriteIcon.getVexHeight(); };
	float getIconPosu()			{ return m_spriteIcon.getVexPosu(); };
	float getIconPosv()			{ return m_spriteIcon.getVexPosv(); };
	float getIconPosuvWidth()	{ return m_spriteIcon.getVexUVWidth(); };
	float getIconPosuvHeight()	{ return m_spriteIcon.getVexUVHeight(); };

	GLuint getPortraitID() { return m_spritePortraitID; };
	float getPortraitPosx()			{ return m_spritePortrait.getVexPosx(); };
	float getPortraitPosy()			{ return m_spritePortrait.getVexPosy(); };
	float getPortraitPosWidth()		{ return m_spritePortrait.getVexWidth(); };
	float getPortraitPosHeight()	{ return m_spritePortrait.getVexHeight(); };
	float getPortraitPosu()			{ return m_spritePortrait.getVexPosu(); };
	float getPortraitPosv()			{ return m_spritePortrait.getVexPosv(); };
	float getPortraitPosuvWidth()	{ return m_spritePortrait.getVexUVWidth(); };
	float getPortraitPosuvHeight()	{ return m_spritePortrait.getVexUVHeight(); };

	void setSpriteIconUV(float u, float v, float uvWidth, float uvHeight);
	void setSpriteProtraitUV(float u, float v, float uvWidth, float uvHeight);
};

