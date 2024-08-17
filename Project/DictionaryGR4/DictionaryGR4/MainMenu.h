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
    wxButton* to_Search_Word, * to_Add_to_Dataset;
    wxButton* next_Search_Word;
    wxButton* tmp_Quiz;
    MainMenu(wxWindow* parent);

private:   
    wxPanel* titleBar, * mainPanel;

};
