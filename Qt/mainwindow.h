#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>
#include <QMainWindow>
#include "OAICANApiApi.h"
#include "networkerror.h"
#include "simulatorbase.h"
#include "helpers.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void defaultSim(QString name);
    void Parse_Sims(QList<QString> sims, QNetworkReply::NetworkError error_type = QNetworkReply::NetworkError::NoError, QString error_str = QString());
    void apiCanapiUielements_bool_cb(bool ret, QNetworkReply::NetworkError error_type = QNetworkReply::NetworkError::NoError, QString error_str = QString());
    void apiCanapiCurrentsimGet_cb(OpenAPI::OAISimulatorInfo summary, QNetworkReply::NetworkError error_type = QNetworkReply::NetworkError::NoError, QString error_str = QString());
    void open_sim_window(void);
    void open_sim(QString sim_name);
    void setup_cb(void);

  signals:
    void quit(void);
  private:
    void showEvent( QShowEvent* event );
    void hideEvent(QHideEvent* event);

    bool default_sim = false;
    QString selected_sim_name;
    QMainWindow* selected_sim;
    Ui::MainWindow *ui;
    OpenAPI::OAICANApiApi *api;
    NetworkError* error_dialog;

    std::map<QString, SimulatorBase*> sims;
};

#endif // MAINWINDOW_H
