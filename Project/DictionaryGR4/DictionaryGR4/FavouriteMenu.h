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
class FavouriteMenu : public wxPanel
{
public:
	FavouriteMenu(wxWindow* parent, Dictionary*& dict);
	~FavouriteMenu();
private:
	wxTextCtrl* searchBar;
	WordView* wordView;
	wxButton* back, * view;
	wxListCtrl* list;



	void OnViewButton(wxCommandEvent& evt, Dictionary* dict);
	void OnBackButton(wxCommandEvent& evt, Dictionary* dict);
};

