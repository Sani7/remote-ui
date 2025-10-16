## Client UI

To create a new simulator ui follow these steps  
1. Make sure the client folder is folded down  
2. Right click on unisim_client and select Add new  
3. Choose Qt in files and classes and then select Qt widgets designer form class + Choose  
4. Select Mainwindow + Next  
5. Change the classname to the name of your sim use underscores as spaces, don't forget classnames always start with a capital letter  
6. Select the path by clicking browse, and make sure it is in the client/simulators folder + Next  
7. Set add to project to None + Finished  
8. Refresh CMake by right clicking on the project name + Run Cmake, it should show your sim as ui_ + filename  
9. Change the hpp file as following. This is an example out of test sim  

OPTIONAL Add id_to_label is optional if you want to use value labels
```cpp
#pragma once
#include "simulator_base.hpp"

namespace Ui
{
class Test_Sim;
}

class Test_Sim : public Simulator_base
{
    Q_OBJECT

  public:
    explicit Test_Sim(Web_socket_wrapper *web_socket = nullptr, QWidget *parent = nullptr);
    ~Test_Sim();
    // Optional
    QLabel *id_to_label(QWidget* widget) override;

  private:
    Ui::Test_Sim *ui;
};
```
10. Change the cpp file as following. This is an example out of test sim. PUSH_UI_ITEM is the method to add new QWidgets to the similator UI  
OPTIONAL Add id_to_label is optional if you want to use value labels switch case is also an option instead of if 
```cpp
#include "test.hpp"
#include "ui_test.h"

EXPORT_UI(Test_Sim);

Test_Sim::Test_Sim(Web_socket_wrapper *web_socket, QWidget *parent)
    : Simulator_base("Test Sim", web_socket, parent), ui(new Ui::Test_Sim)
{
    ui->setupUi(this);

    PUSH_UI_ITEM(item);
}

Test_Sim::~Test_Sim()
{
    delete ui;
}

QLabel *Test_Sim::id_to_label(QWidget* widget)
{
    if (widget == ui->slider)
        return ui->slider_label;
    if (widget == ui->thermo)
        return ui->thermo_label;
    if (widget == ui->dial)
        return ui->dial_label;

    return nullptr;
}
```
For extra examples look at the existing simulators