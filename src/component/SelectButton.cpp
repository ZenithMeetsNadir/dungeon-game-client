#include <component/SelectButton.hpp>

SelectButton::SelectButton(Context *context)
    : Button(context)
{ }

SelectButton::~SelectButton() {
    setSelectGroup(nullptr);
}

void SelectButton::determineColor() {
    backColor = selected ? pressedBackColor : (hovered ? hoverBackColor : idleColor);
}

void SelectButton::onClick() {
    Button::onClick();
    selected ? deselect() : select();
}

void SelectButton::setSelectGroup(bool **selectGroup) {
    if (this->selectGroup && *this->selectGroup == &selected)
        *this->selectGroup = nullptr;

    this->selectGroup = selectGroup;
}

void SelectButton::clearState() {
    Button::clearState();
    deselect();
}

void SelectButton::clearVolatileState() {
    Button::clearVolatileState();
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
    if (selectGroup && *selectGroup == &selected)
        *selectGroup = nullptr;
}