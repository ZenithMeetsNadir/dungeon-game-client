#include <component/SelectButton.hpp>

SelectButton::SelectButton(SDL_Renderer *renderer, const std::string &text)
    : Button(renderer, text)
{ }

SelectButton::~SelectButton() {
    if (selectGroup)
        *selectGroup = nullptr;
}

void SelectButton::determineBackColor() {
    backColor = selected ? pressedColor : (hovered ? hoverColor : idleColor);
}

void SelectButton::onClick() {
    selected ? deselect() : select();
}

void SelectButton::setSelectGroup(bool **selectGroup) {
    if (this->selectGroup && *(this->selectGroup) == &selected)
        *this->selectGroup = nullptr;

    this->selectGroup = selectGroup;
}

void SelectButton::select() {
    selected = true;
    if (selectGroup) {
        if (*selectGroup)
            **selectGroup = false; // deselect the previous selection

        *selectGroup = &selected;
    }
}

void SelectButton::deselect() {
    selected = false;
    if (selectGroup)
        *selectGroup = nullptr;
}