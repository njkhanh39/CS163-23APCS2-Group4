#pragma once

#include <wx/wx.h>
#include <wx/simplebook.h>
#include <wx/spinctrl.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>

#include "Dictionary.h"
#include "GUI_WordView.h"

class MainMenu : public wxWindow {
public:
    wxButton* next_Search_Word;
    MainMenu(wxWindow* parent);

private:   
    wxPanel* titleBar, * mainPanel;

};
