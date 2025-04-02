#include "plot_wrapper.hpp"
#include <QPen>

Plot_wrapper::Plot_wrapper(QWidget *parent) : QwtPlot(parent)
{
    m_curve = new QwtPlotCurve("Test 1");
    m_curve->setPen(QPen(Qt::white));
    m_curve->attach(this);
    this->replot();
}

QwtPlotCurve *Plot_wrapper::curve() const
{
    return m_curve;
}
