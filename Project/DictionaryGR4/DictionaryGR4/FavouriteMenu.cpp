#include "FavouriteMenu.h"

using namespace std;
namespace fs = std::experimental::filesystem;

FavouriteMenu::~FavouriteMenu() {
}

FavouriteMenu::FavouriteMenu(wxWindow* parent, Dictionary*& dict)
    : wxSimplebook(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

	//font
	wxFont font(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wxFont fontTitle(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_NORMAL);
	wxFont fontCB(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	fontTitle.MakeBold();

	auto purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);

	SetBackgroundColour(black);

	//------FIRST PANE-------//

	mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);

	wxArrayString datasets = { "Eng-Eng", "Eng-Vie", "Vie-Eng" };
	datasetCbb = new wxComboBox(mainPanel, wxID_ANY, "Eng-Eng", wxPoint(290, 70), wxSize(154, -1), datasets, wxCB_READONLY);
	datasetCbb->SetFont(fontCB);
	datasetCbb->Refresh();

	wxStaticText* listCtrlText = new wxStaticText(mainPanel, wxID_ANY, "FAVOURITE WORDS", wxPoint(290, 30), wxSize(400, 20));
	auto fnt = listCtrlText->GetFont();
	fnt.SetPointSize(16);
	listCtrlText->SetFont(fnt);
	listCtrlText->SetBackgroundColour(black);
	listCtrlText->SetForegroundColour(white);

	listCtrl = new wxListCtrl(mainPanel, wxID_ANY, wxPoint(290, 130), wxSize(700, 530),
		wxLC_REPORT | wxBORDER_SUNKEN);

	listCtrl->SetFont(font);

	// Adding columns
	listCtrl->InsertColumn(0, "Word", wxLIST_FORMAT_LEFT, 250);
	listCtrl->InsertColumn(1, "Total number of definitions", wxLIST_FORMAT_CENTER, 440);


	//bindings

	datasetCbb->Bind(wxEVT_COMBOBOX, [this, dict](wxCommandEvent& evt) {
		dict->chooseLanguage(datasetCbb->GetStringSelection().ToStdString());
		OnLoadFavourite(evt, dict);
		});

	//---SECOND PANE---//

	secondPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);

	wxStaticText* viewingWord = new wxStaticText(secondPanel, wxID_ANY, "VIEWING FAVOURITE WORD", wxPoint(134, 40), wxSize(200, -1));
	viewingWord->SetFont(fnt);
	viewingWord->SetBackgroundColour(black);
	viewingWord->SetForegroundColour(white);

	wxStaticText* viewingDataSet = new wxStaticText(secondPanel, wxID_ANY, "ENG ENG", wxPoint(134, 90), wxSize(200, -1));
	viewingDataSet->SetFont(fnt);
	viewingDataSet->SetBackgroundColour(black);
	viewingDataSet->SetForegroundColour(white);

	wordView = new WordView(secondPanel, wxPoint(134, 133), wxSize(979, 460), dict);
	wordView->HideButtons();
	wordView->SetColor(white);


	//----------UNFAV BUTTON------------//
	wxFont boldfnt = fnt;
	boldfnt.SetWeight(wxFONTWEIGHT_BOLD);

	unfav = new wxButton(secondPanel, wxID_ANY, "UNFAVOURITE", wxPoint(741, 600), wxSize(164, 60), wxBORDER_NONE);
	unfav->SetFont(boldfnt);
	unfav->SetForegroundColour(white);
	unfav->SetBackgroundColour(red);

	//---------------------------------//

	back = new wxButton(secondPanel, wxID_ANY, "BACK", wxPoint(951, 600), wxSize(154, 60), wxBORDER_NONE);

	back->SetFont(fnt);
	back->SetForegroundColour(white);
	back->SetBackgroundColour(purple);

	//bindings

	back->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		this->SetSelection(0);
	});

	unfav->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		OnFavButtonClicked(evt, dict);
	});
	//---LIST CTRL BIND---//

	listCtrl->Bind(wxEVT_LIST_ITEM_SELECTED, [this, dict, viewingDataSet](wxListEvent& evt) {
		viewingDataSet->SetLabel(datasetCbb->GetStringSelection());
		OnViewWord(evt, dict);
	});

	//---SIMPLE BOOK---//

	this->AddPage(mainPanel, "FAVOURITE", true);
	this->AddPage(secondPanel, "VIEW A WORD", true);

	this->SetSelection(0);
}

void FavouriteMenu::OnLoadFavourite(wxCommandEvent& evt, Dictionary* dict) {
	dict->chooseLanguage(datasetCbb->GetStringSelection().ToStdString());
	listCtrl->DeleteAllItems();

	vector<string> words = dict->getFavWords();


	for (auto& w : words) {
		//find word w

		Word searchedWord = dict->searchWordMatching(w);

		if (searchedWord.empty()) continue;

		int cur = listCtrl->GetItemCount();
		int numDef = searchedWord.getNumberOfDefinitions();

		string str = searchedWord.getText();
		wxString wxstr = wxString::FromUTF8(str);

		wxString numstr = to_string(numDef);

		long itemIndex = listCtrl->InsertItem(cur, wxString::Format(wxstr, cur + 1));

		listCtrl->SetItem(itemIndex, 1, wxString::Format(numstr, cur + 1));
	}
}

void FavouriteMenu::OnViewWord(wxListEvent& evt, Dictionary* dict) {
	//move to second pane

	this->SetSelection(1);

	long selectedRow = evt.GetIndex();

	wxString wxtext = listCtrl->GetItemText(selectedRow);

	string str = string(wxtext.mb_str(wxConvUTF8));

	Word showing = dict->searchWordMatching(str);

	if (showing.empty()) {
		wordView->clearWordView();
	}
	else {
		wordView->processWord(showing, dict);
	}
}

void FavouriteMenu::OnFavButtonClicked(wxCommandEvent& evt, Dictionary* dict) {
	if (wordView->getShowingText() == "") {
		return;
	}

	wxMessageDialog* ask = new wxMessageDialog(this,
		"Are you sure to remove this word from favourite list?",
		"Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

	if (ask->ShowModal() == wxID_YES) {
		string word = wordView->getShowingText();
		wordView->clearWordView();

		dict->removeFavourite(word);

		this->SetSelection(0);
	}
}

