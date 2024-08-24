#include "MainFrame.h"
#include "QuizMenu.h"

QuizMenu::QuizMenu(wxWindow* parent) : wxSimplebook(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

	auto purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);
	//SetBackgroundColour(black);

	/*GAME MODE*/
	gamevar = new wxPanel(this, wxID_ANY,wxDefaultPosition,wxSize(1280,720),wxBORDER_NONE);
	gamevar->SetBackgroundColour(black);


	game = new wxStaticText(gamevar, wxID_ANY,"Choose Game Type", wxPoint(601,100));
	datachose = new wxStaticText(gamevar, wxID_ANY, "Choose Dataset", wxPoint(562, 280));
	numberques = new wxStaticText(gamevar, wxID_ANY, "Choose Number of Questions", wxPoint(100, 300));

	game->SetForegroundColour(white);
	datachose->SetForegroundColour(white);
	numberques->SetForegroundColour(white);

	GuessWord = new wxButton(gamevar, wxID_ANY, "GUESS WORD", wxPoint(238, 140), wxSize(402, 70));
	GuessDef = new wxButton(gamevar, wxID_ANY, "GUESS DEFINITION", wxPoint(640, 140), wxSize(402, 70));

	GuessWord->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		gametype = 0;
		GuessWord->SetBackgroundColour(wxColour(101, 86, 142));
		GuessDef->SetBackgroundColour(wxColour(255, 255, 255));
		});

	GuessDef->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		gametype = 1;
		GuessDef->SetBackgroundColour(wxColour(101, 86, 142));
		GuessWord->SetBackgroundColour(wxColour(255, 255, 255));
		});

	/**/

	/*QUESTION*/
	question = new wxPanel(this,wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);
	question->SetBackgroundColour(black);

	wxPoint boxPosition(78, 80);
	wxSize boxSize(1124, 290);

	chosen_box = new wxStaticBox(question, wxID_ANY, "", boxPosition, boxSize, wxBORDER_NONE);
	chosen_box->SetBackgroundColour(white);

	chosen_quest = new wxStaticText(chosen_box,wxID_ANY,"Example");

	// Calculate center position within the static box
	int textWidth = chosen_quest->GetBestSize().GetWidth();
	int textHeight = chosen_quest->GetBestSize().GetHeight();
	int centerX = (boxSize.GetWidth() - textWidth) / 2;
	int centerY = (boxSize.GetHeight() - textHeight) / 2;

	// Position the wxStaticText manually to be centered
	chosen_quest->SetPosition(wxPoint(centerX, centerY));

	/**/

	/*RESULT*/

	/**/

	this->AddPage(question, "Question", true);
	this->AddPage(gamevar,"Game variable", true);

	this->SetSelection(0);

}

void QuizMenu::DefaultSetting()
{
	current_question = 0; score = 0; numquest = 30, gametype = 0, dataset = 0;
}