#pragma once

#include "MainMenu.h"
#include "SearchMenu.h"
#include "GUI_AddWordMenu.h"
#include "QuizMenu.h"
#include "HistoryMenu.h"
class MainFrame: public wxFrame {
private:
	wxSimplebook* simpleBook;

public: 
	MainFrame(Dictionary*& dict, const wxString& Title, wxSize FrameSize);

    void OnPreviousPage(wxCommandEvent& event);

	void OnNextPage(wxCommandEvent& event);

	wxColour purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);

	wxPanel* NavPane;
	wxButton* searchpane, * addpane, * quizpane, * hispane, * favpane;
};