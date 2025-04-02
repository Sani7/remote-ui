#ifndef PLOT_WRAPPER_HPP
#define PLOT_WRAPPER_HPP

#include <QObject>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_text.h>

class Plot_wrapper : public QwtPlot
{
    Q_OBJECT
  public:
    Plot_wrapper(QWidget *parent = nullptr);
    QwtPlotCurve *curve() const;

  private:
    QwtPlotCurve *m_curve;
};

#endif // PLOT_WRAPPER_HPP
