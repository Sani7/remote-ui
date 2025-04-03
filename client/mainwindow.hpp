#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "networkerror.hpp"
#include "simulator_base.hpp"
#include "web_socket_wrapper.hpp"
#include <QMainWindow>
#include <map>

#define INSERT_SIMULATOR(type)                                                                                         \
    m_sims.insert(std::make_pair(type().sim_name(), std::make_unique<type>(m_web_socket.get(), this)))

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QUrl ws_url, QWidget *parent = nullptr);
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
    std::unique_ptr<Web_socket_wrapper> m_web_socket;
    std::unique_ptr<NetworkError> m_error_dialog;

    std::map<QString, std::unique_ptr<Simulator_base>> m_sims;
};

#endif // MAINWINDOW_H
