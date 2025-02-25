#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <wx/wx.h>
#include <wx/simplebook.h>
#include <wx/spinctrl.h>
#include <wx/progdlg.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>
#include <experimental/filesystem>

#include "Dictionary.h"
#include "GUI_WordView.h"


class SearchMenu : public wxPanel {
public:
	SearchMenu(wxWindow* parent, Dictionary*& dict);
	~SearchMenu();

private:
	//Dictionary dict;
	//History hist;
	wxButton* resetbutton, * button, * deleteword, * rd_button;
	wxBitmapButton* searchByDef, * searchByWord;
	wxListBox* suggestBar;
	wxTextCtrl* searchBar;
	wxComboBox* datasetCbb;
	WordView* wordView;
	wxBitmap defbm[2] = {
	wxBitmap("IMG/defoff.png", wxBITMAP_TYPE_PNG),
	wxBitmap("IMG/defon.png", wxBITMAP_TYPE_PNG)
	};
	wxBitmap wordbm[2] = {
	wxBitmap("IMG/wordoff.png", wxBITMAP_TYPE_PNG),
	wxBitmap("IMG/wordon.png", wxBITMAP_TYPE_PNG)
	};

	void skip(wxMouseEvent& evt);

	//suggest bars
	void adjustSuggestBar(int maxHeight, int maxItem);

	//word view
	void OnViewWord(wxCommandEvent& evt, Dictionary* dict);

	//searchBar
	void OnSearchAndSuggestHandler(wxCommandEvent& evt, Dictionary* dict);

	//microscope button

	void OnSearchButton(wxCommandEvent& evt, Dictionary* dict);

	void OnSearchingByDef(wxCommandEvent& evt, Dictionary* dict);

	void OnSearchingByWord(wxCommandEvent& evt, Dictionary* dict);

	void OnResetButtonClicked(wxCommandEvent& evt, Dictionary* dict);

	void OnRandomClicked(wxCommandEvent& evt, Dictionary* dict);

};