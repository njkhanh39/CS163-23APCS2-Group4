#include "MainFrame.h"

MainFrame::MainFrame(Dictionary*& dict, const wxString& Title, wxSize FrameSize) : wxFrame(NULL, wxID_ANY, Title, wxDefaultPosition, FrameSize) {
	simpleBook = new wxSimplebook(this, wxID_ANY);
    

    NavPane = new wxPanel(this, 10001, wxDefaultPosition, wxSize(1280, 80), wxBORDER_NONE);
    NavPane->SetBackgroundColour(purple);

    searchpane = new wxButton(NavPane, wxID_ANY, "SEARCH", wxPoint(344, 18), wxSize(147, 44), wxBORDER_NONE);
    searchpane->SetBackgroundColour(black);
    searchpane->SetForegroundColour(white);

    addpane = new wxButton(NavPane, wxID_ANY, "ADD TO DATASET", wxPoint(524, 18), wxSize(147, 44), wxBORDER_NONE);
    addpane->SetBackgroundColour(purple);
    addpane->SetForegroundColour(white);

    quizpane = new wxButton(NavPane, wxID_ANY, "QUIZ GAME", wxPoint(704, 18), wxSize(147, 44), wxBORDER_NONE);
    quizpane->SetBackgroundColour(purple);
    quizpane->SetForegroundColour(white);

    hispane = new wxButton(NavPane, wxID_ANY, "HISTORY", wxPoint(884, 18), wxSize(147, 44), wxBORDER_NONE);
    hispane->SetBackgroundColour(purple);
    hispane->SetForegroundColour(white);

    favpane = new wxButton(NavPane, wxID_ANY, "FAVOURITE", wxPoint(1064, 18), wxSize(147, 44), wxBORDER_NONE);
    favpane->SetBackgroundColour(purple);
    favpane->SetForegroundColour(white);


	
    //MainMenu* mainMenu = new MainMenu(simpleBook);
	SearchMenu* searchMenu = new SearchMenu(simpleBook, dict);
    QuizMenu* quizMenu = new QuizMenu(simpleBook, dict);
    AddWordMenu* addMenu = new AddWordMenu(simpleBook, dict);
    HistoryMenu* histMenu = new HistoryMenu(simpleBook, dict);
 
    simpleBook->AddPage(addMenu, "Add Menu", true); //selection = 0
    simpleBook->AddPage(quizMenu, "Quiz Menu", true); //selection = 1
    simpleBook->AddPage(searchMenu, "Search Menu", true);//selection = 2
	//simpleBook->AddPage(mainMenu, "Main Menu", true); //selection = 2
    simpleBook->AddPage(histMenu, "History Menu", true);

    simpleBook->SetSelection(2);
   

    //binding


    searchpane->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        simpleBook->SetSelection(2); 
        searchpane->SetBackgroundColour(black);
        addpane->SetBackgroundColour(purple);
        hispane->SetBackgroundColour(purple);
        quizpane->SetBackgroundColour(purple);
        favpane->SetBackgroundColour(purple);
        });

    quizpane->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        simpleBook->SetSelection(1); 
        searchpane->SetBackgroundColour(purple);
        addpane->SetBackgroundColour(purple);
        hispane->SetBackgroundColour(purple);
        quizpane->SetBackgroundColour(black);
        favpane->SetBackgroundColour(purple);
        });

    addpane->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        simpleBook->SetSelection(0);
        searchpane->SetBackgroundColour(purple);
        addpane->SetBackgroundColour(black);
        hispane->SetBackgroundColour(purple);
        quizpane->SetBackgroundColour(purple);
        favpane->SetBackgroundColour(purple);
        });

    favpane->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        simpleBook->SetSelection(3);
        searchpane->SetBackgroundColour(purple);
        addpane->SetBackgroundColour(black);
        hispane->SetBackgroundColour(purple);
        quizpane->SetBackgroundColour(purple);
        favpane->SetBackgroundColour(purple);
        });

    hispane->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
        simpleBook->SetSelection(3);
        searchpane->SetBackgroundColour(purple);
        addpane->SetBackgroundColour(black);
        hispane->SetBackgroundColour(purple);
        quizpane->SetBackgroundColour(purple);
        favpane->SetBackgroundColour(purple);
        });

    //searchMenu->back_to_home->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {simpleBook->SetSelection(0); });
    //addWordMenu->back_to_home->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {simpleBook->SetSelection(0); });

    //sizing

    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(NavPane, 1, wxEXPAND | wxLEFT | wxUP | wxRIGHT, 0);
    frameSizer->Add(simpleBook, 5, wxEXPAND | wxALL, 0);

    SetSizerAndFit(frameSizer);
    
}

void MainFrame::OnPreviousPage(wxCommandEvent& event)
{
    int selection = simpleBook->GetSelection();
    if (selection > 0)
    {
       simpleBook->SetSelection(selection - 1);
    }
}

void MainFrame::OnNextPage(wxCommandEvent& event)
{
    int selection = simpleBook->GetSelection();
    if (selection < simpleBook->GetPageCount() - 1)
    {
        simpleBook->SetSelection(selection + 1);
    }
}
