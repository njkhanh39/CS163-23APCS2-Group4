#include "MainFrame.h"
#include "QuizMenu.h"

QuizMenu::QuizMenu(wxWindow* parent, Dictionary*& dict) : wxSimplebook(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

	
	//SetBackgroundColour(black);

	/*GAME MODE*/
	gamevar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);
	gamevar->SetBackgroundColour(black);


	game = new wxStaticText(gamevar, wxID_ANY, "GAMETYPE", wxPoint(601, 66));
	datachose = new wxStaticText(gamevar, wxID_ANY, "DATASET", wxPoint(601, 211));
	numberques = new wxStaticText(gamevar, wxID_ANY, "NUMBER OF QUESTIONS", wxPoint(562, 347));

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

	displaynum = new wxButton(gamevar, wxID_ANY, std::to_string(numquest), wxPoint(439, 380), wxSize(402, 65), wxBORDER_NONE);

	wxBitmap bitendless(wxT("IMG/endless.png"), wxBITMAP_TYPE_PNG);
	wxBitmap bitstart(wxT("IMG/start.png"), wxBITMAP_TYPE_PNG);

	start = new wxBitmapButton(gamevar, wxID_ANY, bitstart, wxPoint(238, 500), wxSize(296, 78), wxBORDER_NONE);
	endlessmode = new wxBitmapButton(gamevar, wxID_ANY, bitendless, wxPoint(640, 500), wxSize(296, 78), wxBORDER_NONE);

	displayGameMode(dict);
	start->Bind(wxEVT_BUTTON, [this,dict](wxCommandEvent& evt) {
		is_endless = false;
		this->SetSelection(1);
		processQuestion(dict);
		});
	endlessmode->Bind(wxEVT_BUTTON, [this,dict](wxCommandEvent& evt) {
		is_endless = true;
		this->SetSelection(1);
		processQuestion(dict);
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
	nextquest->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& event) {
		OnNextQuestion(dict);
		});

	for (int i = 0; i < 4; ++i)
	{
		options[i] = new wxButton(question, 1000+i, "", optPosition[i], optSize, wxBORDER_NONE);
		options[i]->SetBackgroundColour(purple);
		options[i]->Bind(wxEVT_BUTTON, &QuizMenu::OnOptionSelected, this);
	}

	exit = new wxButton(question, wxID_ANY, "END QUIZ", wxPoint(906, 640), wxSize(296, 51), wxBORDER_NONE);
	exit->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
		this->SetSelection(0);
		displayscore();
		});

	modeQuestion(boxSize);

	/**/

	/*RESULT*/

	score_announce = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);
	score_announce->SetBackgroundColour(black);
	announce = new wxStaticText(score_announce,wxID_ANY, "", wxPoint(51,244), wxDefaultSize);
	restart = new wxButton(score_announce, wxID_ANY, "RESTART", wxPoint(344,297), wxSize(240,78), wxBORDER_NONE);
	backmenu = new wxButton(score_announce, wxID_ANY, "MENU", wxPoint(696,297), wxSize(240, 78), wxBORDER_NONE);

	restart->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& event) {
		current_question = 0; score = 0;
		this->SetSelection(1);
		processQuestion(dict);
		});

	backmenu->Bind(wxEVT_BUTTON, [this, dict](wxCommandEvent& event) {
		DefaultSetting(dict);
		this->SetSelection(2);
		});

	/**/

	this->AddPage(score_announce, "Result", true);
	this->AddPage(question, "Question", true);
	this->AddPage(gamevar, "Game variable", true);

	this->SetSelection(2);

}

void QuizMenu::displayscore()
{
	announce->SetLabel("Your score: " + std::to_string(score) + "/" + std::to_string(current_question));
}

void QuizMenu::displayGameMode(Dictionary* dict)
{
	game->SetForegroundColour(white);
	datachose->SetForegroundColour(white);
	numberques->SetForegroundColour(white);

	displaynum->SetLabel(wxString::Format("%d", numquest));

	increase->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		numquest += 5;
		displaynum->SetLabel(wxString::Format("%d", numquest));
		});

	decrease->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		if (numquest > 5) numquest -= 5;
		displaynum->SetLabel(wxString::Format("%d", numquest));
		});

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
	if (current_question < numquest || (is_endless))
	{
		string quest;
		vector<string> opt = GenQuest(dict, pos_correct, quest);
		for (int i = 0; i < 4; ++i)
		{
			options[i]->Enable();
			options[i]->SetBackgroundColour(purple);
		}

		chosen_quest->SetLabel(wxString::FromUTF8(quest));
		chosen_quest->Wrap(1100);

		// Calculate center position within the static box
		int textWidth = chosen_quest->GetBestSize().GetWidth();
		int textHeight = chosen_quest->GetBestSize().GetHeight();
		int centerX = (boxSize.GetWidth() - textWidth) / 2;
		int centerY = (boxSize.GetHeight() - textHeight) / 2;

		// Position the wxStaticText manually to be centered
		chosen_quest->SetPosition(wxPoint(centerX, centerY));

		for (int i = 0; i < 4; ++i)
			options[i]->SetLabel(wxString::FromUTF8(opt[i]));
	}

	else
	{
		displayscore();
		this->SetSelection(0);
	}
}

void QuizMenu::OnOptionSelected(wxCommandEvent& event) {
	int selected = event.GetId() - 1000;

	if (selected != -1) {
		if (selected == pos_correct) {
			options[selected]->SetBackgroundColour(green);
			score++;
		}
		else {
			options[selected]->SetBackgroundColour(red);
			options[pos_correct]->SetBackgroundColour(green);
		}

		for (int i = 0; i < 4; ++i)
			options[i]->Disable();
	}
}

void QuizMenu::OnNextQuestion(Dictionary* dict)
{
	++current_question;
	processQuestion(dict);
}

void QuizMenu::DefaultSetting(Dictionary* dict)
{
	current_question = 0; score = 0; numquest = 30, gametype = 0, dataset = 0;
	displayGameMode(dict);
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
	wordText = text;

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

	if (gametype == 1)
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
				if (gametype == 1) opt_list[j] = tmp_def;
				else opt_list[j] = tmp;
				++j;
			}
		}
		else ++j;
	}
	return opt_list;
}