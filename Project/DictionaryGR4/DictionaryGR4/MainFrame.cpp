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

	panel = new wxPanel(this, 10001, wxDefaultPosition, wxSize(1280, 120));
	panel->SetBackgroundColour(wxColor(100, 100, 200));
	panel2 = new wxPanel(this, 10002, wxDefaultPosition, wxSize(1280, 600));

	
	wxBitmap bitmap(wxT("image.png"), wxBITMAP_TYPE_PNG);

	wxStaticText* titleBar = new wxStaticText(panel, wxID_ANY, "Dictionary", wxPoint(30, 30));
	titleBar->SetFont(fontTitle);

	// Create a wxBitmapButton
	button = new wxBitmapButton(panel2 , wxID_ANY, bitmap, wxPoint(320, 40), wxDefaultSize);
	searchBar = new wxTextCtrl(panel2, wxID_ANY, "", wxPoint(357, 40), wxSize(450, 40));
	searchBar->SetFont(font);

	//load
	runTool = new wxButton(panel2, wxID_ANY, "Def->Word", wxPoint(60, 200), wxSize(120, 40));
	runTool->Bind(wxEVT_BUTTON, &MainFrame::OnLoadTool, this);
	runTool->SetFont(font);

	//unload
	unrunTool = new wxButton(panel2, wxID_ANY, "Word->Def", wxPoint(60, 240), wxSize(120, 40));
	unrunTool->Bind(wxEVT_BUTTON, &MainFrame::OnUnLoadTool, this);
	unrunTool->SetFont(font);

	//word view appears first
	wordView = new WordView(panel2, wxPoint(320, 200), wxSize(750, 350));
	//init height = 0
	suggestBar = new wxListBox(panel2, wxID_ANY, wxPoint(357, 83), wxSize(450, 0));
	suggestBar->SetFont(font);

	

	//sizer

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(panel, 1, wxEXPAND | wxLEFT | wxUP | wxRIGHT, 2);
	sizer->Add(panel2, 5, wxEXPAND | wxALL, 2);

	this->SetSizerAndFit(sizer);

	//events
	//button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonDefToWord, this);


	button->Bind(wxEVT_BUTTON, &MainFrame::OnSearchButton, this);
	searchBar->Bind(wxEVT_TEXT, &MainFrame::OnSearchAndSuggestHandler, this);
	suggestBar->Bind(wxEVT_LISTBOX, &MainFrame::OnViewWord, this);
	

	CreateStatusBar();

}
MainFrame::~MainFrame() {

}


void MainFrame::OnSearchButton(wxCommandEvent& evt) {
	//#case 1: search by definition, press the button will list out
	if (dict.isSearchingDefinition) {
		wxString wunicode = searchBar->GetValue();

		string word = string(wunicode.mb_str(wxConvUTF8));

		if (word == "" || word == " ") {
			suggestBar->Clear();
			adjustSuggestBar(300, 14);
			return;
		}

		suggestBar->Clear();


		//recommend 20 words with defs
		vector<Word> ans = dict.searchDefToWord(word, 20);

		for (auto& w : ans) {
			string def = w.getStringDefinitions().back();
			string ww = w.getWord();
			suggestBar->Append(wxString::FromUTF8(ww + "\t" + def));
		}
		adjustSuggestBar(300, 14);

		return;
	}

	//#case2: Search by word

	//if suggest bar is empty, or suggested words doesnt match search bar's keyword
	if (suggestBar->GetCount() == 0 || ((int)searchBar->GetValue().length() != (int)suggestBar->GetString(0).length())) {


	}
	else {
		//add the word to our search bar
		string key = (string)searchBar->GetValue();


		//clear the suggest bar

		suggestBar->Clear();
		adjustSuggestBar(300, 14);

		//do not clear search bar, since it will terminate our data

		Word word = dict.searchWordMatching(key);

		wordView->processWord(word);
	}

}

void MainFrame::OnViewWord(wxCommandEvent& evt) {
	if (!dict.isSearchingDefinition) { //word->def
		int selected = suggestBar->GetSelection();
		wxString keyuni = suggestBar->GetString(selected);
		string key = (string)keyuni.mb_str(wxConvUTF8);

		//add the word to our search bar

		searchBar->SetValue(keyuni);

		//clear the suggest bar

		suggestBar->Clear();
		adjustSuggestBar(300, 14);

		//do not clear search bar, since it will terminate our data

		Word word = dict.searchWordMatching(key);

		wordView->processWord(word);

		//avoiding overlapping panels
		//wordView->Enable();
		return;
	}

	//word->def

	int selected = suggestBar->GetSelection();
	wxString keyuni = suggestBar->GetString(selected);
	string key = (string)keyuni.mb_str(wxConvUTF8);

	//wordView->Enable();
	if (key.empty()) return;

	//extract word

	Word mainWord;
	string text,def; int i = 0;
	while (key[i] != '\t') {
		if (key[i] == '\t') break;
		text.push_back(key[i]);
		++i;
	}
	++i;

	def = key.substr(i, (int)key.length());

	//clear the suggest bar

	suggestBar->Clear();
	adjustSuggestBar(300, 14);

	//look for the specific # of the definition wrt to that "text" word

	//quickly search for one
	mainWord = dict.searchWordMatching(text);

	//process the word
	wordView->processWord(mainWord);
	wordView->goToDefinition(def);

	//avoiding overlapping panels
	//wordView->Enable();
}

void MainFrame::OnSearchAndSuggestHandler(wxCommandEvent& evt) {
	//avoiding overlapping panels
	//wordView->Disable();
	//only for #case2, search by word
	if (!dict.isSearchingDefinition) {
		wxString s = evt.GetString();

		string word = string(s);


		dict.chooseLanguage("Eng-Vie");
		dict.runSearchEngine(word, true);

		suggestBar->Clear();

		vector<Word> listWord;

		//search for 20 related words
		listWord = dict.searchRelatedWords(word, 20);

		for (auto& w : listWord) {
			suggestBar->AppendString(wxString::FromUTF8(w.getWord()));
		}

		adjustSuggestBar(300, 14);
	}
	
	//if (suggestBar->IsEmpty()) wordView->Enable();
}

void MainFrame::adjustSuggestBar(int maxHeight, int maxItem) {
	// Calculate the new size for the list box based on the number of items

	int itemCount = suggestBar->GetCount();        // Get the number of items

	int newHeight;

	if (itemCount == 0) newHeight = 0;
	else newHeight = maxHeight * itemCount / maxItem;

	if (newHeight > maxHeight) newHeight = maxHeight;

	// Set the new size for the list box
	wxSize currentSize = suggestBar->GetSize();
	suggestBar->SetSize(wxSize(currentSize.GetWidth(), newHeight));

	// Update the layout to reflect the new size
	Layout();
}

void MainFrame::OnMousePosition(wxMouseEvent& evt) {
	wxPoint m = evt.GetPosition();

	wxString message = wxString::Format("Mouse position within panel: (x=%d y=%d)", m.x, m.y);
	wxLogStatus(message);
}

void MainFrame::OnLoadTool(wxCommandEvent& evt) {
	dict.chooseLanguage("Eng-Vie");
	if (!dict.isSearchingDefinition) dict.runSearchDefinitionEngine();
}

void MainFrame::OnUnLoadTool(wxCommandEvent& evt) {
	dict.turnOffSearchDefinitionEngine();
}

