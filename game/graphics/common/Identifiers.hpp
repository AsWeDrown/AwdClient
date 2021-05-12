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
#define ID_LOADING_OVERLAY         (id_type) 1E+06L
#define ID_DIALOG                  (id_type) 1E+05L
#define ID_BUTTON                  (id_type) 1E+04L
#define ID_TEXT_FIELD              (id_type) 1E+03L
#define ID_WATER_BACKGROUND        (id_type) 1E+02L
#define ID_OTHER                   (id_type) 1E+00L

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   MainMenu Screen
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// MainMenu

#define ID_SCREEN_MAIN_MENU                            1  *  ID_SCREEN

#define ID_SCREEN_MAIN_MENU_WATER_BACKGROUND           ID_SCREEN_MAIN_MENU  \
                                                       +  1  *  ID_WATER_BACKGROUND

#define ID_SCREEN_MAIN_MENU_BUTTON_QUIT_GAME           ID_SCREEN_MAIN_MENU  \
                                                       +  1  *  ID_BUTTON

#define ID_SCREEN_MAIN_MENU_BUTTON_JOIN_LOBBY          ID_SCREEN_MAIN_MENU  \
                                                       +  2  *  ID_BUTTON

#define ID_SCREEN_MAIN_MENU_BUTTON_CREATE_LOBBY        ID_SCREEN_MAIN_MENU  \
                                                       +  3  *  ID_BUTTON

// MainMenu.CreateLobby + MainMenu.JoinLobby

#define ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT                    ID_SCREEN_MAIN_MENU  \
                                                                 +  1  *  ID_DIALOG

#define ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_TEXT_FIELD         ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT  \
                                                                 +  1  *  ID_TEXT_FIELD

#define ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_BUTTON_NEXT        ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT  \
                                                                 +  1  *  ID_BUTTON

#define ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_BUTTON_BACK        ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT  \
                                                                 +  2  *  ID_BUTTON

// MainMenu.Error

#define ID_SCREEN_MAIN_MENU_DIALOG_ERROR                  ID_SCREEN_MAIN_MENU  \
                                                           +  2  *  ID_DIALOG

#define ID_SCREEN_MAIN_MENU_DIALOG_ERROR_BUTTON_OK        ID_SCREEN_MAIN_MENU_DIALOG_ERROR  \
                                                          +  1  *  ID_BUTTON

// MainMenu.LoadingOverlay

#define ID_SCREEN_MAIN_MENU_LOADING_OVERLAY        ID_SCREEN_MAIN_MENU \
                                                   + 1 * ID_LOADING_OVERLAY


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   Lobby Screen
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Lobby

#define ID_SCREEN_LOBBY                           2  *  ID_SCREEN

#define ID_SCREEN_LOBBY_WATER_BACKGROUND          ID_SCREEN_LOBBY  \
                                                  +  1  *  ID_WATER_BACKGROUND

#define ID_SCREEN_LOBBY_BUTTON_LEAVE_LOBBY        ID_SCREEN_LOBBY  \
                                                  +  1  *  ID_BUTTON

#define ID_SCREEN_LOBBY_BUTTON_NEW_GAME           ID_SCREEN_LOBBY  \
                                                  +  2  *  ID_BUTTON

#define ID_SCREEN_LOBBY_BUTTON_LOAD_GAME          ID_SCREEN_LOBBY  \
                                                  +  3  *  ID_BUTTON

// Lobby.Error

#define ID_SCREEN_LOBBY_DIALOG_ERROR                  ID_SCREEN_LOBBY  \
                                                      +  2  *  ID_DIALOG

#define ID_SCREEN_LOBBY_DIALOG_ERROR_BUTTON_OK        ID_SCREEN_LOBBY_DIALOG_ERROR  \
                                                      +  1  *  ID_BUTTON

// Lobby.LoadingOverlay

#define ID_SCREEN_LOBBY_LOADING_OVERLAY        ID_SCREEN_LOBBY \
                                               + 1 * ID_LOADING_OVERLAY


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   Play Screen
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ID_SCREEN_PLAY        3  *  ID_SCREEN


#pragma clang diagnostic pop
