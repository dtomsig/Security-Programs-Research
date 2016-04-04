/*******************************************************************************
* FILE: inject.h                                                               *
*                                                                              *
* DESCRIPTION: inject.cpp contains methods for interacting with Hearthstone.   *                                                     *
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
void INJECT_DLL openRankedGame();
void INJECT_DLL playCard(int c);
#endif
