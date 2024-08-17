#include "MainFrame.h"
#include "QuizMenu.h"

QuizMenu::QuizMenu(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

	auto purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);
	SetBackgroundColour(black);

}