#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "networkerror.h"
#include "simulator_base.h"
#include "web_socket_wrapper.h"
#include <QMainWindow>
#include <map>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QString host, uint16_t port, QWidget *parent = nullptr);
    ~MainWindow();

    void defaultSim(QString name);
    void pase_sim_names(json &sims);
    void open_sim_window(void);
    void check_active_sim(QString name);
    void open_sim(QString sim_name);
    void setup_cb(void);

  signals:
    void quit(void);

  private:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

    void on_cmd_cb(json &j);

    bool m_default_sim = false;
    QString m_selected_sim_name;
    QMainWindow *m_selected_sim;
    Ui::MainWindow *ui;
    Web_socket_wrapper *m_web_socket;
    NetworkError *m_error_dialog;

    std::map<QString, Simulator_base *> m_sims;
};

#endif // MAINWINDOW_H
