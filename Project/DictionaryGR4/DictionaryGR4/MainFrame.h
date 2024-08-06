#pragma once

#include "MainMenu.h"
#include "SearchMenu.h"

class MainFrame: public wxFrame {
private:
	wxSimplebook* simpleBook;
public: 
	MainFrame(const wxString& Title, wxSize FrameSize);

    void OnPreviousPage(wxCommandEvent& event);

	void OnNextPage(wxCommandEvent& event);
};