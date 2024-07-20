#pragma once
#include <wx/wx.h>
#include "Dictionary.h"

class MainFrame : public wxFrame {
public:
	MainFrame(const wxString& title);
	~MainFrame();
private:
	Dictionary dict;
	wxPanel* panel, *panel2;
	wxButton* button, *runTool;
	wxListBox* suggestBar, *wordView;
	wxTextCtrl* searchBar;

	void OnMousePosition(wxMouseEvent& evt);	

	void adjustSuggestBar(int maxHeight, int maxItem) {
		// Calculate the new size for the list box based on the number of items
		
		int itemCount = suggestBar->GetCount();        // Get the number of items
		
		int newHeight;

		if (itemCount == 0) newHeight = 0;
		else newHeight = maxHeight * itemCount / maxItem;

		if (newHeight > maxHeight) newHeight = maxHeight;

		// Set the new size for the list box
		wxSize currentSize = suggestBar->GetSize();
		suggestBar->SetSize(wxSize(currentSize.GetWidth(), newHeight));

		// Update the layout to reflect the new size
		Layout();
	}

	void OnViewWord(wxCommandEvent& evt);

	void OnTextCtrlWordToDef(wxCommandEvent& evt);
	
	void OnButtonDefToWord(wxCommandEvent& evt);

	void OnLoadTool(wxCommandEvent& evt) {
		if (!dict.isToolLoaded) dict.load();
	}
};
