#include "GUI_AddWordMenu.h"

using namespace std;

AddWordMenu::AddWordMenu(wxWindow* parent, Dictionary*& dict) : wxSimplebook(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

	//font
	wxFont font(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wxFont fontTitle(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_NORMAL);
	wxFont fontCB(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	fontTitle.MakeBold();

	auto purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);

	SetBackgroundColour(black);


	//----First Panel----//


	mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);

	wordText = new wxTextCtrl(mainPanel, wxID_ANY, "", wxPoint(114, 101), wxSize(635, 60));
	wordText->SetFont(font);


	wxArrayString datasets = { "Eng-Eng", "Eng-Vie", "Vie-Eng" };
	datasetCbb = new wxComboBox(mainPanel, wxID_ANY, "Eng-Eng", wxPoint(773, 101), wxSize(154, -1), datasets, wxCB_READONLY);
	datasetCbb->SetFont(fontCB);
	datasetCbb->SetSize(154, 60);
	datasetCbb->Refresh();


	defText = new wxTextCtrl(mainPanel, wxID_ANY, "", wxPoint(110, 240), wxSize(1000, 300), wxTE_MULTILINE);
	defText->SetFont(font);

	wordTypeText = new wxTextCtrl(mainPanel, wxID_ANY, "", wxPoint(951, 101), wxSize(154, 60));
	wordTypeText->SetFont(fontCB);

	submit = new wxButton(mainPanel, wxID_ANY, "ADD", wxPoint(951, 600), wxSize(154, 60), wxBORDER_NONE);
	auto fntSubmit = submit->GetFont();
	fntSubmit.SetPointSize(16);
	submit->SetFont(fntSubmit);
	submit->SetForegroundColour(white);
	submit->SetBackgroundColour(green);


	deletedWords = new wxButton(mainPanel, wxID_ANY, "VIEW DELETED WORDS", wxPoint(651, 600), wxSize(254, 60), wxBORDER_NONE);
	deletedWords->SetFont(fntSubmit);
	deletedWords->SetForegroundColour(white);
	deletedWords->SetBackgroundColour(purple);


	wxStaticText* wordBarText = new wxStaticText(mainPanel, wxID_ANY, "WORD", wxPoint(114, 60), wxSize(100, 20));
	auto fnt = wordBarText->GetFont();
	fnt.SetPointSize(16);
	wordBarText->SetFont(fnt);
	wordBarText->SetBackgroundColour(black);
	wordBarText->SetForegroundColour(white);

	wxStaticText* dataSetBarText = new wxStaticText(mainPanel, wxID_ANY, "DATASET", wxPoint(773, 60), wxSize(100, 20));
	dataSetBarText->SetFont(fnt);
	dataSetBarText->SetBackgroundColour(black);
	dataSetBarText->SetForegroundColour(white);

	wxStaticText* defBarText = new wxStaticText(mainPanel, wxID_ANY, "DEFINITION", wxPoint(110, 189), wxSize(100, 20));
	defBarText->SetFont(fnt);
	defBarText->SetBackgroundColour(black);
	defBarText->SetForegroundColour(white);

	wxStaticText* wordTypeBarText = new wxStaticText(mainPanel, wxID_ANY, "WORDTYPE", wxPoint(951, 60), wxSize(100, 20));
	wordTypeBarText->SetFont(fnt);
	wordTypeBarText->SetBackgroundColour(black);
	wordTypeBarText->SetForegroundColour(white);



	datasetCbb->Bind(wxEVT_COMBOBOX, [this, dict](wxCommandEvent& evt) {
		dict->chooseLanguage(datasetCbb->GetStringSelection().ToStdString());
		});

	submit->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		wxString typeunicode = wordTypeText->GetValue();
		string type = string(typeunicode.mb_str(wxConvUTF8));

		wxString wunicode = wordText->GetValue();
		string text = string(wunicode.mb_str(wxConvUTF8));

		wxString defunicode = defText->GetValue();
		string def = string(defunicode.mb_str(wxConvUTF8));

		//normalize
		for (auto& c : text) c = tolower(c);

		while (!def.empty() && def.back() == '\n') def.pop_back();

		if (!type.empty()) {
			if (type.back() != ')') {
				type.push_back(')');
			}

			if (type[0] != '(') {
				reverse(type.begin(), type.end());
				type.push_back(')');
				reverse(type.begin(), type.end());
			}

			def = type + " " + def;
		}

		wxMessageDialog* ask = new wxMessageDialog(this,
			"Are you sure to add word \"" + wunicode + "\"?",
			"Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

		if (ask->ShowModal() == wxID_YES) dict->addNewWordOneDef(text, def);
		});

	//----Second Panel----//

	secondPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);

	backToMain = new wxButton(secondPanel, wxID_ANY, "BACK", wxPoint(951, 600), wxSize(154, 60));

	backToMain->SetFont(fntSubmit);
	backToMain->SetForegroundColour(white);
	backToMain->SetBackgroundColour(purple);



	datasetCbb2 = new wxComboBox(secondPanel, wxID_ANY, "Eng-Eng", wxPoint(696, 30), wxSize(154, -1), datasets, wxCB_READONLY);
	datasetCbb2->SetFont(fontCB);
	datasetCbb2->SetSize(154, 60);
	datasetCbb2->Refresh();

	wxStaticText* listCtrlText = new wxStaticText(secondPanel, wxID_ANY, "DELETED WORDS/DEFINITION", wxPoint(250, 59), wxSize(400, 20));
	listCtrlText->SetFont(fnt);
	listCtrlText->SetBackgroundColour(black);
	listCtrlText->SetForegroundColour(white);



	listCtrl = new wxListCtrl(secondPanel, wxID_ANY, wxPoint(250, 100), wxSize(700, 500),
		wxLC_REPORT | wxBORDER_SUNKEN);

	listCtrl->SetFont(font);

	// Adding columns
	listCtrl->InsertColumn(0, "Word", wxLIST_FORMAT_LEFT, 250);
	listCtrl->InsertColumn(1, "Deleted definitions / Total", wxLIST_FORMAT_LEFT, 450);


	datasetCbb2->Bind(wxEVT_COMBOBOX, [this, dict](wxCommandEvent& evt) {
		dict->chooseLanguage(datasetCbb2->GetStringSelection().ToStdString());
		listCtrl->DeleteAllItems();

		vector<Word> show = dict->getDeletedWords();

		for (auto& w : show) {
			string str = w.getText();


			wxString wxstr = wxString::FromUTF8(str);

			int num = w.getNumberOfDefinitions();


			int size = dict->searchWordMatchingForcedWordFinder(str).getNumberOfDefinitions() + num;

			wxString numstr = to_string(num) + "/" + to_string(size);
			int cur = listCtrl->GetItemCount();

			long itemIndex = listCtrl->InsertItem(cur, wxString::Format(wxstr, cur + 1));

			listCtrl->SetItem(itemIndex, 1, wxString::Format(numstr, cur + 1));
		}
		});

	//listctrl bind at the end

	//----------------Third panel-------------------//

	thirdPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);


	backToSecond = new wxButton(thirdPanel, wxID_ANY, "BACK", wxPoint(951, 600), wxSize(154, 60));

	backToSecond->SetFont(fntSubmit);
	backToSecond->SetForegroundColour(white);
	backToSecond->SetBackgroundColour(purple);

	recover = new wxButton(thirdPanel, wxID_ANY, "RECOVER", wxPoint(795, 600), wxSize(154, 60));

	recover->SetFont(fntSubmit);
	recover->SetForegroundColour(white);
	recover->SetBackgroundColour(red);

	wxStaticText* viewingWord = new wxStaticText(thirdPanel, wxID_ANY, "VIEWING DELETED WORD", wxPoint(134, 60), wxSize(200, 60));
	viewingWord->SetFont(fnt);
	viewingWord->SetBackgroundColour(black);
	viewingWord->SetForegroundColour(white);

	wxStaticText* viewingDataSet = new wxStaticText(thirdPanel, wxID_ANY, "ENG ENG", wxPoint(434, 60), wxSize(200, 60));
	viewingDataSet->SetFont(fnt);
	viewingDataSet->SetBackgroundColour(black);
	viewingDataSet->SetForegroundColour(white);

	wordView = new WordView(thirdPanel, wxPoint(134, 133), wxSize(979, 460), dict);
	wordView->SetColor(white);
	wordView->HideButtons();
	wordView->ShowRecover();

	recover->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		wxMessageDialog* ask = new wxMessageDialog(this,
			"Are you sure to recover this whole word?",
			"Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

		if (ask->ShowModal() == wxID_YES) {
			Word wrd = wordView->getShowingWord();

			if (!wrd.empty()) {
				dict->addNewWord(wrd);
				this->SetSelection(1);
			}
		}
		});


	//----LIST CTRL BIND

	listCtrl->Bind(wxEVT_LIST_ITEM_SELECTED, [this, dict, viewingDataSet](wxListEvent& evt) {
		long selectedRow = evt.GetIndex();

		wxString wxtext = listCtrl->GetItemText(selectedRow);

		//set viewing data set
		viewingDataSet->SetLabel(datasetCbb2->GetStringSelection());

		string str = string(wxtext.mb_str(wxConvUTF8));

		dict->chooseLanguage(datasetCbb2->GetStringSelection().ToStdString());

		vector<Word> get = dict->getDeletedWords();

		for (auto& w : get) {
			if (w.getText() == str) {
				wordView->processWord(w, dict);
				break;
			}
		}

		this->SetSelection(2);
		});

	//----Simple Book----//
	deletedWords->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		dict->chooseLanguage(datasetCbb2->GetStringSelection().ToStdString());
		listCtrl->DeleteAllItems();

		vector<Word> show = dict->getDeletedWords();

		for (auto& w : show) {
			string str = w.getText();


			wxString wxstr = wxString::FromUTF8(str);

			int num = w.getNumberOfDefinitions();


			int size = dict->searchWordMatchingForcedWordFinder(str).getNumberOfDefinitions() + num;

			wxString numstr = to_string(num) + "/" + to_string(size);
			int cur = listCtrl->GetItemCount();

			long itemIndex = listCtrl->InsertItem(cur, wxString::Format(wxstr, cur + 1));

			listCtrl->SetItem(itemIndex, 1, wxString::Format(numstr, cur + 1));
		}

		//-----//
		this->SetSelection(1);
		});

	backToMain->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		this->SetSelection(0);
		});

	backToSecond->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		this->SetSelection(1);
		});

	this->AddPage(mainPanel, "ADD WORD", true);
	this->AddPage(secondPanel, "VIEW DELETED DEFINITIONS", true);
	this->AddPage(thirdPanel, "VIEW SPECIFIC WORD", true);

	this->SetSelection(0);
}



AddWordMenu::~AddWordMenu() {

}

//void AddWordMenu::OnDatasetChosen(wxCommandEvent& evt) {
//	string activeDataset = datasetCbb->GetStringSelection().ToStdString();
//	wxArrayString parts;
//
//	if (activeDataset == "Eng-Eng")
//		parts = { "", "noun", "verb", "adjective", "adverb", "preposition", "conjunction" , "interjection", "plural of", "single of", "transitive verb", "intransitive verb", "past participle", "superlative", "imperative" };
//	
//	else if (activeDataset == "Vie-Eng")
//		parts = { "", "danh từ", "động từ", "tính từ", "phó từ", "giới từ", "ngoại động từ", "nội động từ", "thành ngữ" };
//	
//	else if (activeDataset == "Eng-Vie")
//		parts = { "", "danh từ", "động từ", "tính từ", "phó từ", "trạng từ", "giới từ", "liên từ", "thán từ", "số nhiều", "số ít", "ngoại động từ", "nội động từ", "quá khứ phân từ", "viết tắt", "thành ngữ"};
//		
//	partCbb->Append(parts);
//}

//void AddWordMenu::OnSubmitClicked(wxCommandEvent& evt) {
//	string activeDataset = datasetCbb->GetStringSelection().ToStdString();
//	string part;
//
//	//convert from combobox to in-file
//	if (activeDataset == "Eng-Eng") {
//		vector<string> partInFile = { "", "n.", "v.", "adj.", "adv.", "prep.", "conj.", "interj." "pl.", "sgl.", "v. t.", "v. i.", "p. p.", "superl.", "imp." };
//		part = partInFile[datasetCbb->GetCurrentSelection()];
//	}
//	else if (activeDataset == "Vie-Eng") {
//		vector<string> partInFile = { "", "dtừ", "đtừ", "ttừ", "phó từ", "gtừ", "ngđtừ", "nđtừ", "thngữ" };
//		part = partInFile[datasetCbb->GetCurrentSelection()];
//	}
//	else if (activeDataset == "Eng-Vie")
//		part = partCbb->GetStringSelection().ToStdString();
//
//}