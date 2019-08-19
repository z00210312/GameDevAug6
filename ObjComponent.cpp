#include "ObjComponent.h"



void ObjComponent::init(textureObj* _texObj, const char* _spriteIconFilePath, const char* _spritePortraitFilePath)
{
	texObj = _texObj;
	m_spriteIconID = texObj->setPNGTextureID(_spriteIconFilePath);
	m_spritePortraitID = texObj->setPNGTextureID(_spritePortraitFilePath);
	m_isInUse = true;
}

void ObjComponent::Once_setSpriteIcon(float posx, float posy, float posWidth, float posHeight, float u, float v, float uvWidth, float uvHeight)
{
	m_spriteIcon.setSprite(posx, posy, posWidth, posHeight, u, v, uvWidth, uvHeight);
}

void ObjComponent::Once_setSpritePortrait(float posx, float posy, float posWidth, float posHeight, float u, float v, float uvWidth, float uvHeight)
{
	m_spritePortrait.setSprite(posx, posy, posWidth, posHeight, u, v, uvWidth, uvHeight);
}

void ObjComponent::Once_drawSpriteIcon()
{
	m_spriteIcon.drawSprite();
}

void ObjComponent::Once_drawSpritePortrait()
{
	m_spritePortrait.drawSprite();
}

void ObjComponent::setSpriteIconUV(float u, float v, float uvWidth, float uvHeight)
{
	m_spriteIcon.setSpriteUV(u, v, uvWidth, uvHeight);
}

void ObjComponent::setSpriteProtraitUV(float u, float v, float uvWidth, float uvHeight)
{
	m_spritePortrait.setSpriteUV(u, v, uvWidth, uvHeight);
}
