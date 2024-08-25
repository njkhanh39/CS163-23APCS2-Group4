#include "SearchMenu.h"

using namespace std;
namespace fs = std::experimental::filesystem;

SearchMenu::SearchMenu(wxWindow* parent, Dictionary*& dict) : wxPanel(parent, 10001, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {
	

	auto purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);

	//font
	wxFont font(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wxFont fontTitle(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_NORMAL);
	wxFont fontCB(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	fontTitle.MakeBold();

	SetBackgroundColour(black);
	 
	//mainPanel->SetBackgroundColour((0xff, 0xcc, 0xcc));

	wxBitmap bitmap(wxT("IMG/image.png"), wxBITMAP_TYPE_PNG);

	// Create a wxBitmapButton
	button = new wxBitmapButton(this, wxID_ANY, bitmap, wxPoint(1010, 41), wxSize(60,60));
	//button->SetBackgroundColour(purple);
	searchBar = new wxTextCtrl(this, wxID_ANY, "", wxPoint(234, 41), wxSize(776, 60));
	searchBar->SetFont(fontCB);

	rd_button = new wxButton(this, wxID_ANY, "RANDOM", wxPoint(1097, 41), wxSize(116, 60), wxBORDER_NONE);
	auto fnt = rd_button->GetFont();
	fnt.SetPointSize(16);

	rd_button->SetFont(fnt);
	rd_button->SetBackgroundColour(purple);
	rd_button->SetForegroundColour(white);


	wxBitmap bitmaphexa(wxT("IMG/Hexagon.png"), wxBITMAP_TYPE_PNG);

	searchByDef = new wxBitmapButton(this, wxID_ANY, bitmaphexa, wxPoint(40,123), wxSize(181,181), wxBORDER_NONE);

	searchByWord = new wxBitmapButton(this, wxID_ANY, bitmaphexa, wxPoint(40, 304), wxSize(181, 181), wxBORDER_NONE);

	//def2word->SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
	//word2def->SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);

	searchByDef->SetBackgroundColour(black);
	searchByWord->SetBackgroundColour(black);

	searchByDef->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		OnSearchingByDef(evt, dict);
	});
	searchByWord->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		OnSearchingByWord(evt, dict);
	});



	// Create combo box to choose dataset
	
	wxArrayString languages = { "Eng-Eng", "Eng-Vie", "Vie-Eng" };
	datasetCbb = new wxComboBox(this, wxID_ANY, "", wxPoint(53, 41), wxSize(154, -1), languages, wxCB_READONLY);

	datasetCbb->SetFont(fontCB);
	datasetCbb->SetSize(154, 60);

	datasetCbb->SetBackgroundColour(green);
	datasetCbb->SetForegroundColour(white);

	datasetCbb->SetFont(fontCB);
	datasetCbb->SetSize(154, 60);
	datasetCbb->SetSelection(0);
	datasetCbb->Refresh();

	datasetCbb->Bind(wxEVT_COMBOBOX, [this, dict](wxCommandEvent& evt) {
		dict->chooseLanguage(string(datasetCbb->GetStringSelection().mb_str(wxConvUTF8)));
	});

	//load
	/*	searchByDef = new wxButton(mainPanel, wxID_ANY, "Def->Word", wxPoint(60, 200), wxSize(120, 40));
	searchByDef->Bind(wxEVT_BUTTON, &SearchMenu::OnLoadTool, this);
	searchByDef->SetFont(font);

	//unload
	searchByWord = new wxButton(mainPanel, wxID_ANY, "Word->Def", wxPoint(60, 240), wxSize(120, 40));
	searchByWord->Bind(wxEVT_BUTTON, &SearchMenu::OnUnLoadTool, this);
	searchByWord->SetFont(font);*/


	//word view appears first
	wordView = new WordView(this, wxPoint(234, 133), wxSize(979, 460), dict);
	wordView->SetColor(white);

	//init height = 0
	suggestBar = new wxListBox(this, wxID_ANY, wxPoint(357, 100), wxSize(450, 0));
	suggestBar->SetFont(font);

	deleteword = new wxButton(this, wxID_ANY, "DELETE WORD", wxPoint(1042, 610), wxSize(172, 60));
	deleteword->SetFont(fnt);
	deleteword->SetBackgroundColour(red);
	deleteword->SetForegroundColour(white);

	resetbutton = new wxButton(this, wxID_ANY, "RESET", wxPoint(53, 495), wxSize(154, 98)/*, wxBORDER_NONE*/);
	resetbutton->SetFont(fnt);
	resetbutton->SetBackgroundColour(red);
	resetbutton->SetForegroundColour(white);


	//events
	
	deleteword->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		Word get = wordView->getShowingWord();
		if (get.empty()) return;
		dict->deleteWord(get);
		wordView->SetBackDefault();
	});
	
	suggestBar->Bind(wxEVT_LEFT_DOWN, &SearchMenu::skip, this);
	button->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		OnSearchButton(evt, dict);
	});
	searchBar->Bind(wxEVT_TEXT, [this, dict](wxCommandEvent& evt) {
		OnSearchAndSuggestHandler(evt, dict);
	});
	//searchBar->Bind(wxEVT_BUTTON, bind(&SearchMenu::OnSearchAndSuggestHandler, this, placeholders::_1, ref(dict)));
	suggestBar->Bind(wxEVT_LISTBOX, [this, dict](wxCommandEvent& evt) {
		OnViewWord(evt, dict);
	});
	rd_button->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		OnRandomClicked(evt, dict);
	});

	resetbutton->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		OnResetButtonClicked(evt, dict);
	});

	wxProgressDialog progressDialog("Please Wait", "Performing a long task...", 100, this,
		wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_SMOOTH);

	dict->runSearchDefinitionEngine();

	wxLogMessage("Task completed!");
}

SearchMenu::~SearchMenu() {

}

void SearchMenu::skip(wxMouseEvent& evt) {
	evt.Skip();
}

void SearchMenu::OnSearchButton(wxCommandEvent& evt, Dictionary* dict) {
	//#case 1: search by definition, press the button will list out
	if (dict->isSearchingDefinition) {
		wxString wunicode = searchBar->GetValue();

		string word = string(wunicode.mb_str(wxConvUTF8));

		if (word == "" || word == " ") {
			suggestBar->Clear();
			adjustSuggestBar(300, 14);
			return;
		}

		suggestBar->Clear();

		//recommend 20 words with defs
		vector<Word> ans = dict->searchDefToWord(word, 20);

		for (auto& w : ans) {
			string def = w.getStringDefinitions().back();
			string ww = w.getText();
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

		Word word = dict->searchWordMatching(key);

		wordView->processWord(word);

		Word* ptr = dict->getWordPtr(key);
		wordView->setWord(ptr);
		wordView->setActiveDataset(dict->getActiveDataset());
	}
}

void SearchMenu::OnViewWord(wxCommandEvent& evt, Dictionary* dict) {
	if (!dict->isSearchingDefinition) { //word->def
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
		Word word = dict->searchWordMatching(key);

		wordView->processWord(word);

		Word* ptr = dict->getWordPtr(key);
		wordView->setWord(ptr);
		wordView->setActiveDataset(dict->getActiveDataset());

		//avoiding overlapping panels
		//wordView->Enable();

		SearchedWord SW(key);
		SW.setTime();
		SW.setDate();
		dict->getHistory().saveToFile(SW);



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
	Word mainWord = dict->searchWordMatching(word);

	//clear the suggest bar

	suggestBar->Clear();

	adjustSuggestBar(300, 14);


	//process the word
	wordView->processWord(mainWord);
	wordView->goToDefinition(def);

	//avoiding overlapping panels
	//wordView->Enable();
	SearchedWord SW(word);



	SW.setTime();
	SW.setDate();
	dict->getHistory().saveDevToWord(SW, def);
}

void SearchMenu::OnSearchAndSuggestHandler(wxCommandEvent& evt, Dictionary* dict) {
	//avoiding overlapping panels
	//wordView->Disable();
	//only for #case2, search by word
	if (!(dict->isSearchingDefinition)) {
		wxString s = evt.GetString();

		string word = string(s.mb_str(wxConvUTF8));

		dict->chooseLanguage(string(datasetCbb->GetStringSelection().mb_str(wxConvUTF8)));

		dict->runSearchEngine(word, false);

		suggestBar->Clear();

		vector<Word> listWord;

		//search for 20 related words
		listWord = dict->searchRelatedWords(word, 20);

		for (auto& w : listWord) {
			suggestBar->AppendString(wxString::FromUTF8(w.getText()));
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


void SearchMenu::OnSearchingByDef(wxCommandEvent& evt, Dictionary* dict) {
	dict->isSearchingDefinition = true;
}

void SearchMenu::OnSearchingByWord(wxCommandEvent& evt, Dictionary* dict) {
	dict->isSearchingDefinition = false;
}

void SearchMenu::OnResetButtonClicked(wxCommandEvent& evt, Dictionary* dict) {
	string curDataset = dict->getActiveDataset();

	wxMessageDialog* ask = new wxMessageDialog(this,
		"Are you sure to reset the dataset \"" + curDataset + "\"?",
		"Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

	if (ask->ShowModal() == wxID_YES) {
		fs::copy("DataSet - Backup\\" + curDataset, "DataSet\\" + curDataset, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
	}

	dict->reloadWordFinder(curDataset);
}

void SearchMenu::OnRandomClicked(wxCommandEvent& evt, Dictionary* dict) {
	string wordText;
	Word word = dict->getRandomWord(wordText);

	wordView->processWord(word);

	Word* ptr = dict->getWordPtr(word.getText());
	wordView->setWord(ptr);
	wordView->setActiveDataset(dict->getActiveDataset());
}
