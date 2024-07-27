#pragma once
#include <wx/wx.h>
#include "Dictionary.h"

class MainFrame : public wxFrame {
public:
	MainFrame(const wxString& title);
	~MainFrame();
private:
	Dictionary dict;
	History his;
	wxPanel* panel;
	wxButton* button;
	wxListBox* listBox, *defView, *wordView;
	wxTextCtrl* textCtrl;
	wxButton* historyButton;
	wxListBox* HistoryList;

	void OnMousePosition(wxMouseEvent& evt);
	void OnViewWord(wxCommandEvent& evt);
	void OnTextWritten(wxCommandEvent& evt);
	void OnHistoryButtonClicked(wxCommandEvent& evt);
	void ShowHistory();
};
