#pragma once
#ifndef QuizMenu_h
#include "wx/wx.h"
#include <wx/simplebook.h>
#include <wx/spinctrl.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>

class QuizMenu : public wxWindow {
public:

	QuizMenu(wxWindow* parent);

private:
	/*===============NaviPane==========================*/
	wxButton* searchpane, * addpane, * quizpane, * hispane, * favpane;
	wxPanel* titleBar;
	/*=================================================*/

};
#endif