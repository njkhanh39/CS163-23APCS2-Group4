#include "GUI_AddWordMenu.h"

using namespace std;

AddWordMenu::AddWordMenu(wxWindow* parent, Dictionary*& dict) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

	//font
	wxFont font(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
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

	wordText = new wxTextCtrl(this, wxID_ANY, "", wxPoint(114, 161), wxSize(635, 60));
	wordText->SetFont(fontCB);

	wxArrayString datasets = { "Eng-Eng", "Eng-Vie", "Vie-Eng" };
	datasetCbb = new wxComboBox(this, wxID_ANY, "", wxPoint(773, 161), wxSize(210, 60), datasets, wxCB_READONLY);	
	datasetCbb->SetFont(fontCB);
	datasetCbb->SetSize(154, 60);

	partCbb = new wxComboBox(this, wxID_ANY, "", wxPoint(1007, 161), wxSize(154, 60));

	defText = new wxTextCtrl(this, wxID_ANY, "", wxPoint(110, 300), wxSize(700, 300));
		
	submit = new wxButton(this, wxID_ANY, "Submit", wxPoint(1000, 600), wxDefaultSize);

	datasetCbb->Bind(wxEVT_COMBOBOX, [this, dict](wxCommandEvent& evt) {
		dict->chooseLanguage(datasetCbb->GetStringSelection().ToStdString());
	});

	submit->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		string text = (string)wordText->GetValue();
		string def = (string)defText->GetValue();
		dict->addNewWordOneDef(text, def);
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