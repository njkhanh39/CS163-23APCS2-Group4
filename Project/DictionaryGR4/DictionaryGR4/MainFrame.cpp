#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>
using namespace std;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

	//font
	wxFont font(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wxFont fontTitle(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_NORMAL);
	fontTitle.MakeBold();

	panel = new wxPanel(this, 10001, wxDefaultPosition, wxSize(800, 100));
	panel->SetBackgroundColour(wxColor(100, 100, 200));
	panel2 = new wxPanel(this, 10002, wxDefaultPosition, wxSize(800, 500));
	
	wxBitmap bitmap(wxT("image.png"), wxBITMAP_TYPE_PNG);

	wxStaticText* titleBar = new wxStaticText(panel, wxID_ANY, "Dictionary", wxPoint(30, 30));
	titleBar->SetFont(fontTitle);

	// Create a wxBitmapButton
	button = new wxBitmapButton(panel2 , wxID_ANY, bitmap, wxPoint(120, 40), wxDefaultSize);
	searchBar = new wxTextCtrl(panel2, wxID_ANY, "", wxPoint(157, 40), wxSize(350, 40));
	searchBar->SetFont(font);

	//load
	runTool = new wxButton(panel2, wxID_ANY, "run", wxPoint(60, 200), wxSize(40, 40));
	
	runTool->Bind(wxEVT_BUTTON, &MainFrame::OnLoadTool, this);

	//init height = 0
	suggestBar = new wxListBox(panel2, wxID_ANY, wxPoint(157, 85), wxSize(1200, 0));
	suggestBar->SetFont(font);

	//wordView = new wxListBox(panel2, wxID_ANY, wxPoint(520, 85), wxSize(250, 250));

	//sizer

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(panel, 1, wxEXPAND | wxLEFT | wxUP | wxRIGHT, 2);
	sizer->Add(panel2, 5, wxEXPAND | wxALL, 2);

	this->SetSizerAndFit(sizer);

	//events
	button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonDefToWord, this);
	

	CreateStatusBar();

}
MainFrame::~MainFrame() {

}


void MainFrame::OnButtonDefToWord(wxCommandEvent& evt) {
	wxString wstr = searchBar->GetValue();


	string word = string(wstr);

	if (word == "" || word == " ") {
		suggestBar->Clear();
		adjustSuggestBar(300, 14);
		return;
	}

	suggestBar->Clear();

	vector<Word> ans = dict.searchDefToWord2(word, 20);

	for (auto& w : ans) {
		string def = w.getStringDefinitions().back();
		string ww = w.getWord();
		suggestBar->Append(ww + "-" + def);
	}
	adjustSuggestBar(300, 14);
}


void MainFrame::OnViewWord(wxCommandEvent& evt) {


}

void MainFrame::OnMousePosition(wxMouseEvent& evt) {
	wxPoint m = evt.GetPosition();

	wxString message = wxString::Format("Mouse position within panel: (x=%d y=%d)", m.x, m.y);
	wxLogStatus(message);
}

void MainFrame::OnTextCtrlWordToDef(wxCommandEvent& evt) {
	wxString s = evt.GetString();
	
	string word = string(s);

	if (word.empty()) {
		wordView->Clear();
	}

	dict.chooseLanguage("DataSetEngEng");
	dict.runSearchEngine(word, true);

	suggestBar->Clear();

	vector<Word> listWord;

	listWord = dict.searchRelatedWords(word, 20);
	
	for (auto& w : listWord) {
		suggestBar->AppendString(w.getWord());
	}

	adjustSuggestBar(300, 14);

}



