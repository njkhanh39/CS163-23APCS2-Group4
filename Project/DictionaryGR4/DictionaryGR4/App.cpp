#include "App.h"
#include "MainFrame.h"
#include "SearchTree.h"
#include <wx/wx.h>

//let wxwidget knows which class represents our app

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	Dictionary* dict = &dictionary;
	
	wxImage::AddHandler(new wxPNGHandler);
	MainFrame* frame = new MainFrame(dict,"Dictionary", wxSize(1280,800));
	frame->Show(true);
	
	return true;
}
