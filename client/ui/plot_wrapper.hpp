#pragma once
#include <QObject>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_text.h>

class Plot_wrapper : public QwtPlot
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a new Plot_wrapper object
     *
     * @param parent The parent QWidget
     */
    Plot_wrapper(QWidget *parent = nullptr);

    /**
     * @brief Get the curve associated with the plot
     *
     * @return QwtPlotCurve*
     */
    QwtPlotCurve *curve() const;

  private:
    QwtPlotCurve *m_curve;
};
