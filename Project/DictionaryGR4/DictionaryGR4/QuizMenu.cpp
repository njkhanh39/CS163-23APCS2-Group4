#include "MainFrame.h"
#include "QuizMenu.h"

QuizMenu::QuizMenu(wxWindow* parent) : wxSimplebook(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

	auto purple = wxColour(101, 86, 142), red = wxColour(184, 89, 89), green = wxColour(11, 199, 189), white = wxColour(255, 255, 255), black = wxColour(34, 36, 40);
	//SetBackgroundColour(black);

	/*GAME MODE*/
	gamevar = new wxPanel(this, wxID_ANY,wxDefaultPosition,wxSize(1280,720),wxBORDER_NONE);
	gamevar->SetBackgroundColour(black);


	game = new wxStaticText(gamevar, wxID_ANY,"GAMETYPE", wxPoint(601,66));
	datachose = new wxStaticText(gamevar, wxID_ANY, "DATASET", wxPoint(601, 211));
	numberques = new wxStaticText(gamevar, wxID_ANY, "NUMBER OF QUESTIONS", wxPoint(562, 347)) ;

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

	decrease = new wxBitmapButton(gamevar, wxID_ANY, bitde, wxPoint(451, 388), wxSize(48,48), wxBORDER_NONE);
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

	displaynum = new wxButton(gamevar, wxID_ANY, std::to_string(numquest),wxPoint(439,380),wxSize(402,65),wxBORDER_NONE);

	EngEng->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		dataset = 0;
		EngEng->SetBackgroundColour(wxColour(11, 199, 189));
		EngVie->SetBackgroundColour(wxColour(255, 255, 255));
		VieEng->SetBackgroundColour(wxColour(255, 255, 255));
		});

	EngVie->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		dataset = 1;
		EngVie->SetBackgroundColour(wxColour(11, 199, 189));
		EngEng->SetBackgroundColour(wxColour(255, 255, 255));
		VieEng->SetBackgroundColour(wxColour(255, 255, 255));
		});

	VieEng->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		dataset = 2;
		VieEng->SetBackgroundColour(wxColour(11, 199, 189));
		EngVie->SetBackgroundColour(wxColour(255, 255, 255));
		EngEng->SetBackgroundColour(wxColour(255, 255, 255));
		});

	GuessWord->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		gametype = 0;
		GuessWord->SetBackgroundColour(wxColour(11, 199, 189));
		GuessDef->SetBackgroundColour(wxColour(255, 255, 255));
		});

	GuessDef->Bind(wxEVT_BUTTON, [this](wxCommandEvent& evt) {
		gametype = 1;
		GuessDef->SetBackgroundColour(wxColour(11, 199, 189));
		GuessWord->SetBackgroundColour(wxColour(255, 255, 255));
		});

	/**/

	/*QUESTION*/
	question = new wxPanel(this,wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE);
	question->SetBackgroundColour(black);

	wxPoint boxPosition(78, 80);
	wxSize boxSize(1124, 290);

	chosen_box = new wxStaticBox(question, wxID_ANY, "", boxPosition, boxSize, wxBORDER_NONE);
	chosen_box->SetBackgroundColour(white);

	chosen_quest = new wxStaticText(chosen_box,wxID_ANY,"Example");

	// Calculate center position within the static box
	int textWidth = chosen_quest->GetBestSize().GetWidth();
	int textHeight = chosen_quest->GetBestSize().GetHeight();
	int centerX = (boxSize.GetWidth() - textWidth) / 2;
	int centerY = (boxSize.GetHeight() - textHeight) / 2;

	// Position the wxStaticText manually to be centered
	chosen_quest->SetPosition(wxPoint(centerX, centerY));

	/**/

	/*RESULT*/

	/**/


	this->AddPage(question, "Question", true);
	this->AddPage(gamevar,"Game variable", true);

	this->SetSelection(1);

}

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

	while (j < str.length())
		def += str[j];

	return wordtype;
}

std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
int RandInt(int l, int r)
{
	return l + rng() % (r - l + 1);
}

void QuizMenu::GenQuest(wxCommandEvent& evt, Dictionary* dict)
{
	string wordText;
	Word chosen_word = dict->getRandomWord(wordText);
	int ans_locate = RandInt(0, 3);

	vector <string> opt_list;
	opt_list.resize(4);

	if (gametype == 0)
	{
		int j = 0;
		while (j < 4)
		{
			if (j != ans_locate)
			{
				string tmp, tmp_type, tmp_def;
				Word tmp_word = dict->getRandomWord(tmp);
				tmp_type = GetWordType(tmp_word, tmp_def);
				opt_list[j] = tmp_def;
			}
			else
			{
				string tmp_def, tmp_type;
				tmp_type = GetWordType(chosen_word, tmp_def);
				opt_list[j] = tmp_def;
			}
		}
	}

}