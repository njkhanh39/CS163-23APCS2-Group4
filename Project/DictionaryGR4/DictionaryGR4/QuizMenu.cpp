#include "MainFrame.h"
#include "QuizMenu.h"

QuizMenu::QuizMenu(wxWindow* parent, Dictionary*& dict) : wxSimplebook(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

	
	//SetBackgroundColour(black);

	/*GAME MODE*/
	displayGameMode(dict);
	start->Bind(wxEVT_BUTTON, [this,dict](wxCommandEvent& evt) {
		is_endless = false;
		this->SetSelection(0);
		processQuestion(dict);
		});
	endlessmode->Bind(wxEVT_BUTTON, [this,dict](wxCommandEvent& evt) {
		is_endless = true;
		this->SetSelection(0);
		});
	/**/

	/*QUESTION*/

	question = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);
	question->SetBackgroundColour(black);

	chosen_box = new wxStaticBox(question, wxID_ANY, "", boxPosition, boxSize, wxBORDER_NONE);
	chosen_box->SetBackgroundColour(white);

	chosen_quest = new wxStaticText(chosen_box, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);

	wxBitmap bitnext(wxT("IMG/nextdef.png"), wxBITMAP_TYPE_PNG);
	nextquest = new wxBitmapButton(question, wxID_ANY, bitnext, wxPoint(1218, 333), wxSize(53, 53), wxBORDER_NONE);

	for (int i = 0; i < 4; ++i)
	{
		options[i] = new wxButton(question, wxID_ANY, "", optPosition[i], optSize, wxBORDER_NONE);
		options[i]->SetBackgroundColour(purple);
	}

	modeQuestion(boxSize);

	/**/

	/*RESULT*/

	/**/

	this->AddPage(question, "Question", true);
	this->AddPage(gamevar, "Game variable", true);

	this->SetSelection(1);

}

void QuizMenu::displayGameMode(Dictionary* dict)
{
	gamevar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);
	gamevar->SetBackgroundColour(black);


	game = new wxStaticText(gamevar, wxID_ANY, "GAMETYPE", wxPoint(601, 66));
	datachose = new wxStaticText(gamevar, wxID_ANY, "DATASET", wxPoint(601, 211));
	numberques = new wxStaticText(gamevar, wxID_ANY, "NUMBER OF QUESTIONS", wxPoint(562, 347));

	game->SetForegroundColour(white);
	datachose->SetForegroundColour(white);
	numberques->SetForegroundColour(white);

	GuessWord = new wxButton(gamevar, wxID_ANY, "GUESS WORD", wxPoint(238, 110), wxSize(402, 65));
	GuessWord->SetBackgroundColour(green);
	GuessDef = new wxButton(gamevar, wxID_ANY, "GUESS DEFINITION", wxPoint(640, 110), wxSize(402, 65));

	EngEng = new wxButton(gamevar, wxID_ANY, "ENG-ENG", wxPoint(238, 249), wxSize(268, 65));
	EngEng->SetBackgroundColour(green);
	EngVie = new wxButton(gamevar, wxID_ANY, "ENG-VIE", wxPoint(506, 249), wxSize(268, 65));
	VieEng = new wxButton(gamevar, wxID_ANY, "VIE-ENG", wxPoint(774, 249), wxSize(268, 65));

	wxBitmap bitin(wxT("IMG/Plus.png"), wxBITMAP_TYPE_PNG);
	wxBitmap bitde(wxT("IMG/Minus.png"), wxBITMAP_TYPE_PNG);

	decrease = new wxBitmapButton(gamevar, wxID_ANY, bitde, wxPoint(451, 388), wxSize(48, 48), wxBORDER_NONE);
	increase = new wxBitmapButton(gamevar, wxID_ANY, bitin, wxPoint(791, 388), wxSize(48, 48), wxBORDER_NONE);
	increase->SetBackgroundColour(white);
	decrease->SetBackgroundColour(white);

	increase->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		numquest += 5;
		displaynum->SetLabel(wxString::Format("%d", numquest));
		});

	decrease->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		numquest -= 5;
		displaynum->SetLabel(wxString::Format("%d", numquest));
		});

	displaynum = new wxButton(gamevar, wxID_ANY, std::to_string(numquest), wxPoint(439, 380), wxSize(402, 65), wxBORDER_NONE);

	wxBitmap bitendless(wxT("IMG/endless.png"), wxBITMAP_TYPE_PNG);
	wxBitmap bitstart(wxT("IMG/start.png"), wxBITMAP_TYPE_PNG);

	start = new wxBitmapButton(gamevar, wxID_ANY, bitstart, wxPoint(238, 500), wxSize(296, 78), wxBORDER_NONE);
	endlessmode = new wxBitmapButton(gamevar, wxID_ANY, bitendless, wxPoint(640, 500), wxSize(296, 78), wxBORDER_NONE);
	start->SetBackgroundColour(black);
	endlessmode->SetBackgroundColour(black);

	dict->activeSearcher = &dict->toolEngEng;

	EngEng->Bind(wxEVT_BUTTON, [this,dict](wxCommandEvent& evt) {
		dataset = 0;
		dict->activeSearcher = &dict->toolEngEng;
		EngEng->SetBackgroundColour(wxColour(11, 199, 189));
		EngVie->SetBackgroundColour(wxColour(255, 255, 255));
		VieEng->SetBackgroundColour(wxColour(255, 255, 255));
		});

	EngVie->Bind(wxEVT_BUTTON, [this,dict](wxCommandEvent& evt) {
		dataset = 1;
		dict->activeSearcher = &dict->toolEngVie;
		EngVie->SetBackgroundColour(wxColour(11, 199, 189));
		EngEng->SetBackgroundColour(wxColour(255, 255, 255));
		VieEng->SetBackgroundColour(wxColour(255, 255, 255));
		});

	VieEng->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& evt) {
		dataset = 2;
		dict->activeSearcher = &dict->toolVieEng;
		VieEng->SetBackgroundColour(wxColour(11, 199, 189));
		EngVie->SetBackgroundColour(wxColour(255, 255, 255));
		EngEng->SetBackgroundColour(wxColour(255, 255, 255));
		});

	GuessWord->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		gametype = 0;
		GuessWord->SetBackgroundColour(wxColour(11, 199, 189));
		GuessDef->SetBackgroundColour(wxColour(255, 255, 255));
		boxSize = wxSize(1124, 292);
		optSize = wxSize(554, 130);
		optPosition[0] = wxPoint(78, 348);
		optPosition[1] = wxPoint(648, 348);
		optPosition[2] = wxPoint(78, 494);
		optPosition[3] = wxPoint(648, 494);
		modeQuestion(boxSize);
		});

	GuessDef->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		gametype = 1;
		GuessDef->SetBackgroundColour(wxColour(11, 199, 189));
		GuessWord->SetBackgroundColour(wxColour(255, 255, 255));
		boxSize = wxSize(1124, 118);
		optSize = wxSize(554, 190);
		optPosition[0] = wxPoint(78, 182);
		optPosition[1] = wxPoint(648, 182);
		optPosition[2] = wxPoint(78, 388);
		optPosition[3] = wxPoint(648, 388);
		modeQuestion(boxSize);
		});
}

void QuizMenu::modeQuestion(wxSize boxSize)
{
	chosen_box->SetSize(boxSize);
	
	for (int i = 0; i < 4; ++i)
	{
		options[i]->SetPosition(optPosition[i]);
		options[i]->SetSize(optSize);
	}
}

void QuizMenu::processQuestion(Dictionary* dict)
{
	if (current_question < numquest)
	{
		string quest; 
		vector<string> opt = GenQuest(dict, pos_correct, quest);
		for (int i = 0; i < 4; ++i)
			options[i]->Enable();

		chosen_quest->SetLabel(quest);
		chosen_quest->Wrap(1100);

		// Calculate center position within the static box
		int textWidth = chosen_quest->GetBestSize().GetWidth();
		int textHeight = chosen_quest->GetBestSize().GetHeight();
		int centerX = (boxSize.GetWidth() - textWidth) / 2;
		int centerY = (boxSize.GetHeight() - textHeight) / 2;

		// Position the wxStaticText manually to be centered
		chosen_quest->SetPosition(wxPoint(centerX, centerY));

		for (int i = 0; i < 4; ++i)
			options[i]->SetLabel(opt[i]);
	}
}

void QuizMenu::OnOptionSelected(wxCommandEvent& event, Dictionary* dict) {
	wxButton* selectedButton = dynamic_cast<wxButton*>(event.GetEventObject());
	int selectedIndex = -1;

	// Find the index of the selected button
	for (int i = 0; i < 4; ++i) {
		if (options[i] == selectedButton) {
			selectedIndex = i;
			break;
		}
	}

	if (selectedIndex != -1) {
		if (selectedIndex == pos_correct) {
			wxMessageBox("Correct!", "Result", wxOK | wxICON_INFORMATION);
		}
		else {
			wxMessageBox("Wrong answer!", "Result", wxOK | wxICON_ERROR);
		}

		current_question++;
		processQuestion(dict);
	}
}

/*void QuizMenu::displayQuestion(Dictionary* dict, string quest, vector<string> opt)
{
	for (int i = 0; i < 4; ++i)
		options[i]->Enable();

	chosen_quest->SetLabel(quest);
	chosen_quest->Wrap(1100);

	// Calculate center position within the static box
	int textWidth = chosen_quest->GetBestSize().GetWidth();
	int textHeight = chosen_quest->GetBestSize().GetHeight();
	int centerX = (boxSize.GetWidth() - textWidth) / 2;
	int centerY = (boxSize.GetHeight() - textHeight) / 2;

	// Position the wxStaticText manually to be centered
	chosen_quest->SetPosition(wxPoint(centerX, centerY));

	for (int i = 0; i < 4; ++i)
		options[i]->SetLabel(opt[i]);
	
	options[0]->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		if (pos_correct == 0) {
			options[0]->SetBackgroundColour(green);
			score++;
		}
		else
		{
			options[0]->SetBackgroundColour(red);
			options[pos_correct]->SetBackgroundColour(green);
		}
		for (int i = 0; i < 4; ++i)
			options[i]->Disable();
		});

	options[1]->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		if (pos_correct == 0)
		{
			options[1]->SetBackgroundColour(green);
			score++;
		}
		else
		{
			options[1]->SetBackgroundColour(red);
			options[pos_correct]->SetBackgroundColour(green);
		}
		for (int i = 0; i < 4; ++i)
			options[i]->Disable();
		});

	options[2]->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		if (pos_correct == 0)
		{
			options[2]->SetBackgroundColour(green);
			score++;
		}
		else
		{
			options[2]->SetBackgroundColour(red);
			options[pos_correct]->SetBackgroundColour(green);
		}
		for (int i = 0; i < 4; ++i)
			options[i]->Disable();
		});

	options[3]->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		if (pos_correct == 0)
		{
			options[3]->SetBackgroundColour(green);
			score++;
		}
		else
		{
			options[3]->SetBackgroundColour(red);
			options[pos_correct]->SetBackgroundColour(green);
		}
		for (int i = 0; i < 4; ++i)
			options[i]->Disable();
		});
}*/

void QuizMenu::DefaultSetting()
{
	current_question = 0; score = 0; numquest = 30, gametype = 0, dataset = 0;
}

string QuizMenu::GetWordType(Word word, string& def)
{
	string str = word.getDefinitionAt(0).getStringDefinition();
	string wordtype = "";

	int j = 0;
	if (str[j] == '(') {
		while (j < (int)str.length() && (j == 0 || str[j - 1] != ')')) {
			wordtype += str[j];
			++j;
		}
	}

	while (j < str.length()) { def += str[j]; ++j; }

	return wordtype;
}

Word QuizMenu::RandomWord(string& wordText, Dictionary * dict) {
	srand(time(NULL));

	string text;
	while (text.empty()) {
		int r = RandInt(0, 200000);
		text = dict->activeSearcher->getWord(r).getText();
	}

	return dict->activeSearcher->searchWord(text);
}

vector<string> QuizMenu::GenQuest( Dictionary* dict, int& correctans, string& quest)
{
	string wordText, ans_def;
	Word chosen_word = RandomWord(wordText,dict);
	string ans_type = GetWordType(chosen_word, ans_def);
	correctans = RandInt(0,3);

	vector <string> opt_list;
	opt_list.resize(4);

	if (gametype == 0)
	{
		quest = wordText;
		opt_list[correctans] = ans_def;
	}
	else
	{
		quest = ans_def;
		opt_list[correctans] = wordText;
	}

	int j = 0;
	while (j < 4)
	{
		if (j != correctans)
		{
			string tmp, tmp_type, tmp_def;
			Word tmp_word = RandomWord(tmp,dict);
			if (tmp_word != chosen_word)
			{
				tmp_type = GetWordType(tmp_word, tmp_def);
				if (gametype == 0) opt_list[j] = tmp_def;
				else opt_list[j] = tmp;
				++j;
			}
		}
		else ++j;
	}
	return opt_list;
}