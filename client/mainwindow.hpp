#pragma once
#include "simulator_base.hpp"
#include "web_socket_wrapper.hpp"
#include <QMainWindow>
#include <map>

#define INSERT_SIMULATOR(type)                                                                                         \
    m_sims.insert(std::make_pair(type().sim_name(), std::make_unique<type>(m_web_socket.get(), this)))

Q_FORWARD_DECLARE_OBJC_CLASS(QMessageBox);

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QUrl ws_url, QString sim, QWidget *parent = nullptr);
    ~MainWindow();

    void default_sim(QString name);
    void pase_sim_names(json &sims);
    void open_sim_window(void);
    void check_active_sim(QString name);
    void open_sim(QString sim_name);
    void setup_cb(void);

  private:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

    void on_cmd_cb(json &j);

    bool m_default_sim = false;
    QString m_selected_sim_name;
    QMainWindow *m_selected_sim;
    QMessageBox *m_error;
    Ui::MainWindow *ui;
    std::unique_ptr<Web_socket_wrapper> m_web_socket;
    std::map<QString, std::unique_ptr<Simulator_base>> m_sims;
};
