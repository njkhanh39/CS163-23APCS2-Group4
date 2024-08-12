#include "SearchMenu.h"


using namespace std;

SearchMenu::SearchMenu(wxWindow* parent) : wxWindow(parent, wxID_ANY) {

	//font
	wxFont font(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wxFont fontTitle(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_NORMAL);
	fontTitle.MakeBold();

	titleBar = new wxPanel(this, 10001, wxDefaultPosition, wxSize(1280, 120));
	titleBar->SetBackgroundColour(wxColor(100, 100, 200));
	mainPanel = new wxPanel(this, 10002, wxDefaultPosition, wxSize(1280, 600));


	wxBitmap bitmap(wxT("image.png"), wxBITMAP_TYPE_PNG);

	wxStaticText* title = new wxStaticText(titleBar, wxID_ANY, "Dictionary", wxPoint(30, 30));
	title->SetFont(fontTitle);

	// Create a wxBitmapButton
	button = new wxBitmapButton(mainPanel, wxID_ANY, bitmap, wxPoint(320, 40), wxDefaultSize);
	searchBar = new wxTextCtrl(mainPanel, wxID_ANY, "", wxPoint(357, 40), wxSize(450, 40));
	searchBar->SetFont(font);

	// Create combo box to choose dataset
	wxArrayString languages = { "Eng-Eng", "Eng-Vie", "Vie-Eng" };
	datasetCbb = new wxComboBox(mainPanel, wxID_ANY, "", wxPoint(200,40), wxSize(120, 40), languages, wxCB_DROPDOWN | wxCB_READONLY);
	datasetCbb->SetFont(font);
	datasetCbb->Select(0);

	//load
	searchByDef = new wxButton(mainPanel, wxID_ANY, "Def->Word", wxPoint(60, 200), wxSize(120, 40));
	searchByDef->Bind(wxEVT_BUTTON, &SearchMenu::OnLoadTool, this);
	searchByDef->SetFont(font);

	//unload
	searchByWord = new wxButton(mainPanel, wxID_ANY, "Word->Def", wxPoint(60, 240), wxSize(120, 40));
	searchByWord->Bind(wxEVT_BUTTON, &SearchMenu::OnUnLoadTool, this);
	searchByWord->SetFont(font);

	//word view appears first
	wordView = new WordView(mainPanel, wxPoint(320, 200), wxSize(750, 350));

	//init height = 0
	suggestBar = new wxListBox(mainPanel, wxID_ANY, wxPoint(357, 83), wxSize(450, 0));
	suggestBar->SetFont(font);



	//back button

	back_to_home = new wxButton(mainPanel, wxID_ANY, "Home", wxPoint(50, 100), wxSize(100, 50));
	back_to_home->SetFont(font);


	//sizer

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(titleBar, 1, wxEXPAND | wxLEFT | wxUP | wxRIGHT, 2);
	sizer->Add(mainPanel, 5, wxEXPAND | wxALL, 2);

	this->SetSizerAndFit(sizer);

	//events
	
	
	suggestBar->Bind(wxEVT_LEFT_DOWN, &SearchMenu::skip, this);
	button->Bind(wxEVT_BUTTON, &SearchMenu::OnSearchButton, this);
	searchBar->Bind(wxEVT_TEXT, &SearchMenu::OnSearchAndSuggestHandler, this);
	suggestBar->Bind(wxEVT_LISTBOX, &SearchMenu::OnViewWord, this);

}
SearchMenu::~SearchMenu() {

}

void SearchMenu::skip(wxMouseEvent& evt) {
	evt.Skip();
}


void SearchMenu::OnSearchButton(wxCommandEvent& evt) {
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
		wxString keyuni = searchBar->GetValue();

		string key = string(keyuni.mb_str(wxConvUTF8));

		//clear the suggest bar

		suggestBar->Clear();

		adjustSuggestBar(300, 14);

		//do not clear search bar, since it will terminate our data

		Word word = dict.searchWordMatching(key);

		wordView->processWord(word);
	}
}

void SearchMenu::OnViewWord(wxCommandEvent& evt) {
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

		//we need to search for that word & all its definitions
		Word word = dict.searchWordMatching(key);

		wordView->processWord(word);

		//avoiding overlapping panels
		//wordView->Enable();

		SearchedWord SW(key);
		SW.setTime();
		SW.setDate();
		hist.saveToFile(SW);



		return;
	}

	//word->def

	int selected = suggestBar->GetSelection();

	wxString keyuni = suggestBar->GetString(selected);

	string key = string(keyuni.mb_str(wxConvUTF8));

	//process the key
	string word, def;

	int i = 0;
	while (key[i] != '\t') {
		word.push_back(key[i]);
		++i;
	}
	++i;
	def = key.substr(i, (int)key.length());

	//we want other definitions as well, not just "def" alone.
	Word mainWord = dict.searchWordMatching(word);

	//clear the suggest bar

	suggestBar->Clear();

	adjustSuggestBar(300, 14);


	//process the word
	wordView->processWord(mainWord);
	wordView->goToDefinition(def);

	//avoiding overlapping panels
	//wordView->Enable();
}

void SearchMenu::OnSearchAndSuggestHandler(wxCommandEvent& evt) {
	//avoiding overlapping panels
	//wordView->Disable();
	//only for #case2, search by word
	if (!dict.isSearchingDefinition) {
		wxString s = evt.GetString();

		string word = string(s);

		dict.chooseLanguage(datasetCbb->GetStringSelection().ToStdString());

		dict.runSearchEngine(word, false);

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

void SearchMenu::adjustSuggestBar(int maxHeight, int maxItem) {
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


void SearchMenu::OnLoadTool(wxCommandEvent& evt) {
	dict.chooseLanguage(datasetCbb->GetStringSelection().ToStdString());
	if (!dict.isSearchingDefinition) dict.runSearchDefinitionEngine();
}

void SearchMenu::OnUnLoadTool(wxCommandEvent& evt) {
	dict.turnOffSearchDefinitionEngine();
}

void SearchMenu::OnResetButtonClicked() {
	string curDataset = datasetCbb->GetStringSelection().ToStdString();

	wxMessageDialog* ask = new wxMessageDialog(this,
		"Are you sure to reset the dataset \"" + curDataset + "\"?",
		"Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

	if (ask->ShowModal() == wxID_YES) {

	}
}

