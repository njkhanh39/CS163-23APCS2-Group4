#pragma once
#include <codecvt>
#include "Dictionary.h"

class WordView {
private:

	wxWindow* parentWindow;
	wxPanel* panel;
	wxBoxSizer* frame;
	
	wxStaticText* text, *wordTypeText;
	wxTextCtrl* defText, *pageText;
	wxButton* confirmEdit, *cancelEdit;
	wxBitmapButton* back, * next, *editDef, *delDef, *favDef;
	wxBitmap favon,  favoff;
		
	vector<string> defs;
	vector<string> wordtype;

	Word* word = nullptr;
	string activeDataset;

	int cur = -1;
	int pages = 0;

	//---FOR ADD WORD MENU
	wxButton* recover = nullptr;

public:
	wxColour  purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);

	WordView() {

	}

	WordView(wxWindow* parent, wxPoint pos, wxSize size, Dictionary* dict) {
		wxFont font(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		wxFont largerFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		largerFont.MakeBold();

		wxFont fnt, ifnt, bfnt;

		frame = new wxBoxSizer(wxVERTICAL);

		panel = new wxPanel(parent, wxID_ANY, pos, size, wxTAB_TRAVERSAL | wxBORDER_SIMPLE);
		frame->Add(panel, 1, wxEXPAND | wxALL, 5);

		text = new wxStaticText(panel, wxID_ANY, "Word-displayed-here", wxPoint(size.x / 20, size.y / 20), wxDefaultSize);
		fnt = text->GetFont();
		ifnt = fnt; bfnt = fnt;
		ifnt.MakeItalic(); bfnt.MakeBold();
		ifnt.SetPointSize(13); fnt.SetPointSize(15); bfnt.SetPointSize(25);
		text->SetFont(bfnt);

		wordTypeText = new wxStaticText(panel, wxID_ANY, "(Wordtype displayed here.)", wxPoint(size.x / 20, 3.5 * size.y / 20), wxSize(500, 200));
		wordTypeText->SetFont(ifnt);

		defText = new wxTextCtrl(panel, wxID_ANY, "Definition displayed here.", wxPoint(size.x / 10, size.y / 3.5), wxSize(8 * size.x / 10, 250), wxTE_MULTILINE | wxTE_NO_VSCROLL | wxBORDER_NONE);
		defText->SetBackgroundColour(white);
		defText->SetEditable(0);
		defText->SetFont(fnt);

		//next = new wxButton(panel, 10017, "Next", wxPoint(5 * size.x / 6, 0), size / 6);
		//back = new wxButton(panel, 10016, "Back", wxPoint(2 * size.x / 3, 0), size / 6);

		wxBitmap bitmapnext(wxT("IMG/nextdef.png"), wxBITMAP_TYPE_PNG);
		wxBitmap bitmapback(wxT("IMG/prevdef.png"), wxBITMAP_TYPE_PNG);

		back = new wxBitmapButton(panel, 10016, bitmapback, wxPoint(0, 194), wxSize(53, 53), wxBORDER_NONE);
		next = new wxBitmapButton(panel, 10017, bitmapnext, wxPoint(926, 194), wxSize(53, 53), wxBORDER_NONE);
		back->SetBackgroundColour(white);
		next->SetBackgroundColour(white);


		wxBitmap bitmapdel(wxT("IMG/delbutton.png"), wxBITMAP_TYPE_PNG);
		wxBitmap bitmapedit(wxT("IMG/editbutton.png"), wxBITMAP_TYPE_PNG);
		favoff = wxBitmap(wxT("IMG/favbutton.png"), wxBITMAP_TYPE_PNG);
		favon = wxBitmap(wxT("IMG/favon.png"), wxBITMAP_TYPE_PNG);

		editDef = new wxBitmapButton(panel, wxID_ANY, bitmapedit, wxPoint(744, 20), wxSize(44, 44), wxBORDER_NONE);
		delDef = new wxBitmapButton(panel, wxID_ANY, bitmapdel, wxPoint(808, 20), wxSize(44, 44), wxBORDER_NONE);
		favDef = new wxBitmapButton(panel, wxID_ANY, favoff, wxPoint(872, 20), wxSize(44, 44), wxBORDER_NONE);

		editDef->SetBackgroundColour(white);
		delDef->SetBackgroundColour(white);
		favDef->SetBackgroundColour(white);

		confirmEdit = new wxButton(panel, wxID_ANY, "Confirm", wxPoint(744, 380), wxSize(91, 44));
		confirmEdit->SetBackgroundColour(wxColour(67, 57, 97));
		confirmEdit->SetForegroundColour(wxColour(255, 255, 255));

		cancelEdit = new wxButton(panel, wxID_ANY, "Cancel", wxPoint(845, 380), wxSize(91, 44));
		cancelEdit->SetBackgroundColour(wxColour(67, 57, 97));
		cancelEdit->SetForegroundColour(wxColour(255, 255, 255));

		//removeDef = new wxButton(panel, wxID_ANY, "Remove", wxPoint(size.x * 5 / 6, size.y * 2 / 6));

		pageText = new wxTextCtrl(panel, wxID_ANY, "0/0", wxPoint(808, 3 * size.y / 20), wxSize(100, 30), wxTE_RIGHT | wxTE_PROCESS_ENTER | wxBORDER_NONE);
		pageText->SetFont(ifnt);
		//pageText->SetBackgroundColour(purple);

		confirmEdit->Hide();
		cancelEdit->Hide();


		//panel->Bind(wxEVT_LEFT_DOWN, &WordView::skip, this);
		next->Bind(wxEVT_BUTTON, &WordView::showWord, this);
		back->Bind(wxEVT_BUTTON, &WordView::showWord, this);
		editDef->Bind(wxEVT_BUTTON, &WordView::OnEditDefClicked, this);
		confirmEdit->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
			OnConfirmEditClicked(evt, dict);
			});
		cancelEdit->Bind(wxEVT_BUTTON, &WordView::OnCancelEditClicked, this);
		delDef->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
			OnRemoveDefClicked(evt, dict);
			});
		favDef->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
			OnAddAndUnFavourite(evt, dict);
		});
		defText->Bind(wxEVT_SET_FOCUS, &WordView::OnTextFocus, this);
		defText->Bind(wxEVT_KILL_FOCUS, &WordView::OnDefTextKillFocus, this);

		pageText->Bind(wxEVT_TEXT_ENTER, &WordView::OnPageTextChanged, this);

		//----ADD WORD
		recover = new wxButton(panel, wxID_ANY, "Recover", wxPoint(10 * size.x / 20, 2 * size.y / 20), wxSize(100, 30));
		recover->Hide();

		if (recover) {
			recover->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
				OnRecoverWord(evt, dict);
				});
		}

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

	void skip(wxMouseEvent& evt) {
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

	void SetBackDefault() {
		cur = -1;
		pages = 0;
		defs.clear();
		wordtype.clear();

		pageText->SetValue("0/0");
		text->SetLabel("Word-displayed here");
		wordTypeText->SetLabel("Wordtype displayed here");
		defText->SetLabel("Definition displayed here.");
	}

	void Lower() {
		panel->Lower();
		text->Lower();
		wordTypeText->Lower();
		defText->Lower();
		back->Lower();
		next->Lower();
	}

	Word getShowingWord() {
		Word ans;
		if (string(pageText->GetValue().mb_str(wxConvUTF8)) == "0/0") return ans;

		wxString wxstr = text->GetLabel();
		string str = string(wxstr.mb_str(wxConvUTF8));

		ans.setWord(str);

		for (int i = 0; i < (int)defs.size(); ++i) {
			string defstr;
			if (wordtype[i] == "") {
				defstr = defs[i];
			}
			else {
				defstr = wordtype[i] + " " + defs[i];
			}

			ans.addDefinition(defstr);
		}

		return ans;
	}

	void processWord(Word& processWord, Dictionary* dict) {
		//careful with this, must set cur = 0 when loading new word
		if (processWord.empty()) return;

		cur = 0;
		pages = processWord.getNumberOfDefinitions();
		defs.clear(); wordtype.clear();
		defs.assign(pages, "");
		wordtype.assign(pages, "");

		for (int i = 0; i < pages; ++i) {
			string str = processWord.getDefinitionAt(i).getStringDefinition();

			int j = 0;
			if (str[j] == '(') {
				while (j < (int)str.length() && (j == 0 || str[j - 1] != ')')) {
					wordtype[i].push_back(str[j]);
					++j;
				}
				++j;
			}
			else wordtype[i] = "";


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
		wxString unicodestr = wxString::FromUTF8(processWord.getText());
		if (text) text->SetLabel(unicodestr);
		if (wordTypeText) wordTypeText->SetLabel(wxString::FromUTF8(wordtype[cur]));
		if (defText) defText->SetLabel(wxString::FromUTF8(defs[cur]));
		if (pageText) {
			string show = to_string(cur + 1) + "/" + to_string(pages);
			pageText->SetValue(show);
		}

		bool isFav = true;

		//check fav
		if (text) {
			wxString checkFavwx = text->GetLabel();

			string checkFavStr = string(checkFavwx.mb_str(wxConvUTF8));

			isFav = dict->checkFav(checkFavStr);
		}

		if (isFav) {
			//red
			favDef->SetBitmap(favon);
		}
		else favDef->SetBitmap(favoff);

		// Update the layout to reflect the new size
		//Layout();
	}

	void clearWordView() {
		cur = -1;
		pages = 0;
		defs.clear();
		wordtype.clear();

		pageText->SetValue("");
		text->SetLabel("");
		wordTypeText->SetLabel("");
		defText->SetLabel("");
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
			pageText->SetValue(show);
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
					pageText->SetValue(show);
				}
				break;
			}
		}
	}

	void OnEditDefClicked(wxCommandEvent& evt) {
		defText->SetEditable(1);
		defText->SetFocus();
		defText->SetInsertionPointEnd();

		confirmEdit->Show();
		cancelEdit->Show();
	}

	void OnConfirmEditClicked(wxCommandEvent& evt, Dictionary* dict) {
		int curIndex = cur;

		wxMessageDialog* ask = new wxMessageDialog(parentWindow,
			"Are you sure to modify this definition?",
			"Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

		if (ask->ShowModal() == wxID_YES and curIndex >= 0) {
			parentWindow->SetCursor(wxCURSOR_WAIT);

			string newDef = string(defText->GetValue().mb_str(wxConvUTF8));
			dict->editDefinition(string(text->GetLabel().mb_str(wxConvUTF8)), defs[curIndex], newDef, string(wordTypeText->GetLabel().mb_str(wxConvUTF8)));
			defs[curIndex] = newDef;
			if (word->empty())
				word->modifyDefinition(newDef, curIndex);

			parentWindow->SetCursor(wxNullCursor);
		}

		defText->SetEditable(0);
		confirmEdit->Hide();
		cancelEdit->Hide();
		cur = curIndex;
	}

	void OnCancelEditClicked(wxCommandEvent& evt) {
		defText->SetEditable(0);

		int curIndex = cur;

		if (curIndex < 0)
			defText->SetLabel(wxString("Definition displayed here."));
		else
			defText->SetLabel(wxString::FromUTF8(defs[curIndex]));

		confirmEdit->Hide();
		cancelEdit->Hide();
	}

	void OnTextFocus(wxFocusEvent& evt) {
		evt.Skip();
	}

	void OnDefTextKillFocus(wxFocusEvent& evt) {
		wxWindow* curFocus = wxWindow::FindFocus();

		if (defText->IsEditable() and curFocus != confirmEdit and curFocus != cancelEdit) {
			wxMessageDialog* ask = new wxMessageDialog(parentWindow,
				"Do you want to continue the ongoing definition editing?",
				"Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

			if (ask->ShowModal() == wxID_YES) {
				defText->SetFocus();
				defText->SetInsertionPointEnd();
			}

			else {
				defText->SetEditable(0);

				int curIndex = cur;

				if (curIndex < 0)
					defText->SetLabel(wxString("Definition."));
				else
					defText->SetLabel(wxString::FromUTF8(defs[curIndex]));

				confirmEdit->Hide();
				cancelEdit->Hide();
			}
		}
		evt.Skip();
	}

	void OnRemoveDefClicked(wxCommandEvent& evt, Dictionary* dict) {

		if (pages == 0) return;

		int curIndex = cur;

		wxString wxstr = text->GetLabel();
		string wordText = string(wxstr.mb_str(wxConvUTF8));

		string defstr;

		if (wordtype[curIndex].empty()) {
			defstr = defs[curIndex];
		}
		else {
			defstr = wordtype[curIndex] + " " + defs[curIndex];
		}

		wxMessageDialog* ask = new wxMessageDialog(parentWindow,
			"Are you sure to remove this definition?",
			"Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

		if (ask->ShowModal() == wxID_YES) {
			defs.erase(defs.begin() + curIndex);
			wordtype.erase(wordtype.begin() + curIndex);
			--pages;
			if (curIndex == pages)
				--curIndex;
			pageText->SetValue(to_string(curIndex + 1) + "/" + to_string(pages));

			if (!wordtype.empty()) wordTypeText->SetLabel(wxString::FromUTF8(wordtype[curIndex]));
			else wordTypeText->SetLabel("");

			if (curIndex != -1) defText->SetLabel(wxString::FromUTF8(defs[curIndex]));
			else defText->SetLabel("");

			if (curIndex == -1) text->SetLabel("");

			cur = curIndex;
			if (curIndex == -1) cur = 0;

			dict->deleteWordOneDef(wordText, defstr);
		}
	}

	void OnAddAndUnFavourite(wxCommandEvent& evt, Dictionary* dict) {
		if (pages == 0) return;
		wxString wxstr = text->GetLabel();
		string wordText = string(wxstr.mb_str(wxConvUTF8));

		bool isFav = dict->checkFav(wordText);

		//add to fav
		if (!isFav) {
			favDef->SetBitmap(favon);
			dict->addToFavourite(wordText);
		}
		else {
			dict->removeFavourite(wordText);
			favDef->SetBitmap(favoff);
		}

		/*if (favDef->GetBackgroundColour() == wxColour(184, 89, 89)) {
			favDef->SetBackgroundColour(wxColour(255, 255, 255));
		}
		else favDef->SetBackgroundColour(wxColour(184, 89, 89));*/

	}

	void OnPageTextChanged(wxCommandEvent& evt) {
		wxString page = pageText->GetValue();
		int enteredPage = wxAtoi(page.Left(page.First('/')));

		if (enteredPage > pages)
			cur = pages - 1;
		else if (enteredPage <= 0)
			cur = 0;
		else
			cur = enteredPage - 1;

		if (pages) {
			wordTypeText->SetLabel(wxString::FromUTF8(wordtype[cur]));
			defText->SetLabel(wxString::FromUTF8(defs[cur]));
			string show = to_string(cur + 1) + "/" + to_string(pages);
			pageText->SetValue(show);
		}
	}

	wxButton* getEditDefButton() {
		return editDef;
	}

	wxButton* getDelDefButton() {
		return delDef;
	}

	wxButton* getFavDefButton() {
		return favDef;
	}

	void Hide() {
		panel->Hide();
		text->Hide();
		wordTypeText->Hide();
		defText->Hide();
		back->Hide();
		next->Hide();
		pageText->Hide();


	}
	void Show() {
		panel->Show();
		text->Show();
		wordTypeText->Show();
		defText->Show();
		back->Show();
		next->Show();
		pageText->Show();
	}
	//*editDef, *delDef, *favDef

	//-----------FOR ADD WORD MENU EXTRA TASK-------------//
	void HideButtons() {
		editDef->Hide();
		delDef->Hide();
		favDef->Hide();
	}

	void ShowRecover() {
		recover->Show();
	}

	void OnRecoverWord(wxCommandEvent& evt, Dictionary* dict) {
		if (pages == 0) return;

		int curIndex = cur;

		wxString wxstr = text->GetLabel();
		string wordText = string(wxstr.mb_str(wxConvUTF8));

		string defstr;

		if (wordtype[curIndex].empty()) {
			defstr = defs[curIndex];
		}
		else {
			defstr = wordtype[curIndex] + " " + defs[curIndex];
		}

		wxMessageDialog* ask = new wxMessageDialog(parentWindow,
			"Are you sure to recover this definition?",
			"Confirmation", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

		if (ask->ShowModal() == wxID_YES) {
			defs.erase(defs.begin() + curIndex);
			wordtype.erase(wordtype.begin() + curIndex);
			--pages;
			if (curIndex == pages)
				--curIndex;
			pageText->SetValue(to_string(curIndex + 1) + "/" + to_string(pages));

			if (!wordtype.empty()) wordTypeText->SetLabel(wxString::FromUTF8(wordtype[curIndex]));
			else wordTypeText->SetLabel("");

			if (curIndex != -1) defText->SetLabel(wxString::FromUTF8(defs[curIndex]));
			else defText->SetLabel("");

			if (curIndex == -1) text->SetLabel("");

			cur = curIndex;
			if (curIndex == -1) cur = 0;

			dict->addNewWordOneDef(wordText, defstr);
		}
	}

	string getShowingText() {
		wxString wxstr = text->GetLabel();
		string str = string(wxstr.mb_str(wxConvUTF8));

		return str;
	}
};