#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-macro-parentheses"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   TODO - прописывать ID всех графических элементов в этом файле.
//          ВАЖНО: каждый ID должен быть уникальным в рамках всей программы.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   Типы
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef unsigned long long id_type;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   Основа
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ID_SCREEN                  (id_type) 1E+16L
#define ID_WORLD                   (id_type) 1E+14L
#define ID_LOADING_OVERLAY         (id_type) 1E+12L
#define ID_DIALOG                  (id_type) 1E+10L
#define ID_SUBCOMPONENT            (id_type) 1E+00L

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   MainMenu Screen
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// MainMenu

#define ID_SCREEN_MAIN_MENU                            1  *  ID_SCREEN

#define ID_SCREEN_MAIN_MENU_WATER_BACKGROUND           ID_SCREEN_MAIN_MENU  \
                                                       +  1  *  ID_SUBCOMPONENT

#define ID_SCREEN_MAIN_MENU_BUTTON_QUIT_GAME           ID_SCREEN_MAIN_MENU  \
                                                       +  2  *  ID_SUBCOMPONENT

#define ID_SCREEN_MAIN_MENU_BUTTON_JOIN_LOBBY          ID_SCREEN_MAIN_MENU  \
                                                       +  3  *  ID_SUBCOMPONENT

#define ID_SCREEN_MAIN_MENU_BUTTON_CREATE_LOBBY        ID_SCREEN_MAIN_MENU  \
                                                       +  4  *  ID_SUBCOMPONENT

// MainMenu.CreateLobby + MainMenu.JoinLobby

#define ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT                    ID_SCREEN_MAIN_MENU  \
                                                                 +  1  *  ID_DIALOG

#define ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_TEXT_FIELD         ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT  \
                                                                 +  1  *  ID_SUBCOMPONENT

#define ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_BUTTON_NEXT        ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT  \
                                                                 +  2  *  ID_SUBCOMPONENT

#define ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_BUTTON_BACK        ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT  \
                                                                 +  3  *  ID_SUBCOMPONENT

// MainMenu.Error

#define ID_SCREEN_MAIN_MENU_DIALOG_ERROR                  ID_SCREEN_MAIN_MENU  \
                                                           +  2  *  ID_DIALOG

#define ID_SCREEN_MAIN_MENU_DIALOG_ERROR_BUTTON_OK        ID_SCREEN_MAIN_MENU_DIALOG_ERROR  \
                                                          +  1  *  ID_SUBCOMPONENT

// MainMenu.LoadingOverlay

#define ID_SCREEN_MAIN_MENU_LOADING_OVERLAY        ID_SCREEN_MAIN_MENU  \
                                                   +  1  *  ID_LOADING_OVERLAY


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   Lobby Screen
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Lobby

#define ID_SCREEN_LOBBY                           2  *  ID_SCREEN

#define ID_SCREEN_LOBBY_PLAYER_CARD               ID_SCREEN_LOBBY  \
                                                  +  (id_type) 1E+10L

#define ID_SCREEN_LOBBY_WATER_BACKGROUND          ID_SCREEN_LOBBY  \
                                                  +  1  *  ID_SUBCOMPONENT

#define ID_SCREEN_LOBBY_BUTTON_LEAVE_LOBBY        ID_SCREEN_LOBBY  \
                                                  +  2  *  ID_SUBCOMPONENT

#define ID_SCREEN_LOBBY_BUTTON_NEW_GAME           ID_SCREEN_LOBBY  \
                                                  +  3  *  ID_SUBCOMPONENT

#define ID_SCREEN_LOBBY_BUTTON_LOAD_GAME          ID_SCREEN_LOBBY  \
                                                  +  4  *  ID_SUBCOMPONENT

// Lobby.Error

#define ID_SCREEN_LOBBY_DIALOG_ERROR                  ID_SCREEN_LOBBY  \
                                                      +  1  *  ID_DIALOG

#define ID_SCREEN_LOBBY_DIALOG_ERROR_BUTTON_OK        ID_SCREEN_LOBBY_DIALOG_ERROR  \
                                                      +  1  *  ID_SUBCOMPONENT

// Lobby.LoadingOverlay

#define ID_SCREEN_LOBBY_LOADING_OVERLAY        ID_SCREEN_LOBBY  \
                                               +  1  *  ID_LOADING_OVERLAY


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   Play Screen
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ID_SCREEN_PLAY                                      3  *  ID_SCREEN

#define ID_SCREEN_PLAY_WORLD                                ID_SCREEN_PLAY  \
                                                            +  1  *  ID_WORLD

#define ID_SCREEN_PLAY_QUEST_MANAGER                        ID_SCREEN_PLAY  \
                                                            +  2  *  ID_WORLD

#define ID_SCREEN_PLAY_QUEST_MANAGER_QUEST_BALLOON          ID_SCREEN_PLAY_QUEST_MANAGER  \
                                                            +  (id_type) 1E+10L

#define ID_SCREEN_PLAY_WORLD_WATER_BACKGROUND               ID_SCREEN_PLAY_WORLD  \
                                                            +  1  *  ID_SUBCOMPONENT

#define ID_SCREEN_PLAY_WORLD_ENTITY                         ID_SCREEN_PLAY_WORLD  \
                                                            +  (id_type) 1E+10L

#pragma clang diagnostic pop
