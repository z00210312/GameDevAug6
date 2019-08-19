#include "gameWindow.h"
//#define WaitPullMode 0;

gameWindow::gameWindow(const char *title, int &width, int &height)
{
	_DefaultColorProgram = new GLSL();
	m_texObj = new textureObj();
	fontTex = new gameFont();


	// window variables
	m_title = title;
	m_width = width;
	m_height = height;

	// openGL variables
	init();
	glClearColor(0.5, 0.5f, 0.5, 1.0f);
	glfwPollEvents();
	glfwSwapBuffers(m_window);

	// game variables
	newState = loading;

	lastTime = 0.0;
	CurrentSpritesLocation_x = 0.0f;
	CurrentSpritesLocation_y = 0.0f;
	light_pos_z = -1.0f;

	testScaleValue = 1.0f;
	testDegree = 0.0f;

	// GLSL
	_DefaultColorProgram->setGLSL("Shader/newShader.vert", "Shader/newShader.frag", "Shader/newShader.attr");

	setMainScene();
	setCrusor();

	// finish loading 
	newState = mainScene;
	MenuSprites[0].setSprite((float)-width / height + 0.1f, 0.7f, 0.6f, 0.2f, 0.0f, (float)0 / 3, 1.0f, (float)1 / 3);
	MenuSprites[1].setSprite((float)-width / height + 0.1f, 0.5f, 0.6f, 0.2f, 0.0f, (float)1 / 3, 1.0f, (float)2 / 3);
	MenuSprites[2].setSprite((float)-width / height + 0.1f, 0.3f, 0.6f, 0.2f, 0.0f, (float)2 / 3, 1.0f, (float)3 / 3);
	MenuTexture = m_texObj->setPNGTextureID("Shader/gameMenu.png");
	FontTexture = m_texObj->setPNGTextureID("Shader/font.png");

	/////////////////
	// game variables
	/////////////////

	// init
	newMenu.init(m_texObj);
	
	// setUP
	newMenu.setUp(m_texObj);
	newMenu.setTranslateValue((float)m_width, (float)m_height);

	// add items
	newItem.init(m_texObj, "Shader/BlankSpot_0.png", "Shader/BlankSpot_0.png");
	newItem.Once_setSpriteIcon(0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 1.0f);
	newItem2.init(m_texObj, "Shader/border_0.png", "Shader/border_0.png");
	newItem2.Once_setSpriteIcon(0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 1.0f);

	newMenu.getCurrInventory()->insertInventoryItem(8, &newItem);
	newMenu.getCurrInventory()->insertInventoryItem(5, &newItem2);

	// add characters
	newCharacter.init(m_texObj, "Shader/characterName.png", "Shader/characterName.png");
	newCharacter.setUp(m_texObj);
	newCharacter.Once_setSpriteIcon(0.0f, 0.0f, 0.3f, 0.1f, 0.0f, 0.0f, 1.0f, (float)1 / 3);
	newMenu.insertCharacterToList(0, &newCharacter);

	newCharacter2.init(m_texObj, "Shader/characterName.png", "Shader/characterName.png");
	newCharacter2.setUp(m_texObj);
	newCharacter2.Once_setSpriteIcon(0.0f, 0.0f, 0.3f, 0.1f, 0.0f, (float)1 / 3, 1.0f, (float)2 / 3);
	newMenu.insertCharacterToList(3, &newCharacter2);

	//newMenu.findCharacterFromList(3)->getCharacterEquipment()->insertInventoryItem(3, &newItem2);

	//newMenu.getCurrCharacter()->getCharacterEquipment()->insertInventoryItem(3, &newItem2);
	///////////////
	// ERROR! this is not point to the actual address
	// UPDATE: fixed
	// <map> needs to be fixed
	///////////////
	//std::string _err_str = "";
	//newMenu.eraseCharacterFromList(3, _err_str);
	//std::cout << "first character erase : " << _err_str << std::endl;
	//newMenu.eraseCharacterFromList(3, _err_str);
	//std::cout << "second character erase : " << _err_str << std::endl;
	

	itemCounter = 0;
}


gameWindow::~gameWindow()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();

	delete _DefaultColorProgram;
	delete m_texObj;
	delete fontTex;
}

bool gameWindow::init()
{
	if (!glfwInit()) {
		std::cout << "GLFW error" << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (!m_window) {
		std::cout << "window error" << std::endl;
		return false;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetWindowPos(m_window, 100, 100);

	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW error" << std::endl;
		return false;
	}

	initKeys();
	initCallbacks();

	return true;
}

void gameWindow::initKeys()
{
	for (bool i : m_key)
		i = false;
	for (bool j : m_mouseButton)
		j = false;
}

void gameWindow::initCallbacks()
{
	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetCursorPosCallback(m_window, cursor_position_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);
}

void gameWindow::gameLoop()
{
	while (!gameClose()) {
		time = glfwGetTime();
		deltaTime = time - lastTime;
		if (deltaTime >= maxPeriod) {
			lastTime = time;
			gameClear();
			gameUpdate();
		}
	}
}

void gameWindow::gameClear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gameWindow::gameUpdate()
{
	glfwPollEvents();

	resetDrawProgram();
	switch (newState) {
	case mainScene:
		spriteMainScene();
		break;
	case playScene:
		_DefaultColorProgram->enable();
		m_texObj->enable(_DefaultColorProgram->getUniformLocation("sampler_1"), MenuTexture);
		MenuSprites[0].drawSprite();
		m_texObj->disable();
		_DefaultColorProgram->disable();

		_DefaultColorProgram->enable();
		m_texObj->enable(_DefaultColorProgram->getUniformLocation("sampler_1"), MenuTexture);
		MenuSprites[1].drawSprite();
		m_texObj->disable();
		_DefaultColorProgram->disable();

		_DefaultColorProgram->enable();
		m_texObj->enable(_DefaultColorProgram->getUniformLocation("sampler_1"), MenuTexture);
		MenuSprites[2].drawSprite();
		m_texObj->disable();
		_DefaultColorProgram->disable();
		break;
	case characterMenuScene:
		if (isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {

		}
		newMenu.updateMouseCord(getWindowMousePosx(),getWindowMousePosy());
		newMenu.setMovingSprite();
		newMenu.displayAllObjects(m_texObj, _DefaultColorProgram);
		break;
	case teamMenuScene:
		if (isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		}
		break;
	case missionScene:
		_DefaultColorProgram->enable();
		m_texObj->enable(_DefaultColorProgram->getUniformLocation("sampler_1"), FontTexture);
		fontTex->generateFontTexture("HELLO WORLD");
		m_texObj->disable();
		_DefaultColorProgram->disable();
		break;
	default:
		break;
	}
	spriteCrusor();
	glfwSwapBuffers(m_window);
}

bool gameWindow::isKeyPressed(unsigned int key) const
{
	if (key>KEY_MAX)
		return false;
	return m_key[key];
}

bool gameWindow::isMouseButtonPressed(unsigned int button) const
{
	if (button>MOUSE_BUTTON_MAX)
		return false;
	return m_mouseButton[button];
}

void gameWindow::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	gameWindow* win = (gameWindow*)glfwGetWindowUserPointer(window);
	switch (win->newState) {
	case playScene:
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			win->newState = mainScene;
		break;
	case characterMenuScene:
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			win->newState = playScene;
		else if (key == GLFW_KEY_KP_0 && action == GLFW_PRESS) {
			win->newMenu.setCurrCharacter(0);
		}
		else if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS) {
			win->newMenu.setCurrCharacter(3);
		}
		else if (key == GLFW_KEY_KP_9 && action == GLFW_PRESS) {
			win->newMenu.inventorySorting();
			win->newMenu.characterSorting();
		}
		break;
	case teamMenuScene:
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			win->newState = playScene;
		break;
	case missionScene:
		break;
	default:
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	}

	win->m_key[key] = action != GLFW_RELEASE;
}

void gameWindow::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	gameWindow* win = (gameWindow*)glfwGetWindowUserPointer(window);

	switch (win->newState) {
	case mainScene:
		if (action == GLFW_PRESS) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				win->newState = playScene;
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			}
		}
		else if (action == GLFW_RELEASE) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			}
		}
		break;
	case playScene:
		if (action == GLFW_PRESS) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				if (win->MenuSprites[0].isCollision(win->getWindowMousePosx(), win->getWindowMousePosy(), 0.0f, 0.0f)) {
					win->newState = characterMenuScene;
					win->newMenu.resetSlidingDistance();
				}
				else if (win->MenuSprites[1].isCollision(win->getWindowMousePosx(), win->getWindowMousePosy(), 0.0f, 0.0f))
					win->newState = teamMenuScene;
				else if (win->MenuSprites[2].isCollision(win->getWindowMousePosx(), win->getWindowMousePosy(), 0.0f, 0.0f))
					win->newState = missionScene;
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
				win->newState = mainScene;
			}
		}
		else if (action == GLFW_RELEASE) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {

			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {

			}
		}
		break;
	case loading:
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			win->CurrentSpritesLocation_x = 0.0f;
			win->CurrentSpritesLocation_y = 0.0f;
			win->newState = mainScene;
		}
		break;
	case characterMenuScene:
		if (action == GLFW_PRESS) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				//if the item is in the inventory
				if (win->newMenu.isCollisionWithInventory() >= 0) {
					if (win->newMenu.getCurrInventory()->findInventoryItem(win->newMenu.isCollisionWithInventory()) != nullptr) {
						win->newMenu.setMovingItem(0, win->newMenu.isCollisionWithInventory());
					}
				}
				//if the item is in the equipment
				else if (win->newMenu.isCollisionWithEquipment() >= 0) {
					if (win->newMenu.getCurrCharacter()->getCharacterEquipment()->findInventoryItem(win->newMenu.isCollisionWithEquipment()) != nullptr) {
						win->newMenu.setMovingItem(1, win->newMenu.isCollisionWithEquipment());
					}
				}
				//if the item is the character
				else if (win->newMenu.isCollisionWithCharacter() >= 0) {
					if (win->newMenu.setCurrCharacter(win->newMenu.isCollisionWithCharacter())) {
					}
				}
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
				win->newState = playScene;
			}
		}
		else if (action == GLFW_RELEASE) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				int _collisionIndex;
				//if the item is in the inventory
				if ((_collisionIndex = win->newMenu.isCollisionWithInventory()) >= 0) {
					win->newMenu.moveItemToInventory();
				}
				//if the item is in the equipment
				else if ((_collisionIndex = win->newMenu.isCollisionWithEquipment()) >= 0) {
					win->newMenu.moveItemToEquipment();
				}
				//if the item is in invalid place
				else {
					win->newMenu.setMovingItem(-1, -1);
				}
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			}
		}
		break;
	case teamMenuScene:
		if (action == GLFW_PRESS) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
				win->newState = playScene;
			}
		}
		else if (action == GLFW_RELEASE) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			}
		}
		break;
	case missionScene:
		if (action == GLFW_PRESS) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
				win->newState = playScene;
			}
		}
		else if (action == GLFW_RELEASE) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {

			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) {

			}
		}
		break;
	default:
		break;
	}
	win->m_mouseButton[button] = action != GLFW_RELEASE;
}

void gameWindow::cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	gameWindow* win = (gameWindow*)glfwGetWindowUserPointer(window);
	win->m_cursor_xpos = (float)xpos;
	win->m_cursor_ypos = (float)ypos;
}

void gameWindow::resize_callback(GLFWwindow * window, int width, int height)
{
	gameWindow* win = (gameWindow*)glfwGetWindowUserPointer(window);
	glViewport(0, 0, width, height);
	win->m_width = width;
	win->m_height = height;
}

void gameWindow::resetDrawProgram()
{
	_DefaultColorProgram->enable();
	_DefaultColorProgram->setOrthoMatrix((float)-m_width / m_height, (float)m_width / m_height, -1.0f, 1.0f, -3.0f, 3.0f);
	_DefaultColorProgram->setScaleMatrix(glm::vec3(testScaleValue, testScaleValue, testScaleValue));
	_DefaultColorProgram->setRotateMatrix(0.0f, 0.0f, 0.0f, 1.0f);
	//m_colorProgram.setColorAndEffect(1.0f, 1.0f, 1.0f, 1.0f, 0.0000001f, 0.0000001f, light_pos_z);
	_DefaultColorProgram->setColorAndEffect(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	_DefaultColorProgram->setTranslateMatrix(0.0f, 0.0f, 0.0f);
	_DefaultColorProgram->disable();
}

void gameWindow::spriteCrusor()
{
	////////// crusor //////////
	////////////////////////////

	//resetDrawProgram();
	_DefaultColorProgram->enable();
	_DefaultColorProgram->setTranslateMatrix(
		-0.5f*(2.0f * crusorSprite.getVexPosx() + crusorSprite.getVexWidth()) + getWindowMousePosx(), 
		-0.5f*(2.0f * crusorSprite.getVexPosy() + crusorSprite.getVexHeight()) + getWindowMousePosy(), 
		0.0f
	);
	//m_colorProgram.setColorAndEffect(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, light_pos_z);
	_DefaultColorProgram->setColorAndEffect(0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f);

	m_texObj->enable(_DefaultColorProgram->getUniformLocation("sampler_1"), crusorTexture);
	crusorSprite.drawSprite();
	m_texObj->disable();

	_DefaultColorProgram->disable();

	////////////////////////////
	/////////// end ////////////
}

void gameWindow::spriteMainScene()
{
	//resetDrawProgram();
	_DefaultColorProgram->enable();
	_DefaultColorProgram->setTranslateMatrix((float)-m_width / m_height - (float)std::fmod(time, 8.0f) / 2, -1.0f, 0.0f);
	m_texObj->enable(_DefaultColorProgram->getUniformLocation("sampler_1"), backgroundTexture);
	backgroundSprite.drawSprite();
	m_texObj->disable();
	_DefaultColorProgram->disable();
}

void gameWindow::setCrusor()
{
	crusorSprite.setSprite(0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 1.0f);
	crusorTexture = m_texObj->setPNGTextureID("Shader/CharacterLeft_Standing.png");
}

void gameWindow::setMainScene()
{
	backgroundTexture = m_texObj->setPNGTextureID("Shader/sky/Daylight Box_0.png");
	backgroundSprite.setSprite(0.0f, 0.0f, 4.0f * 2.0f * (float)m_width / m_height / testScaleValue, 2.0f * (float)m_height / m_height / testScaleValue, 0.0f, 0.0f, 3.0f, 1.0f);
}

float gameWindow::getWindowMousePosx()
{
	return (1.0f / testScaleValue) * ((m_cursor_xpos - (m_width / 2.0f)) / m_width) * 2.0f * m_width / m_height;
}

float gameWindow::getWindowMousePosy()
{
	return (1.0f / testScaleValue) * ((-m_cursor_ypos + (m_height / 2.0f)) / m_height) * 2.0f * m_height / m_height;
}

void gameWindow::testSceneKeyEvnts()
{
	if (isKeyPressed(GLFW_KEY_A)) {
	}
	if (isKeyPressed(GLFW_KEY_D)) {
	}
	if (isKeyPressed(GLFW_KEY_W)) {
	}
	if (isKeyPressed(GLFW_KEY_S)) {
	}

	if (isKeyPressed(GLFW_KEY_PERIOD)) {
		//std::cout << testDegree << std::endl;
		testDegree += 0.1f;
	}

	if (isKeyPressed(GLFW_KEY_COMMA)) {
		//std::cout << testDegree << std::endl;
		testDegree -= 0.1f;
	}

	if (isKeyPressed(GLFW_KEY_R)) {
		testDegree = 0.0f;
		light_pos_z = 1.0f;
		CurrentSpritesLocation_x = 0.0f;
		CurrentSpritesLocation_y = 0.0f;
	}

	if (isKeyPressed(GLFW_KEY_F)) {
	}

	if (isKeyPressed(GLFW_KEY_Z)) {
		light_pos_z++;
	}

	if (isKeyPressed(GLFW_KEY_C)) {
		light_pos_z--;
	}

	if (isKeyPressed(GLFW_KEY_X)) {
	}
}