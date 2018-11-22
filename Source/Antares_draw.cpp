#include "stdafx.h"
/* TODO:
	Add an option to put things off to the side, like the int option, toggle, submenu etc
*/

namespace MenuVariables {

	bool OptionsCenter = true;
	float MenuXCoord = 0.125f; // old = 0.850
}

//Titlebox
int TitleBoxRed = 0;
int TitleBoxGreen = 0;
int TitleBoxBlue = 0;
int TitleBoxALPHA = 255;
bool TitleCenter = true;
//TittleText
int TitleTextRed = 255;
int TitleTextGreen = 0;
int TitleTextBlue = 0;
int TitleTextALPHA = 255;
int TitleFont = 7;
int SubTitleFont = 7;
//Background
int BackgroundRed = 0;
int BackgroundGreen = 0;
int BackgroundBlue = 0;
int BackgroundALPHA = 100;
//options
int OptionsRed = 255;
int OptionsGreen = 255;
int OptionsBlue = 255;
int OptionsALPHA = 255;
float OptionsScale = 0.5f;
int OptionsFont = 6;
//Scroller
int ScrollerRed = 160;
int ScrollerGreen = 160;
int ScrollerBlue = 160;
int ScrollerALPHA = 190;
//Menu
float MenuWidth = 0.21f;


int OptionCount = 0;
int CurrentOption = 1;
int LastOption[100];
int MenuLevel = 0;
bool NewMenu = false;

char* CurrentMenu;
char* LastMenu[100];

bool MenuOpen = false;
bool MenuHasBeenOpened = false;
bool OptionPressed = false;
bool LeftPressed = false;
bool RightPressed = false;
bool LeftRightOption = false;

bool KeyPressed = false;
bool AutoScroll = false;
int AutoScrollKeyNumber = 0;
static int DelayTime_AutoScroll = 25;
DWORD UpdateTime_AutoScroll;

void Menu::Text(char* text, int font, float x, float y, float scaleX, float scaleY, int red, int green, int blue, int alpha, bool center) {

	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(scaleX, scaleY);
	UI::SET_TEXT_COLOUR(red, green, blue, alpha);
	UI::SET_TEXT_WRAP(0.0f, 1.0f);
	UI::SET_TEXT_CENTRE(center);
	UI::SET_TEXT_OUTLINE();
	UI::_SET_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	UI::_DRAW_TEXT(x, y);
}

void Menu::Texture(char* streamedTexture, char* textureName, float x, float y, float width, float height, float rotation, int red, int green, int blue, int alpha) {

	if (GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(streamedTexture))
		GRAPHICS::DRAW_SPRITE(streamedTexture, textureName, x, y, width, height, rotation, red, green, blue, alpha);
	else
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(streamedTexture, false);
}



void Menu::Title(char* Tittle) {

	Text(Tittle, TitleFont, MenuVariables::MenuXCoord, 0.09f, 0.80f, 0.80f, TitleTextRed, TitleTextGreen, TitleTextBlue, TitleTextALPHA, TitleCenter); // if needed this can be changed to title x coords
	Texture("CommonMenu", "interaction_bgd", MenuVariables::MenuXCoord, 0.1175f, MenuWidth, 0.085f, 180, TitleBoxRed, TitleBoxGreen, TitleBoxBlue, TitleBoxALPHA);

	if (NewMenu) {
		OptionCount = 0;
		NewMenu = false;
	}
}

void Menu::SubTitle(char* SubTitle) {

	Text(SubTitle, 7, MenuVariables::MenuXCoord, 0.132f, 0.50f, 0.40f, TitleTextRed, TitleTextGreen, TitleTextBlue, TitleTextALPHA, TitleCenter);
}

bool Menu::Option(char* Option) {

	OptionCount++;
	if (CurrentOption <= 16 && OptionCount <= 16) {
		if (MenuVariables::OptionsCenter)
			Text(Option, OptionsFont, MenuVariables::MenuXCoord, (OptionCount * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, true);
		else
			Text(Option, OptionsFont, (MenuVariables::MenuXCoord - 0.1f), (OptionCount * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);
	}
	else if ((OptionCount > (CurrentOption - 16)) && OptionCount <= CurrentOption) {
		if (MenuVariables::OptionsCenter)
			Text(Option, OptionsFont, MenuVariables::MenuXCoord, ((OptionCount - (CurrentOption - 16)) * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, true);
		else
			Text(Option, OptionsFont, (MenuVariables::MenuXCoord - 0.1f), ((OptionCount - (CurrentOption - 16)) * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);
	}
	if (CurrentOption == OptionCount && OptionPressed) {
		return true;
	}

	else
		return false;
}

bool Menu::ToggleOption(char* option, bool* toggle) {

	bool ToggleBool = Option(option);

	if (CurrentOption <= 16 && OptionCount <= 16)
		Text((*toggle ? "~g~ON" : "~r~OFF"), OptionsFont, (MenuVariables::MenuXCoord + 0.08f), (OptionCount * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);
	else if ((OptionCount > (CurrentOption - 16)) && OptionCount <= CurrentOption)
		Text((*toggle ? "~g~ON" : "~r~OFF"), OptionsFont, (MenuVariables::MenuXCoord + 0.08f), ((OptionCount - (CurrentOption - 16)) * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);

	if (ToggleBool) {
		if (*toggle)
			*toggle = false;
		else if (*toggle == false)
			*toggle = true;
		return true;
	}

	return false;
}

bool Menu::IntOption(char* option, int* var, int changeBy, int minimumValue, int maximumValue) {

	LeftRightOption = true;
	bool Pressed = Option(option);

	char buff[40];
	_snprintf_s(buff, sizeof(buff), "%i", *var);
	if (CurrentOption <= 16 && OptionCount <= 16)
		Text(buff, OptionsFont, (MenuVariables::MenuXCoord + 0.08f), (OptionCount * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);
	else if ((OptionCount > (CurrentOption - 16)) && OptionCount <= CurrentOption)
		Text(buff, OptionsFont, (MenuVariables::MenuXCoord + 0.08f), ((OptionCount - (CurrentOption - 16)) * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);

	if (CurrentOption == OptionCount) {

		if (RightPressed) {
			if (*var >= maximumValue)
				*var = minimumValue;
			else
				*var = *var + changeBy;
			RightPressed = false;
		}

		if (LeftPressed) {
			if (*var <= minimumValue)
				*var = maximumValue;
			else
				*var = *var - changeBy;
			LeftPressed = false;
		}
	}

	if (Pressed)
		return true;
	return false;
}

bool Menu::FloatOption(char* option, float* var, float changeBy, float minimumValue, float maximumValue) {

	LeftRightOption = true;
	bool Pressed = Option(option);

	char buff[40];
	_snprintf_s(buff, sizeof(buff), "%.2f", *var);
	if (CurrentOption <= 16 && OptionCount <= 16)
		Text(buff, OptionsFont, (MenuVariables::MenuXCoord + 0.08f), (OptionCount * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);
	else if ((OptionCount > (CurrentOption - 16)) && OptionCount <= CurrentOption)
		Text(buff, OptionsFont, (MenuVariables::MenuXCoord + 0.08f), ((OptionCount - (CurrentOption - 16)) * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);

	if (CurrentOption == OptionCount) {

		if (RightPressed) {
			if (*var >= maximumValue)
				*var = minimumValue;
			else
				*var = *var + changeBy;
			RightPressed = false;
		}

		if (LeftPressed) {
			if (*var <= minimumValue)
				*var = maximumValue;
			else
				*var = *var - changeBy;
			LeftPressed = false;
		}
	}

	if (Pressed)
		return true;
	return false;
}

void Menu::PlayerOption(char* option, int playerNumber, int* selecterPlayer, char** holdName) {

	//char buff[38];
	//_snprintf_s(buff, sizeof(buff), "%s   ~r~->", option);


	bool CallChange = Option(option);

	if (CurrentOption <= 16 && OptionCount <= 16)
		Text("~r~->", OptionsFont, (MenuVariables::MenuXCoord + 0.08f), (OptionCount * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);
	else if ((OptionCount > (CurrentOption - 16)) && OptionCount <= CurrentOption)
		Text("~r~->", OptionsFont, (MenuVariables::MenuXCoord + 0.08f), ((OptionCount - (CurrentOption - 16)) * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);

	if (CallChange) {
		*selecterPlayer = playerNumber;
		*holdName = option;
		ChangeSubmenu("PlayerOptions");
	}
}

Ped playerPed = PLAYER::PLAYER_PED_ID();

void Menu::TeleportOptionC(char* option, float x, float y, float z) {

	if (Option(option)) {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, FALSE))
			ENTITY::SET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_USING(playerPed), x, y, z, TRUE, FALSE, FALSE, TRUE);
		else
			ENTITY::SET_ENTITY_COORDS(playerPed, x, y, z, TRUE, FALSE, FALSE, TRUE);
	}
}

void Menu::Submenu(char* option, char* menuName) {

	//char buff[38];
	//_snprintf_s(buff, sizeof(buff), "%s   ~r~->", option);

	bool CallChange = Option(option);

	if (CurrentOption <= 16 && OptionCount <= 16)
		Text("~r~->", OptionsFont, (MenuVariables::MenuXCoord + 0.08f), (OptionCount * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);
	else if ((OptionCount > (CurrentOption - 16)) && OptionCount <= CurrentOption)
		Text("~r~->", OptionsFont, (MenuVariables::MenuXCoord + 0.08f), ((OptionCount - (CurrentOption - 16)) * 0.035f + 0.125f), OptionsScale, OptionsScale, OptionsRed, OptionsGreen, OptionsBlue, OptionsALPHA, false);

	if (CallChange)
		ChangeSubmenu(menuName);

}




void Menu::SetActiveSubmenu(char* menu, int pos) {

	NewMenu = true;
	CurrentMenu = menu;
	MenuLevel = pos;
	CurrentOption = 1;
}

void Menu::MenuBack() {
	NewMenu = true;
	MenuLevel--;
	CurrentOption = LastOption[MenuLevel];
	CurrentMenu = LastMenu[MenuLevel];
	LastMenu[MenuLevel] = ""; // dont need to do it for the option as it is just an int
}

void Menu::ChangeSubmenu(char* newMenuName) {

	NewMenu = true;
	OptionPressed = false;
	LastMenu[MenuLevel] = CurrentMenu;
	LastOption[MenuLevel] = CurrentOption;
	CurrentOption = 1;
	CurrentMenu = newMenuName;
	MenuLevel++;
}

void Menu::EndMenu() {

	if (OptionCount > 16) {

		GRAPHICS::DRAW_RECT(MenuVariables::MenuXCoord, (((16 * 0.035f) / 2) + 0.159f), MenuWidth, (16 * 0.035f), BackgroundRed, BackgroundGreen, BackgroundBlue, BackgroundALPHA);
		if (CurrentOption > 16)
			GRAPHICS::DRAW_RECT(MenuVariables::MenuXCoord, ((16 * 0.035f) + 0.1415f), MenuWidth, 0.035f, ScrollerRed, ScrollerGreen, ScrollerBlue, ScrollerALPHA);
		else
			GRAPHICS::DRAW_RECT(MenuVariables::MenuXCoord, ((CurrentOption * 0.035f) + 0.1415f), MenuWidth, 0.035f, ScrollerRed, ScrollerGreen, ScrollerBlue, ScrollerALPHA);
	}

	else if (OptionCount > 0) {
		GRAPHICS::DRAW_RECT(MenuVariables::MenuXCoord, (((OptionCount * 0.035f) / 2) + 0.159f), MenuWidth, (OptionCount * 0.035f), BackgroundRed, BackgroundGreen, BackgroundBlue, BackgroundALPHA);
		GRAPHICS::DRAW_RECT(MenuVariables::MenuXCoord, ((CurrentOption * 0.035f) + 0.1415f), MenuWidth, 0.035f, ScrollerRed, ScrollerGreen, ScrollerBlue, ScrollerALPHA);
	}

	char buff[38];
	_snprintf_s(buff, sizeof(buff), "%i / %i", CurrentOption, OptionCount);
	if (OptionCount > 16) {
		Text(buff, TitleFont, (MenuVariables::MenuXCoord + 0.08f), (17 * 0.035f + 0.130f), 0.3f, 0.3f, TitleTextRed, TitleTextGreen, TitleTextBlue, TitleTextALPHA, TitleCenter);//(17 * 0.035f + 0.125f)
		Texture("CommonMenu", "interaction_bgd", MenuVariables::MenuXCoord, (17 * 0.035f + 0.140f), MenuWidth, 0.035f, 180, TitleBoxRed, TitleBoxGreen, TitleBoxBlue, TitleBoxALPHA); // was interaction_bgd
	}
	else {
		Text(buff, TitleFont, (MenuVariables::MenuXCoord + 0.08f), ((OptionCount + 1) * 0.035f + 0.130f), 0.3f, 0.3f, TitleTextRed, TitleTextGreen, TitleTextBlue, TitleTextALPHA, TitleCenter); 
		Texture("CommonMenu", "interaction_bgd", MenuVariables::MenuXCoord, ((OptionCount + 1) * 0.035f + 0.140f), MenuWidth, 0.035f, 180, TitleBoxRed, TitleBoxGreen, TitleBoxBlue, TitleBoxALPHA);
	}

	OptionPressed = false;
}



void Menu::AboveMapNotification(char* message, BOOL blink) {

	UI::SET_TEXT_OUTLINE();
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(&message[0u]);
	UI::_DRAW_NOTIFICATION(blink, FALSE);
}



void Menu::MenuSounds(char* soundName, char* soundDict) {
	AUDIO::PLAY_SOUND_FRONTEND(-1, soundName, soundDict, 0);
}



bool Menu::HasEnteredNewMenu() {
	return NewMenu;
}

void Menu::SetOptionCount(int count) {
	OptionCount = count;
}

char* Menu::GetOwnerSubAtPos(int pos) {

	return LastMenu[pos];
}

char* Menu::GetActiveMenu() {
	return CurrentMenu;
}

bool Menu::IsOptionPressed() {
	return OptionPressed;
}

int Menu::GetCurrentOption() {
	return CurrentOption;
}

bool Menu::HasBeenOpened() {
	return MenuHasBeenOpened;
}

bool Menu::IsMenuOpen() {
	return MenuOpen;
}






void ResetAutoScroll() {
	AutoScroll = false;
	KeyPressed = false;
	AutoScrollKeyNumber = 0;
	ResetKeyBoardTimer();
}

bool FirstOrLastOption(int keyNumber) {
	if (((CurrentOption - 1) < 1) && keyNumber == 8) {
		CurrentOption = OptionCount;
		return true;
	}
	if (((CurrentOption + 1) > OptionCount) && keyNumber == 2) {
		CurrentOption = 1;
		return true;
	}
	return false;
}

void SetAutoScrollActive(int scrollKey) {
	AutoScroll = true;
	AutoScrollKeyNumber = scrollKey;
}

void Menu::MenuControls() {

	if (KeyJustUp(VK_F4))
		Hooking::Cleanup();

	if (KeyJustUp(VK_MULTIPLY)) {

		if (!MenuHasBeenOpened) {
			MenuHasBeenOpened = true;
			MenuSounds("Hack_Success", "DLC_HEIST_BIOLAB_PREP_HACKING_SOUNDS");
		}

		MenuOpen = !MenuOpen;

		if (MenuOpen) {
			OptionPressed = false;
			CurrentOption = 1;
			CurrentMenu = "MainMenu";
			MenuLevel = 0;
			MenuSounds("SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET");
			ResetAutoScroll();
		}

		else if (!MenuOpen)
			MenuSounds("BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET");

	}

	if (MenuOpen) {

		if (KeyJustUp(VK_NUMPAD5) && !KeysPressed(5)) {
			OptionPressed = true;
			MenuSounds("SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET");
			AutoScroll = false;
			AutoScrollKeyNumber = 0;
		}

		if (KeyJustUp(VK_NUMPAD0)) { // dont need key check here
			
			if (CurrentMenu == "MainMenu")
				MenuOpen = false;

			else {
				AutoScroll = false;
				AutoScrollKeyNumber = 0;
				MenuBack();
			}

			MenuSounds("BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET");
		}


		if (KeyDown(VK_NUMPAD8) && !KeysPressed(8)) {
			
			if (!KeyPressed) {
				KeyBoardTimer(400);
				KeyPressed = true;
				if (!FirstOrLastOption(8))
					CurrentOption--;
				MenuSounds("NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET");
			}

			if (KeyBoardTimerReady())
				SetAutoScrollActive(8);

		}

		if (KeyDown(VK_NUMPAD2) && !KeysPressed(2)) {
			
			if (!KeyPressed) {
				KeyBoardTimer(400);
				KeyPressed = true;
				if (!FirstOrLastOption(2))
					CurrentOption++;
				MenuSounds("NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET");
			}

			if (KeyBoardTimerReady()) 
				SetAutoScrollActive(2);
		}
		
		if (LeftRightOption) {

			if (KeyDown(VK_NUMPAD4) && !KeysPressed(4)) {

				if (!KeyPressed) {
					KeyBoardTimer(400);
					KeyPressed = true;
					LeftPressed = true;
					MenuSounds("NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET");
				}

				if (KeyBoardTimerReady())
					SetAutoScrollActive(4);
			}

			if (KeyDown(VK_NUMPAD6) && !KeysPressed(6)) {

				if (!KeyPressed) {
					KeyBoardTimer(400);
					KeyPressed = true;
					RightPressed = true;
					MenuSounds("NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET");
				}

				if (KeyBoardTimerReady())
					SetAutoScrollActive(6);
			}
		}

		if (KeyJustUp(VK_NUMPAD8) || KeyJustUp(VK_NUMPAD2) || KeyJustUp(VK_NUMPAD4) || KeyJustUp(VK_NUMPAD6))
			ResetAutoScroll();


		if (AutoScroll) {

			if (UpdateTime_AutoScroll + DelayTime_AutoScroll < GetTickCount()) {
				UpdateTime_AutoScroll = GetTickCount();

				if (AutoScrollKeyNumber == 8) {
					if (!FirstOrLastOption(8))
						CurrentOption--;
					MenuSounds("NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET");
				}

				if (AutoScrollKeyNumber == 2) {
					if (!FirstOrLastOption(2))
						CurrentOption++;
					MenuSounds("NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET");
				}

				if (AutoScrollKeyNumber == 4) {
					LeftPressed = true;
					MenuSounds("NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET");
				}

				if (AutoScrollKeyNumber == 6) {
					RightPressed = true;
					MenuSounds("NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET");
				}
			}

		}

	}

	if (LeftRightOption)
		LeftRightOption = false;

}