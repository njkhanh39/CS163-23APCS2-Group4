#include "App.h"
#include "MainFrame.h"
#include "SearchTree.h"
#include <wx/wx.h>

//let wxwidget knows which class represents our app

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	wxImage::AddHandler(new wxPNGHandler);
	m_frame1 = new MainFrame("Hello");
	m_frame1->SetClientSize(800, 600);
	m_frame1->Center();
	m_frame1->Show();

	return true;
}
