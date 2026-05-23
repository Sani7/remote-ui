#pragma once
#include <QMainWindow>
#include <map>
#include "messages.qpb.h"

#define INSERT_SIMULATOR(type)                                                                                         \
    m_sims.insert(std::make_pair(type().sim_name(), std::make_unique<type>(m_web_socket.get(), this)))

Q_FORWARD_DECLARE_OBJC_CLASS(QMessageBox);
Q_FORWARD_DECLARE_OBJC_CLASS(UI_base);
Q_FORWARD_DECLARE_OBJC_CLASS(QTimer);
namespace RemoteUiService
{
  class Client;
}

namespace Health
{
class Client;
}

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    /**
     * @brief Construct a new Main Window object
     *
     * @param ws_url The WebSocket URL
     * @param sim The default simulator name
     * @param parent The parent widget
     */
    explicit MainWindow(QUrl ws_url, QString sim, QWidget *parent = nullptr);
    ~MainWindow();

  signals:
    void on_show_event();
  private:
    /**
     * @brief Handle the show event
     *        And will connect the WebSocket signals
     *
     * @param event
     */
    void showEvent(QShowEvent *event) override;
    /**
     * @brief Handle the hide event
     *        And will disconnect the WebSocket signals
     *
     * @param event
     */
    void hideEvent(QHideEvent *event) override;

    /**
     * @brief Set the default simulator only possible on construction
     *
     * @param name The simulator name
     */
    void default_sim(QString name);
    /**
     * @brief Parse the simulator names from JSON and add them to the combo box
     *
     * @param sims The JSON array of simulator names
     */
    void pase_sim_names(Simulators_m &sims);
    /**
     * @brief Open the simulator window using the selected simulator
     */
    void open_sim_window(void);
    /**
     * @brief Open a simulator by name
     *
     * @param sim_name The simulator name
     */
    void check_active_sim(QString name);
    /**
     * @brief Open a simulator by name
     *        Used when using the default simulator
     *
     * @param sim_name
     */
    void open_sim(QString sim_name);

    bool m_default_sim = false;
    QString m_selected_sim_name;
    QMainWindow *m_selected_sim;
    QMessageBox *m_error;
    Ui::MainWindow *ui;
    std::unique_ptr<RemoteUiService::Client> m_remote_ui_client;
    std::map<QString, std::unique_ptr<UI_base>> m_sims;
    QTimer *m_ping;
};
