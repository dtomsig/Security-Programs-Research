/*******************************************************************************
* FILE: inject.hpp                                                             *
*                                                                              *
* DESCRIPTION: inject.hpp contains methods for interacting with Hearthstone.   *                                                     *
*                                                                              *
* OUTPUT FILE: inject.dll                                                      *
*******************************************************************************/

#ifndef INJECT_HPP
#define INJECT_HPP


#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILDING_INJECT_DLL
#define INJECT_DLL __declspec(dllexport)
#else
#define INJECT_DLL __declspec(dllimport)
#endif


#ifdef __cplusplus
}
#endif
void INJECT_DLL open_ranked_game();
void INJECT_DLL play_card(int c);
#endif
