#ifndef SELECTBUTTON_HPP
#define SELECTBUTTON_HPP

#include "Button.hpp"

class SelectButton : public Button {
    private:
        bool selected;
        bool **selectGroup{ nullptr };

        void determineBackColor() override;
        void onClick() override;

    public:
        SelectButton(SDL_Renderer *renderer, const std::string &text = "");
        ~SelectButton() override;

        void setSelectGroup(bool **selectGroup);

        void select();
        void deselect();
        bool isSelected() const;
};

inline bool SelectButton::isSelected() const {
    return selected;
}

#endif