## Server simulator
To create a simulator server follow these steps
1) Make sure the server folder is folded down
2) Right click on unisim_server and select Add new
3) Choose C/C++ in files and classes and then select C++ class + Choose
4) Change the classname to the name of your sim use underscores as spaces, don't forget classnames always start with a capital letter
5) Set as base Simulator_base and check "Add Q_Object"
6) Select the path by clicking browse and make sure it is in the server/simulators folder + Next
7) Set add to project to None + Finish
8) Refresh CMake by right clicking on the project name + Run Cmake, it should show your sim as sim_ + filename
9) Change the hpp file as following. This is an example out of test sim
```cpp
#pragma once
#include "simulator_base.hpp"

class Test_Sim : public Simulator_base
{
    Q_OBJECT
  public:
    explicit Test_Sim(Communication *comms = nullptr, QObject *parent = nullptr);
    // Optional
    void timer() override;
    void run_at_startup() override;
    void run_at_stop() override;

  private:
    // Put you're UI items and external connection classes here
    //UI_button m_button;
    //Visa m_device;
    //Output_card m_output;
    //GPIO m_gpio_pin0;
};
```
10) Change the cpp file as following. This is an example out of test sim. PUSH_UI_ITEM is the method to add new UI_items to the similator
```cpp
#include "test.hpp"

EXPORT_SIM(Test_Sim);

Test_Sim::Test_Sim(Communication *com, QObject *parent)
    : Simulator_base("Test Sim", com, std::chrono::milliseconds(1000), parent),
    // Put you're variable constructors here
      m_button("Off", Color::White, Color::Red)
{
    PUSH_UI_ITEM(m_ui_item);

    // Put you're connect statements here
    connect(&m_button, &UI_button::clicked, this, [=, this] {   
        m_button.set_text("On");
    });
}

void Test_Sim::timer()
{
    SPDLOG_INFO("Timer triggered");
}

void Test_Sim::run_at_startup()
{
    // Run at startup
    SPDLOG_INFO("Test_Sim started");
}
void Test_Sim::run_at_stop()
{
    // Run at startup
    SPDLOG_INFO("Test_Sim stoped");
}
```

For extra examples look at the existing simulators