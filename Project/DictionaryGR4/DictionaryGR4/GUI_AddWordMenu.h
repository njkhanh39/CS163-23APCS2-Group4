#pragma once

#include <wx/wx.h>
#include <wx/simplebook.h>
#include <wx/spinctrl.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>

#include "Dictionary.h"
#include "GUI_WordView.h"

class AddWordMenu : public wxPanel {
public:
	wxButton* back_to_home;
	AddWordMenu(wxWindow* parent, Dictionary*& dict);
	~AddWordMenu();

private:
	Dictionary dict;
	wxPanel* titleBar, * mainPanel;
	wxTextCtrl* wordText, * defText, *wordTypeText;
	wxComboBox* datasetCbb;
	wxButton* submit;

	void OnDatasetChosen(wxCommandEvent& evt);

	void OnSubmitClicked(wxCommandEvent& evt);
};