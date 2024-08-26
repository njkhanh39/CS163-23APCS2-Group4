#include "HistoryMenu.h"

using namespace std;
namespace fs = std::experimental::filesystem;


HistoryMenu::~HistoryMenu() {

}

HistoryMenu::HistoryMenu(wxWindow* parent, Dictionary*& dict): wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {
	auto purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);

	//font
	wxFont font(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wxFont fontTitle(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_NORMAL);
	wxFont fontCB(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	fontTitle.MakeBold();

	SetBackgroundColour(black);

	//mainPanel->SetBackgroundColour((0xff, 0xcc, 0xcc));

	searchBar = new wxTextCtrl(this, wxID_ANY, "", wxPoint(234, 41), wxSize(776, 60));
	searchBar->SetFont(fontCB);

	wordView = new WordView(this, wxPoint(234, 133), wxSize(979, 460), dict);
	wordView->SetColor(white);
	wordView->getEditDefButton()->Hide();
	wordView->getDelDefButton()->Hide();
	wordView->getFavDefButton()->Hide();


	back = new wxButton(this, wxID_ANY, "Back", wxPoint(190, 41), wxSize(40, 20));
	back->SetFont(fontCB);
	

	view = new wxButton(this, wxID_ANY, "View", wxPoint(1050, 50), wxSize(40, 20));
	view->SetFont(fontCB);

	wxArrayString items;
	dict->getHistory().loadFromFile();
	for (auto c : dict->getHistory().getsearchList()) {
		string line = c.getDate() + "	" + c.getTime() + "			" + c.getText();
		items.Add(line);
	}
	list = new wxListBox(this, wxID_ANY, wxPoint(234, 50), wxSize(776, 500), items);
	
	view->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		OnViewButton(evt, dict);
		});

	back->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {OnBackButton(evt, dict); 
		});

	back->Hide();
	searchBar->Hide();
	wordView->Hide();
}


void HistoryMenu::OnViewButton(wxCommandEvent& evt, Dictionary* dict) {
	int selected = list->GetSelection();
	wxString keyuni = list->GetString(selected);
	string key = (string)keyuni.mb_str(wxConvUTF8);

	view->Hide();
	list->Hide();

	back->Show();
	searchBar->Show();
	wordView->Show();

	searchBar->SetValue(keyuni);
	Word word = dict->searchWordMatching(key);
	wordView->processWord(word);
	Word* ptr = dict->getWordPtr(key);
	wordView->setWord(ptr);
	wordView->setActiveDataset(dict->getActiveDataset());
}

void HistoryMenu::OnBackButton(wxCommandEvent& evt, Dictionary* dict) {
	back->Hide();
	searchBar->Hide();
	wordView->Hide();
	view->Show();
	list->Show();
}