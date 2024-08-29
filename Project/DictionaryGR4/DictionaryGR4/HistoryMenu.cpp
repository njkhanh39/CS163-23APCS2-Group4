#include "HistoryMenu.h"
#include <wx/listctrl.h>

using namespace std;
namespace fs = std::experimental::filesystem;

HistoryMenu::~HistoryMenu() {
}

HistoryMenu::HistoryMenu(wxWindow* parent, Dictionary*& dict)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

    auto purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);
   
    // Font settings
    wxFont font(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    font.SetPointSize(16);
    wxFont fontTitle(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_NORMAL);
    wxFont fontCB(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    fontTitle.MakeBold();

    SetBackgroundColour(black);

    // Initialize search bar
    searchBar = new wxTextCtrl(this, wxID_ANY, "", wxPoint(150, 50), wxSize(980, 60));
    searchBar->SetFont(fontCB);

    // Initialize word view
    wordView = new WordView(this, wxPoint(150, 150), wxSize(980, 460), dict);
    wordView->SetColor(white);
    wordView->getEditDefButton()->Hide();
    wordView->getDelDefButton()->Hide();
    wordView->getFavDefButton()->Hide();

    // Initialize buttons
    back = new wxButton(this, wxID_ANY, "BACK", wxPoint(45, 50), wxSize(100, 60), wxBORDER_NONE);
    back->SetFont(fontCB);
    back->SetBackgroundColour(purple);
    back->SetForegroundColour(white);

    clear = new wxButton(this, wxID_ANY, "CLEAR", wxPoint(1050, 600), wxSize(154, 60), wxBORDER_NONE);
    clear->SetFont(fontCB);
    clear->SetBackgroundColour(red);
    clear->SetForegroundColour(white);

    wxArrayString languages = { "Eng-Eng", "Eng-Vie", "Vie-Eng" };
    datasetCbb = new wxComboBox(this, wxID_ANY, "", wxPoint(250, 40), wxSize(154, -1), languages, wxCB_READONLY);
    datasetCbb->SetFont(fontCB);
    datasetCbb->SetSelection(0);
    datasetCbb->Refresh();

    wxString keyuni = datasetCbb->GetString(0);
    string key = (string)keyuni.mb_str(wxConvUTF8);
    ViewedDataset = key;

    datasetCbb->Bind(wxEVT_COMBOBOX, [this, dict](wxCommandEvent& evt) {
        OnDataSetChanged(evt, dict);
        });

    // Initialize wxListCtrl for history items
    list = new wxListCtrl(this, wxID_ANY, wxPoint(250, 120), wxSize(680, 550), wxLC_REPORT | wxLC_SINGLE_SEL);
    list->SetFont(font);
    list->InsertColumn(0, "No.", wxLIST_FORMAT_CENTER, 80);
    list->InsertColumn(1, "Date", wxLIST_FORMAT_LEFT, 170);
    list->InsertColumn(2, "Time", wxLIST_FORMAT_LEFT, 120);
    list->InsertColumn(3, "Word", wxLIST_FORMAT_LEFT, 300);

    // Populate list with history items
    if (!dict->hist.loadFromFile(ViewedDataset)) {
    }
    else {
    }

    auto searchList = dict->hist.searchList;
    if (searchList.empty()) {
    }
    else {
        int tmp = searchList.size();
        int index = 0;
        for (auto& c : searchList) {
            wxString no = wxString::Format("%d", index + 1);
            wxString date = c.getDate();
            wxString time = c.getTime();
            wxString word = c.getText();

            long itemIndex = list->InsertItem(index, no);
            list->SetItem(itemIndex, 1, date);
            list->SetItem(itemIndex, 2, time);
            list->SetItem(itemIndex, 3, word);
            index++;
        }
    }

    // Bind events
    //view->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
    //    OnViewButton(evt, dict);
    //    });

    list->Bind(wxEVT_LIST_ITEM_SELECTED, [this, dict](wxListEvent& evt) {
        OnViewButton(evt, dict);
        });

    back->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
        OnBackButton(evt, dict);
        });

    clear->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
        OnClearButton(evt, dict);
        });

    // Initial visibility
    back->Hide();
    searchBar->Hide();
    wordView->Hide();
}

void HistoryMenu::OnViewButton(wxCommandEvent& evt, Dictionary* dict) {
    int selected = list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selected == wxNOT_FOUND) return;

    wxString keyuni = list->GetItemText(selected, 3);  // Get the word (4th column)
    string key = (string)keyuni.mb_str(wxConvUTF8);

    datasetCbb->Hide();
    list->Hide();
    clear->Hide();

    back->Show();
    searchBar->Show();
    wordView->Show();

    

    searchBar->SetValue(keyuni);
    Word word = dict->searchWordMatching(key);
    if (word.empty()) {
        wordView->clearWordView();
        searchBar->SetValue(keyuni + "(word was deleted!)");
    }
    else {
        //string tmp = dict->getActiveDataset();
        dict->chooseLanguage(ViewedDataset);
        wordView->processWord(word, dict);
        Word* ptr = dict->getWordPtr(key);
        wordView->setWord(ptr);
        wordView->setActiveDataset(ViewedDataset);
        //dict->chooseLanguage(tmp);
    }
   
}

void HistoryMenu::OnBackButton(wxCommandEvent& evt, Dictionary* dict) {
    back->Hide();
    searchBar->Hide();
    wordView->Hide();

    datasetCbb->Show();
    list->Show();
    clear->Show();
}

void HistoryMenu::Refresh(Dictionary* dict) {
    // Clear the current list
    list->DeleteAllItems();

    // Load history from the new dataset
    if (!dict->hist.loadFromFile(dict->getActiveDataset())) {
        return;
    }
    else {
    }

    auto searchList = dict->hist.searchList;
    if (searchList.empty()) {
    }
    else {
        int tmp = searchList.size();
        int index = 0;
        for (auto& c : searchList) {
            wxString no = wxString::Format("%d", index + 1);
            wxString date = c.getDate();
            wxString time = c.getTime();
            wxString word = c.getText();

            wxString wxword = wxString::FromUTF8(word);

            long itemIndex = list->InsertItem(index, no);
            list->SetItem(itemIndex, 1, date);
            list->SetItem(itemIndex, 2, time);
            list->SetItem(itemIndex, 3, wxword);
            index++;
        }
    }
}

void HistoryMenu::OnDataSetChanged(wxCommandEvent& evt, Dictionary* dict) {
    int selection = datasetCbb->GetSelection();
    wxString keyuni = datasetCbb->GetString(selection);
    string key = (string)keyuni.mb_str(wxConvUTF8);
    ViewedDataset = key;
    list->DeleteAllItems();
    if (!dict->hist.loadFromFile(ViewedDataset)) {
    }
    else {
    }

    auto searchList = dict->hist.searchList;
    if (searchList.empty()) {
    }
    else {
        int tmp = searchList.size();
        int index = 0;
        for (auto& c : searchList) {
            wxString no = wxString::Format("%d", index + 1);
            wxString date = c.getDate();
            wxString time = c.getTime();
            string word = c.getText();

            wxString wxword = wxString::FromUTF8(word);

            long itemIndex = list->InsertItem(index, no);
            list->SetItem(itemIndex, 1, date);
            list->SetItem(itemIndex, 2, time);
            list->SetItem(itemIndex, 3, wxword);
            index++;
        }
    }
}

void HistoryMenu::OnClearButton(wxCommandEvent& evt, Dictionary* dict) {
    if (dict->hist.clearHistory(ViewedDataset)) {
        wxLogMessage("History deleted successfully!");
        list->DeleteAllItems();
        return;
    }

    wxLogMessage("Error");
}