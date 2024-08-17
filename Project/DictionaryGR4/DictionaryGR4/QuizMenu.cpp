#include "MainFrame.h"
#include "QuizMenu.h"

QuizMenu::QuizMenu(wxWindow* parent) :wxWindow(parent, wxID_ANY) {

	auto purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);

	/*===============NaviPane==========================*/
	titleBar = new wxPanel(this, 10001, wxDefaultPosition, wxSize(1280, 80), wxBORDER_NONE);
	titleBar->SetBackgroundColour(purple);

	searchpane = new wxButton(titleBar, wxID_ANY, "SEARCH", wxPoint(344, 18), wxSize(147, 44), wxBORDER_NONE);
	searchpane->SetBackgroundColour(purple);
	searchpane->SetForegroundColour(white);

	addpane = new wxButton(titleBar, wxID_ANY, "ADD TO DATASET", wxPoint(524, 18), wxSize(147, 44), wxBORDER_NONE);
	addpane->SetBackgroundColour(purple);
	addpane->SetForegroundColour(white);

	quizpane = new wxButton(titleBar, wxID_ANY, "QUIZ GAME", wxPoint(704, 18), wxSize(147, 44), wxBORDER_NONE);
	quizpane->SetBackgroundColour(black);
	quizpane->SetForegroundColour(white);

	hispane = new wxButton(titleBar, wxID_ANY, "HISTORY", wxPoint(884, 18), wxSize(147, 44), wxBORDER_NONE);
	hispane->SetBackgroundColour(purple);
	hispane->SetForegroundColour(white);

	favpane = new wxButton(titleBar, wxID_ANY, "FAVOURITE", wxPoint(1064, 18), wxSize(147, 44), wxBORDER_NONE);
	favpane->SetBackgroundColour(purple);
	favpane->SetForegroundColour(white);
	/*================================================*/



}