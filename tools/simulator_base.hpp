#pragma once

#include "UI.hpp"
#include "timer.hpp"

class Simulator_base {

    public:
        Simulator_base(std::string name);

        std::string name() const;

        void add_UI_item(UI_item* item);
        json get_UI_items() const;

        UI_item* get_UI_item(std::string id) const;

        void run();
        void stop();

        virtual void timer() {};
    protected:
        std::string p_name;
        std::vector<UI_item*> p_UI_items;
        std::map<std::string, UI_item*> p_UI_items_map;
        Timer p_timer;
};