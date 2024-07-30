#include "App.h"
#include "MainFrame.h"
#include "SearchTree.h"
#include <wx/wx.h>

//let wxwidget knows which class represents our app

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	wxImage::AddHandler(new wxPNGHandler);
	MainFrame* frame = new MainFrame("wxNotebook Example");
	frame->Show(true);
	return true;

	return true;
}
