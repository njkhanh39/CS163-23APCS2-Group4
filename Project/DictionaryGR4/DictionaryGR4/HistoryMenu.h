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
#include "GUI_WordView.h"
#include "Dictionary.h"
#include <wx/listctrl.h>

class HistoryMenu : public wxPanel
{
public:
	HistoryMenu(wxWindow* parent, Dictionary*& dict);
	void Refresh(Dictionary* dict);
	~HistoryMenu();
private:
	wxTextCtrl* searchBar;
	WordView* wordView;
	wxButton* back, * clear;
	wxListCtrl* list;
	wxComboBox* datasetCbb;
	string ViewedDataset;

	void OnViewButton(wxCommandEvent& evt, Dictionary* dict);
	void OnBackButton(wxCommandEvent& evt, Dictionary* dict);
	void OnDataSetChanged(wxCommandEvent& evt, Dictionary* dict);
	void OnClearButton(wxCommandEvent& evt, Dictionary* dict);

};