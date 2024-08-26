#pragma once
#ifndef QuizMenu_h
#include "wx/wx.h"
#include <wx/simplebook.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>

#include "Dictionary.h"
#include "GUI_WordView.h"
#include "Word.h"
#include "randomfunc.h"
#include "App.h"
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/tokenzr.h> 


class QuizMenu : public wxSimplebook {
public:

	QuizMenu(wxWindow* parent, Dictionary*& dict);

private:
	
	vector<string> GenQuest(Dictionary* dict, int& correctans, string& quest);

	wxColour purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);

	Word RandomWord(string& wordText, Dictionary* dict);

	void displayGameMode(Dictionary* dict);

	void displayscore();
	
	void modeQuestion(wxSize boxSize);

	void OnOptionSelected(wxCommandEvent& event);
	void OnNextQuestion(Dictionary* dict);

	void processQuestion(Dictionary* dict);

	wxString getlabelLineBreak(wxButton* opt);

	string GetWordType(Word word, string& str);

	wxTimer timer;

	wxPoint boxPosition = wxPoint(78, 40), optPosition[4] =  {wxPoint(78, 348),wxPoint(648, 348),wxPoint(78, 494) ,wxPoint(648, 494) };
	wxSize boxSize = wxSize(1124, 292), optSize = wxSize(554,130);

	/*==============GAME_VARIABLE==============*/
	int current_question = 0, score = 0, numquest = 30, sum_quest = 0;
	int gametype = 0; //0: Guess Def; 1: GuessWord
	int dataset = 0; //0: Eng-Eng, 1: Eng-Viet, 2: Viet-Eng
	bool is_endless = false;
	void DefaultSetting(Dictionary* dict);
	/*=========================================*/

	wxPanel* gamevar;
	wxStaticText* game, * datachose, * numberques;
	wxButton* GuessWord, * GuessDef, * EngEng, * EngVie, * VieEng, *start, *endlessmode, *displaynum;
	wxBitmapButton* increase, * decrease;

	/*=================QUESTION================*/
	int pos_correct;
	wxPanel* question;
	wxStaticBox* chosen_box;
	wxStaticText* chosen_quest,* currquestno;
	wxButton* exit, *options[4];
	wxBitmapButton* nextquest;
	/*=========================================*/

	/*=================SCORE===================*/
	wxPanel* score_announce;
	wxStaticText* announce;
	wxButton* restart, *backmenu;
	/*=========================================*/


};

#endif