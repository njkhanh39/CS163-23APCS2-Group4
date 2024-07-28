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
	wxFont font(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	//controls
	
	panel = new wxPanel(this, 10001, wxDefaultPosition, wxSize(800, 300));
	panel->SetBackgroundColour(wxColor(100, 100, 200));

	button = new wxButton(panel, 10001, "Enter", wxPoint(40, 150), wxSize(70, 40));
	listBox = new wxListBox(panel, wxID_ANY, wxPoint(120, 200), wxSize(220, 300));	
	historyButton = new wxButton(panel, wxID_ANY, "History", wxPoint(40, 200), wxSize(70,40));
	wordView = new wxListBox(panel, wxID_ANY, wxPoint(350, 150), wxSize(220, 40));
	defView = new wxListBox(panel, wxID_ANY, wxPoint(350, 200), wxSize(380, 300));

	textCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(120, 150), wxSize(220,40));



	// Set font
	listBox->SetFont(font);
	button->SetFont(font);
	historyButton->SetFont(font);
	textCtrl->SetFont(font);
	wordView->SetFont(font);
	

	//event handling

	textCtrl->Bind(wxEVT_TEXT, &MainFrame::OnTextWritten, this);

	listBox->Bind(wxEVT_LISTBOX, &MainFrame::OnViewWord, this);
	
	historyButton->Bind(wxEVT_BUTTON, &MainFrame::OnHistoryButtonClicked, this);

	CreateStatusBar();
}
MainFrame::~MainFrame() {

}

void MainFrame::OnViewWord(wxCommandEvent& evt) {
	wordView->Clear();
	defView->Clear();
	int i = listBox->GetSelection();
	string temp = string(listBox->GetString(i));

	wordView->AppendString(temp);
	list<string> defs = dict.searchStringDefinitions(temp);
	for (auto& st : defs) defView->AppendString(st);

	SearchedWord currentWord(temp);
	currentWord.setDate();
	currentWord.setTime();

	// save history to text file

	his.saveToFile(currentWord);


}

void MainFrame::OnMousePosition(wxMouseEvent& evt) {
	wxPoint m = evt.GetPosition();

	wxString message = wxString::Format("Mouse position within panel: (x=%d y=%d)", m.x, m.y);
	wxLogStatus(message);
}

void MainFrame::OnTextWritten(wxCommandEvent& evt) {
	wxString s = evt.GetString();
	
	string word = string(s);

	if (word.empty()) {
		wordView->Clear();
		defView->Clear();
	}

	dict.chooseLanguage("DataSetEngEng");
	dict.runSearchEngine(word, true);

	listBox->Clear();

	list<Word> listWord;

	listWord = dict.relatedWord(word, 10);
	
	for (auto& w : listWord) listBox->AppendString(w.getWord());
	
}

void MainFrame::ShowHistory() {
	textCtrl->Hide();
	button->Hide();
	listBox->Hide();
	wordView->Hide();
	defView->Hide();
	historyButton->Hide();

	HistoryList = new wxListBox(panel, wxID_ANY, wxPoint(120, 200), wxSize(550, 300));
	wxFont font(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	HistoryList->SetFont(font);

	viewWordInHistory = new wxButton(panel, wxID_ANY,"View", wxPoint(40, 150), wxSize(70, 40));
	viewWordInHistory->SetFont(font);
}


void MainFrame::OnHistoryButtonClicked(wxCommandEvent& evt) {
	ShowHistory();
	his.loadFromFile(); // history loaded into searchList
	list<SearchedWord> searchList = his.getsearchList();
	for (auto& sw : searchList) {
		HistoryList->AppendString("[" + sw.getWord() + "][" + sw.getDate() + "][" + sw.getTime() + "]");
	}
	viewWordInHistory->Bind(wxEVT_BUTTON, &MainFrame::OnViewWordInHistory, this);
}

void MainFrame::OnViewWordInHistory(wxCommandEvent& evt) {
	int index = HistoryList->GetSelection();
	if (index == wxNOT_FOUND) {
		wxLogError("No item selected!");
		return;
	}

	string temp = string(HistoryList->GetString(index));
	size_t firstBracket = temp.find('[');
	size_t secondBracket = temp.find(']', firstBracket);



	if (firstBracket == string::npos || secondBracket == string::npos || firstBracket >= secondBracket) {
		wxLogError("Invalid format in selected item!");
		return;
	}

	temp = temp.substr(firstBracket + 1, secondBracket - firstBracket - 1);

	HistoryList->Hide();
	viewWordInHistory->Hide();

	wordView->Show();
	defView->Show();
	wordView->Clear();
	defView->Clear();

	wordView->AppendString(temp);
	list<string> defs = dict.searchStringDefinitions(temp);

	//if (defs.empty()) {
		//wxLogError("No definitions found for the word: %s", temp);
	//}
	//else {
		for (auto& st : defs) {
			defView->AppendString(st);
		}
	//}
}

