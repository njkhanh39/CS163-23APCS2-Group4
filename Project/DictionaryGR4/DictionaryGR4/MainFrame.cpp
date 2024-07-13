#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>
using namespace std;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

	//controls
	
	panel = new wxPanel(this, 10001, wxDefaultPosition, wxSize(800, 300));
	panel->SetBackgroundColour(wxColor(100, 100, 200));

	button = new wxButton(panel, 10001, "Enter", wxPoint(40, 150), wxSize(70, 40));
	listBox = new wxListBox(panel, wxID_ANY, wxPoint(120, 200), wxSize(400, 400));
	textCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(120, 150), wxSize(220,40));

	textCtrl->Bind(wxEVT_TEXT, &MainFrame::OnTextWritten, this);


	
	CreateStatusBar();
}
MainFrame::~MainFrame() {

}

void MainFrame::OnMousePosition(wxMouseEvent& evt) {
	wxPoint m = evt.GetPosition();

	wxString message = wxString::Format("Mouse position within panel: (x=%d y=%d)", m.x, m.y);
	wxLogStatus(message);
}

void MainFrame::OnTextWritten(wxCommandEvent& evt) {
	wxString s = evt.GetString();
	
	string word = string(s);

	for (auto& c : word) c = tolower(c);

	bool yes = false;

	if ((int)word.length() == 1 && searchTree.empty()) {
		char key = word[0];
		if(searchTree.loadData(key)) yes = true;
		wxLogStatus("Loading data...");
	}
	else if ((int)word.length() == 0) {
	    if(!searchTree.empty()) searchTree.clear();
		wxLogStatus("Clearing Search Tree...");
	}
	else {
		wxLogStatus("Doing nothing...");
	}


	listBox->Clear();

	list<string> defs = searchTree.getStringDefinitions(word);
	for (auto& d : defs) listBox->AppendString(d);
	
	
}



