#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <wx/wx.h>
#include <wx/simplebook.h>
#include <wx/spinctrl.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>
#include <experimental/filesystem>

#include "Dictionary.h"
#include "GUI_WordView.h"

class SearchMenu : public wxWindow {
public:
	wxButton* back_to_home;
	SearchMenu(wxWindow* parent);
	~SearchMenu();

private:
	Dictionary dict;
	//History hist;
	wxPanel* titleBar, * mainPanel;
	wxButton* button, * searchByDef, * searchByWord, * rd_button;
	wxListBox* suggestBar;
	wxTextCtrl* searchBar;
	wxComboBox* datasetCbb;
	WordView* wordView;


	void skip(wxMouseEvent& evt);

	//suggest bars
	void adjustSuggestBar(int maxHeight, int maxItem);

	//word view
	void OnViewWord(wxCommandEvent& evt);

	//searchBar
	void OnSearchAndSuggestHandler(wxCommandEvent& evt);

	//microscope button

	void OnSearchButton(wxCommandEvent& evt);

	void OnLoadTool(wxCommandEvent& evt);

	void OnUnLoadTool(wxCommandEvent& evt);

	void OnResetButtonClicked(wxCommandEvent& evt);

	void OnRandomClicked(wxCommandEvent& evt);
};
