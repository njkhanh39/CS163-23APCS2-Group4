#include "MainMenu.h"

MainMenu::MainMenu(wxWindow* parent) :wxWindow(parent, wxID_ANY) {
	//font
	wxFont font(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wxFont fontTitle(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_NORMAL);
	fontTitle.MakeBold();

	titleBar = new wxPanel(this, 10001, wxDefaultPosition, wxSize(1280, 80));
	titleBar->SetBackgroundColour(wxColor(67, 57, 97));
	mainPanel = new wxPanel(this, 10002, wxDefaultPosition, wxSize(1280, 752));
	mainPanel->SetBackgroundColour(wxColor(34, 36, 40));


	// panel #1 - title bar

	wxStaticText* title = new wxStaticText(titleBar, wxID_ANY, "Main Menu", wxPoint(30, 30));
	title->SetFont(fontTitle);


	// panel #2 - mainPanel
	
	
	to_Search_Word = new wxButton(mainPanel, wxID_ANY,"Search Word", wxPoint(600, 300), wxSize(300,100));
	to_Search_Word->SetFont(fontTitle);

	to_Add_to_Dataset = new wxButton(mainPanel, wxID_ANY, "Add to Dataset", wxPoint(600, 500), wxSize(300, 100));
	to_Add_to_Dataset->SetFont(fontTitle);

	//sizer
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(titleBar, 1, wxEXPAND | wxLEFT | wxUP | wxRIGHT, 2);
	sizer->Add(mainPanel, 5, wxEXPAND | wxALL, 2);

	this->SetSizerAndFit(sizer);
}