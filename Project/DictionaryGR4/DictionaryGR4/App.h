#pragma once
#include <wx/wx.h>
#include "MainFrame.h"
class App : public wxApp {
public:
	App() {

	}
	~App() {

	}
private:
	MainFrame* m_frame1 = nullptr;

public:
	//entry point
	virtual bool OnInit();
};


