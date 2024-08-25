#pragma once
#ifndef QuizMenu_h
#include "wx/wx.h"
#include <wx/simplebook.h>
#include <wx/spinctrl.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>

#include "Dictionary.h"
#include "GUI_WordView.h"
#include "Word.h"

class QuizMenu : public wxSimplebook {
public:

	QuizMenu(wxWindow* parent);

private:
	
	vector<string> GenQuest(wxCommandEvent& evt, Dictionary* dict, int& correctans, string& quest);

	wxColour purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);

	void displayGameMode();
	
	void displayQuestion();

	string GetWordType(Word word, string& str);

	wxTimer timer;

	/*==============GAME_VARIABLE==============*/
	int current_question = 0, score = 0, numquest = 30;
	int gametype = 0; //0: Guess Def; 1: GuessWord
	int dataset = 0; //0: Eng-Eng, 1: Eng-Viet, 2: Viet-Eng
	bool is_endless = false;
	void DefaultSetting();
	/*=========================================*/

	wxPanel* gamevar;
	wxStaticText* game, * datachose, * numberques;
	wxButton* GuessWord, * GuessDef, * EngEng, * EngVie, * VieEng, *start, *endlessmode, *displaynum;
	wxBitmapButton* increase, * decrease;

	/*=================QUESTION================*/
	wxPanel* question;
	wxStaticBox* chosen_box;
	wxStaticText* chosen_quest;
	wxButton* options[4], exit;
	/*=========================================*/

	/*=================SCORE===================*/
	wxPanel* score_announce;
	wxStaticText* announce;
	wxButton* restart;
	/*=========================================*/
};
#endif