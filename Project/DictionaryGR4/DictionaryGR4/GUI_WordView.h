#pragma once
#include <codecvt>
#include "Dictionary.h"

class WordView {
private:

	wxWindow* parentWindow;
	wxPanel* panel;
	wxBoxSizer* frame;
	
	wxStaticText* text, *wordTypeText, *pageText;
	wxTextCtrl* defText;
	wxButton* fav, *next, *back, *editDef, *confirmEdit, *cancelEdit, *removeDef;

	Word* word;
	string activeDataset;
	vector<string> defs;
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

		defText = new wxTextCtrl(panel, wxID_ANY, "def", wxPoint(size.x / 20, size.y/3), wxSize(500, 200), wxTE_MULTILINE);
		defText->SetEditable(0);
		defText->SetFont(font);

		next = new wxButton(panel, 10017, "Next", wxPoint(5 * size.x / 6, 0), size / 6);
		back = new wxButton(panel, 10016, "Back", wxPoint(2 * size.x / 3, 0), size / 6);
		
		pageText = new wxStaticText(panel, wxID_ANY, "0/0", wxPoint(6*size.x / 10, 0), wxDefaultSize);
		pageText->SetFont(font);

		next->SetFont(font);
		back->SetFont(font);

		editDef = new wxButton(panel, wxID_ANY, "Edit", wxPoint(size.x * 5 / 6, size.y * 3 / 6));
		confirmEdit = new wxButton(panel, wxID_ANY, "Confirm", wxPoint(size.x * 5 / 6, size.y * 4 / 6));
		cancelEdit = new wxButton(panel, wxID_ANY, "Cancel", wxPoint(size.x * 5 / 6, size.y * 5 / 6));

		removeDef = new wxButton(panel, wxID_ANY, "Remove", wxPoint(size.x * 5 / 6, size.y * 2 / 6));

		confirmEdit->Hide();
		cancelEdit->Hide();

		//panel->Bind(wxEVT_LEFT_DOWN, &WordView::skip, this);
		next->Bind(wxEVT_BUTTON, &WordView::showWord, this);
		back->Bind(wxEVT_BUTTON, &WordView::showWord, this);
		editDef->Bind(wxEVT_BUTTON, &WordView::OnEditDefClicked, this);
		confirmEdit->Bind(wxEVT_BUTTON, &WordView::OnConfirmEditClicked, this);
		cancelEdit->Bind(wxEVT_BUTTON, &WordView::OnCancelEditClicked, this);
		removeDef->Bind(wxEVT_BUTTON, &WordView::OnRemoveDefClicked, this);
		
		parentWindow = parent;
	}

	// Convert UTF-8 to UTF-16
	std::u16string utf8ToUtf16(const std::string& utf8Str) {
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
		return convert.from_bytes(utf8Str);
	}

	// Convert UTF-16 to UTF-8
	std::string utf16ToUtf8(const std::u16string& utf16Str) {
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
		return convert.to_bytes(utf16Str);
	}

	void setWord(Word* newWord) {
		word = newWord;
	}

	void setActiveDataset(string dataset) {
		activeDataset = dataset;
	}

	void skip(wxMouseEvent& evt){
		evt.Skip();
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
		//careful with this, must set cur = 0 when loading new word
		if (word.empty()) return;

		cur = 0;
		pages = word.getNumberOfDefinitions();
		defs.clear(); wordtype.clear();
		defs.assign(pages, "");
		wordtype.assign(pages, "");

		for (int i = 0; i < pages; ++i) {
			string str = word.getDefinitionAt(i).getStringDefinition();

			int j = 0;
			while (j < (int)str.length() && (j == 0 || str[j - 1] != ')')) {
				wordtype[i].push_back(str[j]);
				++j;
			}

			++j;

			defs[i] = str.substr(j, (int)str.length() - j);

			//int cnt = 0;
			//for (int k = 0; k < (int)displayDef[i].length(); ++k) {
			//	++cnt;
			//	if (cnt >= 55 && displayDef[i][k] == ' ') {
			//		displayDef[i][k] = '\n';
			//		cnt = 0;
			//	}
			//}
		}
		wxString unicodestr = wxString::FromUTF8(word.getWord());
		if (text) text->SetLabel(unicodestr);
		if (wordTypeText) wordTypeText->SetLabel(wxString::FromUTF8(wordtype[cur]));
		if (defText) defText->SetLabel(wxString::FromUTF8(defs[cur]));
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
		if (defText) defText->SetLabel(wxString::FromUTF8(defs[cur]));
		if (pageText) {
			string show = to_string(cur + 1) + "/" + to_string(pages);
			pageText->SetLabel(show);
		}

	}

	void goToDefinition(string key) {
		for (int i = 0; i < pages; ++i) {
			if ((wordtype[i] + " " + defs[i]) == key) {
				cur = i;
				if (wordTypeText) wordTypeText->SetLabel(wxString::FromUTF8(wordtype[cur]));
				if (defText) defText->SetLabel(wxString::FromUTF8(defs[cur]));
				if (pageText) {
					string show = to_string(cur + 1) + "/" + to_string(pages);
					pageText->SetLabel(show);
				}
				break;
			}
		}
	}

	int getCurrentPage() {
		wxString page = pageText->GetLabelText();
		return wxAtoi(page.Left(page.First('/'))) - 1;
	}

	void OnEditDefClicked(wxCommandEvent& evt) {
		defText->SetEditable(1);
		defText->SetFocus();
		defText->SetInsertionPointEnd();

		confirmEdit->Show();
		cancelEdit->Show();
	}

	void OnConfirmEditClicked(wxCommandEvent& evt) {
		wxString page = pageText->GetLabelText();
		int curIndex = wxAtoi(page.Left(page.First('/'))) - 1;

		wxMessageDialog* ask = new wxMessageDialog(parentWindow,
			"Are you sure to modify this definition?",
			"Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

		if (ask->ShowModal() == wxID_YES) {
			string newDef = defText->GetValue().ToStdString();
			defs[curIndex] = newDef;
			word->modifyDefinition(newDef, curIndex);
		}

		

		defText->SetEditable(0);
		confirmEdit->Hide();
		cancelEdit->Hide();
	}

	void OnCancelEditClicked(wxCommandEvent& evt) {
		defText->SetEditable(0);

		int curIndex = getCurrentPage();

		if (curIndex < 0)
			defText->SetLabel(wxString("Definition."));
		else
			defText->SetLabel(wxString::FromUTF8(defs[curIndex]));

		confirmEdit->Hide();
		cancelEdit->Hide();
	}

	void OnRemoveDefClicked(wxCommandEvent& evt) {
		int curIndex = getCurrentPage();

		wxMessageDialog* ask = new wxMessageDialog(parentWindow,
			"Are you sure to remove this definition?",
			"Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

		if (ask->ShowModal() == wxID_YES) {
			defs.erase(defs.begin() + curIndex);
			wordtype.erase(wordtype.begin() + curIndex);
			--pages;
			if (curIndex == pages)
				--curIndex;
			pageText->SetLabel(to_string(curIndex + 1) + "/" + to_string(pages));
			wordTypeText->SetLabel(wxString::FromUTF8(wordtype[curIndex]));
			defText->SetLabel(wxString::FromUTF8(defs[curIndex]));
			
			word->removeDefinition(curIndex);
		}
	}

};