#pragma once
#include <wx/wx.h>
#include "SearchTree.h"

class MainFrame : public wxFrame {
public:
	MainFrame(const wxString& title);
	~MainFrame();
private:
	Trie searchTree;

	wxPanel* panel;
	wxButton* button;
	wxListBox* listBox;
	wxTextCtrl* textCtrl;

	void OnMousePosition(wxMouseEvent& evt);	
	void OnTextWritten(wxCommandEvent& evt);
};
