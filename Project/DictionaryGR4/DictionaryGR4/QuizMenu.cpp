#include "MainFrame.h"
#include "QuizMenu.h"

QuizMenu::QuizMenu(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

	auto purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);
	SetBackgroundColour(black);

	game = new wxStaticText(this, wxID_ANY,"Choose Game Type", wxPoint(601,100));
	datachose = new wxStaticText(this, wxID_ANY, "Choose Dataset", wxPoint(562, 280));
	numberques = new wxStaticText(this, wxID_ANY, "Choose Number of Questions", wxPoint(100, 300));

	game->SetForegroundColour(white);
	datachose->SetForegroundColour(white);
	numberques->SetForegroundColour(white);

	GuessWord = new wxButton(this, wxID_ANY, "GUESS WORD", wxPoint(238, 140), wxSize(402, 70));
	GuessDef = new wxButton(this, wxID_ANY, "GUESS DEFINITION", wxPoint(640, 140), wxSize(402, 70));
	 

}

void QuizMenu::DefaultSetting()
{
	current_question = 0; score = 0; numquest = 30, gametype = 0, dataset = 0;
}