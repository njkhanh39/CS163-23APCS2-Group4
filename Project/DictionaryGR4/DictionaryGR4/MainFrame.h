#pragma once
#include <wx/wx.h>
#include "Dictionary.h"
#include "GUI_WordView.h"

class MainFrame : public wxFrame {
public:
	MainFrame(const wxString& title);
	~MainFrame();
private:
	Dictionary dict;
	wxPanel* panel, *panel2;
	wxButton* button, *runTool, *unrunTool;
	wxListBox* suggestBar;
	wxTextCtrl* searchBar;
	WordView* wordView;
	

	void OnMousePosition(wxMouseEvent& evt);	

	//suggest bars
	void adjustSuggestBar(int maxHeight, int maxItem);

	//word view
	void OnViewWord(wxCommandEvent& evt);

	//searchBar
	void OnSearchAndSuggestHandler(wxCommandEvent& evt);
	
	//microscope button

	void OnSearchButton(wxCommandEvent& evt);

	void OnLoadTool(wxCommandEvent& evt) {
		if (!dict.isSearchingDefinition) dict.runSearchDefinitionEngine();
	}

	void OnUnLoadTool(wxCommandEvent& evt) {
		dict.turnOffSearchDefinitionEngine();
	}

};
