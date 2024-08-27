#include "FavouriteMenu.h"

using namespace std;
namespace fs = std::experimental::filesystem;

FavouriteMenu::~FavouriteMenu() {
}

FavouriteMenu::FavouriteMenu(wxWindow* parent, Dictionary*& dict)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1280, 720), wxBORDER_NONE) {

}