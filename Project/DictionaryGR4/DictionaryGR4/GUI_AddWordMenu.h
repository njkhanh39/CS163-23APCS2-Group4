#pragma once

#include <wx/wx.h>
#include <wx/simplebook.h>
#include <wx/spinctrl.h>
#include <wx/grid.h>
#include <wx/listctrl.h>

#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>

#include "Dictionary.h"
#include "GUI_WordView.h"

class AddWordMenu : public wxSimplebook {
public:
	wxButton* back_to_home;
	AddWordMenu(wxWindow* parent, Dictionary*& dict);
	~AddWordMenu();

private:
	Dictionary dict;

	//PAGE 1

	wxPanel* mainPanel;

	wxTextCtrl* wordText, * defText, * wordTypeText;
	wxComboBox* datasetCbb;

	wxButton* submit, * deletedWords;


	//PAGE 2


	wxPanel* secondPanel;

	wxListCtrl* listCtrl;
	wxComboBox* datasetCbb2;

	wxButton* backToMain;

	//PAGE 3

	wxPanel* thirdPanel;

	WordView* wordView;

	wxButton* backToSecond;
	wxButton* recover;
};