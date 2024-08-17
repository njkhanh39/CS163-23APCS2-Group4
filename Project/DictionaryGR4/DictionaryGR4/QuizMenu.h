#pragma once
#ifndef QuizMenu_h
#include "wx/wx.h"
#include <wx/simplebook.h>
#include <wx/spinctrl.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>

class QuizMenu : public wxPanel {
public:

	QuizMenu(wxWindow* parent);

private:
	wxStaticText* game,* datachose,* numberques;
	wxButton* GuessWord, * GuessDef, *EngEng, *EngVie, *VieEng;
	wxBitmapButton* increase, * decrease; 

	/*==============GAME_VARIABLE==============*/
	int current_question = 0, score = 0, numquest = 30;
	int gametype = 0; //0: Guess Def; 1: GuessWord
	int dataset = 0; //0: Eng-Eng, 1: Eng-Viet, 2: Viet-Eng
	void DefaultSetting();
	/*=========================================*/

};
#endif