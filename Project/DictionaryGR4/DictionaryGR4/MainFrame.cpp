#include "MainFrame.h"

MainFrame::MainFrame(const wxString& Title, wxSize FrameSize) : wxFrame(NULL, wxID_ANY, Title, wxDefaultPosition, FrameSize) {
	simpleBook = new wxSimplebook(this, wxID_ANY);
	
	
    MainMenu* mainMenu = new MainMenu(simpleBook);
	SearchMenu* searchMenu = new SearchMenu(simpleBook);
    
    simpleBook->AddPage(searchMenu, "Search Menu", true);//selection = 0
	simpleBook->AddPage(mainMenu, "Main Menu", true); //selection = 1
	


    

    //binding


    mainMenu->next_Search_Word->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {simpleBook->SetSelection(0); });


    searchMenu->back_to_home->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {simpleBook->SetSelection(1); });
    //sizing

    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(simpleBook, 1, wxEXPAND | wxALL, 1);

   

    SetSizer(frameSizer);
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