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
#include <wx/listctrl.h>

#include "Dictionary.h"
#include "GUI_WordView.h"
class FavouriteMenu : public wxSimplebook
{
public:
	FavouriteMenu(wxWindow* parent, Dictionary*& dict);
	~FavouriteMenu();
private:

	//----FIRST PAN----//
	wxPanel* mainPanel;

	wxComboBox* datasetCbb;
	wxListCtrl* listCtrl;

	//----SECOND PAN----//
	wxPanel* secondPanel;

	WordView* wordView;
	wxButton* back, *unfav;

public:

	void OnLoadFavourite(wxCommandEvent& evt, Dictionary* dict);
	void OnViewWord(wxListEvent& evt, Dictionary* dict);
	void OnFavButtonClicked(wxCommandEvent& evt, Dictionary* dict);
};

