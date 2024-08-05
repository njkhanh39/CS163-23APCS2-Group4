#pragma once
#include "Dictionary.h"

class WordView {
private:
	wxBoxSizer* frame;
	wxPanel* panel;
	wxStaticText* text, *wordTypeText, *defText, *pageText;
	wxButton* fav, *next, *back;

	vector<string> displayDef, actualDef;
	vector<string> wordtype;

	int cur = 0;
	int pages = 0;

public:
	
	

	WordView() {

	}

	WordView(wxWindow* parent, wxPoint pos, wxSize size) {
		wxFont font(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		wxFont largerFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		largerFont.MakeBold();

		frame = new wxBoxSizer(wxVERTICAL);

		panel = new wxPanel(parent, wxID_ANY, pos, size, wxTAB_TRAVERSAL | wxBORDER_SIMPLE);
		frame->Add(panel, 1, wxEXPAND | wxALL, 5);
		

		text = new wxStaticText(panel, wxID_ANY, "hello", wxPoint(size.x / 20, size.y/20), wxDefaultSize);
		text->SetFont(largerFont);

		wordTypeText = new wxStaticText(panel, wxID_ANY, "wordtype", wxPoint(size.x / 20, 3*size.y / 20), wxDefaultSize);
		wordTypeText->SetFont(font);

		defText = new wxStaticText(panel, wxID_ANY, "def", wxPoint(size.x / 20, size.y/3), wxDefaultSize);
		defText->SetFont(font);

		next = new wxButton(panel, 10017, "Next", wxPoint(5 * size.x / 6, 0), size / 6);
		back = new wxButton(panel, 10016, "Back", wxPoint(2 * size.x / 3, 0), size / 6);
		
		pageText = new wxStaticText(panel, wxID_ANY, "0/0", wxPoint(6*size.x / 10, 0), wxDefaultSize);
		pageText->SetFont(font);

		next->SetFont(font);
		back->SetFont(font);

		next->Bind(wxEVT_BUTTON, &WordView::showWord, this);
		back->Bind(wxEVT_BUTTON, &WordView::showWord, this);
	}

	void Disable() {
		panel->Disable();
	}

	void Enable() {
		panel->Enable();
	}

	void SetColor(wxColor clr) {
		panel->SetBackgroundColour(clr);
	}

	void processWord(Word& word) {
		//careful with this, must set cur = 0 when loading newword
		if (word.empty()) return;

		cur = 0;
		pages = word.getNumberOfDefinitions();
		displayDef.clear(); wordtype.clear(); actualDef.clear();
		displayDef.assign(pages, "");
		actualDef.assign(pages, "");
		wordtype.assign(pages, "");

		for (int i = 0; i < pages; ++i) {
			string str = word.getDefinitionAt(i).getStringDefinition();

			int j = 0;
			while (j < (int)str.length() && (j == 0 || str[j - 1] != ')')) {
				wordtype[i].push_back(str[j]);
				++j;
			}

			++j;

			displayDef[i] = str.substr(j, (int)str.length() - j);
			actualDef[i] = displayDef[i];

			int cnt = 0;
			for (int k = 0; k < (int)displayDef[i].length(); ++k) {
				++cnt;
				if (cnt >= 50 && displayDef[i][k] == ' ') {
					displayDef[i][k] = '\n';
					cnt = 0;
				}
			}
		}
		wxString unicodestr = wxString::FromUTF8(word.getWord());
		if (text) text->SetLabel(unicodestr);
		if (wordTypeText) wordTypeText->SetLabel(wxString::FromUTF8(wordtype[cur]));
		if (defText) defText->SetLabel(wxString::FromUTF8(displayDef[cur]));
		if (pageText) {
			string show = to_string(cur + 1) + "/" + to_string(pages);
			pageText->SetLabel(show);
		}

		// Update the layout to reflect the new size
		//Layout();
	}

	void showWord(wxCommandEvent& evt) {
		int id = evt.GetId();
		if (pages == 0) return;
		if (id == 10017) cur = (cur + 1) % pages;
		else {
			if (cur == 0) cur = pages - 1;
			else cur = (cur - 1) % pages;
		}

		if (!(cur >= 0 && cur < pages)) return;
		
		if (wordTypeText) wordTypeText->SetLabel(wxString::FromUTF8(wordtype[cur]));
		if (defText) defText->SetLabel(wxString::FromUTF8(displayDef[cur]));
		if (pageText) {
			string show = to_string(cur + 1) + "/" + to_string(pages);
			pageText->SetLabel(show);
		}

	}

	void goToDefinition(string key) {
		for (int i = 0; i < pages; ++i) {
			if ((wordtype[i] +" " + actualDef[i]) == key) {
				cur = i;
				if (wordTypeText) wordTypeText->SetLabel(wxString::FromUTF8(wordtype[cur]));
				if (defText) defText->SetLabel(wxString::FromUTF8(displayDef[cur]));
				if (pageText) {
					string show = to_string(cur + 1) + "/" + to_string(pages);
					pageText->SetLabel(show);
				}
				break;
			}
		}
	}
};