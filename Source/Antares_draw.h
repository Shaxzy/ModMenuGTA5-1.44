#pragma once

/* TODO:
	Add bottom notification.
*/

namespace MenuVariables {

	extern bool OptionsCenter;
	extern float MenuXCoord;
}

class Menu {

public:
	static void Text(char* text, int font, float x, float y, float scaleX, float scaleY, int red, int green, int blue, int alpha, bool center);
	static void Texture(char* streamedTexture, char* textureName, float x, float y, float width, float height, float rotation, int red, int green, int blue, int alpha);

	static void Title(char* tittle);
	static void SubTitle(char* subTitle);

	static bool Option(char* option);
	static bool ToggleOption(char* option, bool *toggle);
	static bool IntOption(char* option, int* var, int changeBy, int minimumValue, int maximumValue);
	static bool FloatOption(char* option, float* var, float changeBy, float minimumValue, float maximumValue);
	static void PlayerOption(char* option, int playerNumber, int* selecterPlayer, char** holdName);
	static void TeleportOptionC(char* option, float x, float y, float z);
	static void Submenu(char* option, char* menuName);

	static void SetActiveSubmenu(char* menu, int pos);
	static void MenuBack();
	static void ChangeSubmenu(char* newMenuName);
	static void EndMenu();

	static void AboveMapNotification(char* message, BOOL blink);

	static void MenuSounds(char* soundName, char* soundDict);

	static bool HasEnteredNewMenu();
	static void SetOptionCount(int count);
	static char* GetOwnerSubAtPos(int pos); // position is the number of submenus in. For example MainMenu = pos 0; Player Menu = 1; because its 
	static char* GetActiveMenu();
	static bool IsOptionPressed();
	static int GetCurrentOption();
	static bool HasBeenOpened();
	static bool IsMenuOpen();

	static void MenuControls();
};