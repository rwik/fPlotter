#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QFileDialog>
#include <iostream>
#include <qcustomplot.h>
#include <qfilesystemwatcher.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMenu *fileMenu;
    QMenuBar *menu ;
    QVector<double> dx,dy;
    QFileSystemWatcher watcher;
    QCPGraph *graphPlot;

private slots:    
    void onOpen();
    void onQuit();
    void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void addRandomGraph(int mode);
    void removeSelectedGraph();
    void removeAllGraphs();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    void graphClicked(QCPAbstractPlottable *plottable);
    void load(QString);
    void saveScreenShot();
    void update(QString);


private :
    void SetupMenu();
    void setupSignals();
    void     SetupUI();



};

#endif // MAINWINDOW_H
