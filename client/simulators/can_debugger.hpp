#pragma once
#include "ui_base.hpp"

namespace Ui
{
class Can_Debugger;
}

class Can_Debugger : public UI_base
{
    Q_OBJECT

  public:
    explicit Can_Debugger(Web_socket_wrapper *web_socket = nullptr, QWidget *parent = nullptr);
    ~Can_Debugger();
    static QString name()
    {
        return "Can Debugger";
    };

  private:
    Ui::Can_Debugger *ui;
};
