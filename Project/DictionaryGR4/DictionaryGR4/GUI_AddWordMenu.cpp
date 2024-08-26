#include "GUI_AddWordMenu.h"

using namespace std;

AddWordMenu::AddWordMenu(wxWindow* parent, Dictionary*& dict) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

	//font
	wxFont font(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wxFont fontTitle(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_NORMAL);
	wxFont fontCB(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	fontTitle.MakeBold();

	auto purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);

	SetBackgroundColour(black);

	/*titleBar = new wxPanel(this, 10001, wxDefaultPosition, wxSize(1280, 80), wxBORDER_NONE);
	titleBar->SetBackgroundColour(wxColor(67, 57, 97));
	mainPanel = new wxPanel(this, 10001, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);
	mainPanel->SetBackgroundColour(wxColor(34, 36, 40));*/

	//this->SetBackgroundColour((0xff, 0xcc, 0xcc));

	/*wxStaticText* title = new wxStaticText(titleBar, wxID_ANY, "Dictionary", wxPoint(30, 30));
	title->SetFont(fontTitle);*/

	wordText = new wxTextCtrl(this, wxID_ANY, "", wxPoint(114, 101), wxSize(635, 60));
	wordText->SetFont(font);


	wxArrayString datasets = { "Eng-Eng", "Eng-Vie", "Vie-Eng" };
	datasetCbb = new wxComboBox(this, wxID_ANY, "Eng-Eng", wxPoint(773, 101), wxSize(154,-1), datasets, wxCB_READONLY);	
	datasetCbb->SetFont(fontCB);
	datasetCbb->SetSize(154, 60);
	datasetCbb->Refresh();

	/*wxArrayString wordTypes = { "(n.)", "(v.)", "(adj.)", "(adv.)" };
	partCbb = new wxComboBox(this, wxID_ANY, "", wxPoint(951, 101), wxSize(154, -1), wordTypes);
	partCbb->SetFont(fontCB);
	partCbb->SetSize(154, 60);
	partCbb->Refresh();*/
	

	defText = new wxTextCtrl(this, wxID_ANY, "", wxPoint(110, 240), wxSize(1000, 300), wxTE_MULTILINE);
	defText->SetFont(font);

	wordTypeText = new wxTextCtrl(this, wxID_ANY, "", wxPoint(951, 101), wxSize(154, 60));
	wordTypeText->SetFont(fontCB);
		
	submit = new wxButton(this, wxID_ANY, "ADD", wxPoint(951, 600), wxSize(154,60));
	auto fntSubmit = submit->GetFont();
	fntSubmit.SetPointSize(16);
	submit->SetFont(fntSubmit);
	submit->SetForegroundColour(white);
	submit->SetBackgroundColour(green);


	wxStaticText* wordBarText = new wxStaticText(this, wxID_ANY, "WORD", wxPoint(114, 60), wxSize(100, 20));
	auto fnt = wordBarText->GetFont();
	fnt.SetPointSize(16);
	wordBarText->SetFont(fnt);
	wordBarText->SetBackgroundColour(black);
	wordBarText->SetForegroundColour(white);

	wxStaticText* dataSetBarText = new wxStaticText(this, wxID_ANY, "DATASET", wxPoint(773, 60), wxSize(100, 20));
	dataSetBarText->SetFont(fnt);
	dataSetBarText->SetBackgroundColour(black);
	dataSetBarText->SetForegroundColour(white);

	wxStaticText* defBarText = new wxStaticText(this, wxID_ANY, "DEFINITION", wxPoint(110, 189), wxSize(100, 20));
	defBarText->SetFont(fnt);
	defBarText->SetBackgroundColour(black);
	defBarText->SetForegroundColour(white);

	wxStaticText* wordTypeBarText = new wxStaticText(this, wxID_ANY, "WORDTYPE", wxPoint(951, 60), wxSize(100, 20));
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

		if(ask->ShowModal() == wxID_YES) dict->addNewWordOneDef(text, def);
	});
	
	
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