#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TODO - прописывать ID всех графических элементов в этом файле.
//        ВАЖНО: каждый ID должен быть уникальным в рамках всей программы.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   Types
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef unsigned long long id_type;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   Base
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ID_SCREEN                  (id_type) 1E+10L
#define ID_DIALOG                  (id_type) 1E+08L
#define ID_BUTTON                  (id_type) 1E+06L
#define ID_TEXT_FIELD              (id_type) 1E+04L
#define ID_WATER_BACKGROUND        (id_type) 1E+02L

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *   MainMenu
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

// MainMenu.CreateLobby

#define ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY                               ID_SCREEN_MAIN_MENU  \
                                                                              +  1  *  ID_DIALOG

#define ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY_TEXT_FIELD_PLAYER_NAME        ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY  \
                                                                              +  1  *  ID_TEXT_FIELD

#define ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY_BUTTON_NEXT                   ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY  \
                                                                              +  1  *  ID_BUTTON

#define ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY_BUTTON_BACK                   ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY  \
                                                                              +  2  *  ID_BUTTON



