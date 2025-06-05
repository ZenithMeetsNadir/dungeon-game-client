#ifndef SELECTBUTTON_HPP
#define SELECTBUTTON_HPP

#include "Button.hpp"

class SelectButton : public Button {
    protected:
        bool selected{ false };
        bool **selectGroup{ nullptr };

        void determineColor() override;
        void onClick() override;

    public:
        SelectButton(Context *context);
        ~SelectButton() override;

        void setSelectGroup(bool **selectGroup);

        void clearState() override;
        void clearVolatileState() override;

        void select();
        void deselect();
        bool isSelected() const;
};

inline bool SelectButton::isSelected() const {
    return selected;
}

#endif