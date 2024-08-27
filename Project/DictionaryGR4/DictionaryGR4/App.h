#pragma once
#include <wx/wx.h>
#include "MainFrame.h"


class App : public wxApp {
public:
	Dictionary dictionary;
	App() {

	}
	~App() {

	}
	//entry point
	virtual bool OnInit();
};