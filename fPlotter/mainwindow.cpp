#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetupMenu();
    SetupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSignals()
{
    //connect(menuBar()->ac)

}

void MainWindow::load(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }



    QTextStream t(&file);
    QStringList list;
    QString token = t.readLine();
    dx.clear();
    dy.clear();
    while(!t.atEnd())
    {

        list = token.split(",");
        if(list.size() == 2)
        {
            dx.push_back(list.at(0).toDouble());
            dy.push_back(list.at(1).toDouble());

            //std::cout<<list.at(0).toDouble()<<list.at(1).toDouble();

        }
        token = t.readLine();
        list.clear();

    }
    file.close();
    addRandomGraph(0);

}

void MainWindow::saveScreenShot()
{
    ui->customPlot->saveJpg("a.jpg",0,0,2,100);

}

void MainWindow::SetupUI()
{

    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->customPlot->xAxis->setRange(0, 8);
    ui->customPlot->yAxis->setRange(0, 5);
    ui->customPlot->axisRect()->setupFullAxesBox();

    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, "fPlot"));

    ui->customPlot->xAxis->setLabel("x Axis");
    ui->customPlot->yAxis->setLabel("y Axis");
    ui->customPlot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setSelectedFont(legendFont);
    //ui->customPlot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
    ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

    // make bottom and left axes transfer their ranges to top and right axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // connect some interaction slots:
    connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    connect(ui->customPlot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(ui->customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));



}

void MainWindow::onOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."), QString(), tr("Text-files (*.txt);;All Files (*)"));
    if (!fn.isEmpty())
    {
        watcher.removePath(fn);
        watcher.addPath(fn);
        connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(update(QString)));

        load(fn);
    }

}

void MainWindow::update(QString fileName )
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Cannot open file for writing: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }


    QTextStream t(&file);
    QStringList list;
    QString token = t.readLine();
    dx.clear();
    dy.clear();
    while(!t.atEnd())
    {

        list = token.split(",");
        if(list.size() == 2)
        {
            dx.push_back(list.at(0).toDouble());
            dy.push_back(list.at(1).toDouble());

            //std::cout<<list.at(0).toDouble()<<list.at(1).toDouble();

        }
        token = t.readLine();
        list.clear();

    }
    file.close();
    addRandomGraph(1);

}

void MainWindow::onQuit()
{
    close();

}

void MainWindow::SetupMenu()
{

    fileMenu = new QMenu(tr("&File"));
    // fileMenu = menu->addMenu("&File");

    /* Add some choices to the menu. */
    fileMenu->addAction(tr("&Open"),
                        this, SLOT(onOpen()));
    fileMenu->addAction(tr("Save Screen"),
                        this, SLOT(saveScreenShot()));
    fileMenu->addAction(tr("Quit"),
                        this, SLOT(onQuit()));


    QMainWindow::menuBar()->addMenu(fileMenu);




}
void MainWindow::axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part){


}
void  MainWindow::legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item){}
void  MainWindow::selectionChanged(){}
void  MainWindow::mousePress(){

    if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
    else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
    else
        ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}
void  MainWindow::mouseWheel(){

    if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
    else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
    else
        ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}
void  MainWindow::addRandomGraph(int mode){
    if(mode ==0)
    {
        if(dx.size()!=0 && dy.size()!=0)
        {
            graphPlot = ui->customPlot->addGraph();
            ui->customPlot->graph()->setName(QString("New graph %1").arg(ui->customPlot->graphCount()-1));
            ui->customPlot->graph()->setData(dx, dy);
            ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
            ui->customPlot->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ssDot)));
            QPen graphPen;
            graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
            graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
            ui->customPlot->graph()->setPen(graphPen);
            ui->customPlot->replot();

        }
    }else{
        graphPlot->clearData();

        ui->customPlot->graph()->setData(dx, dy);
        ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        ui->customPlot->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ssDot)));
        QPen graphPen;
        graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
        graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
        ui->customPlot->graph()->setPen(graphPen);
        ui->customPlot->replot();

    }


}
void  MainWindow::removeSelectedGraph(){
    if (ui->customPlot->selectedGraphs().size() > 0)
    {
        ui->customPlot->removeGraph(ui->customPlot->selectedGraphs().first());
        ui->customPlot->replot();
    }
}
void  MainWindow::removeAllGraphs(){
    ui->customPlot->clearGraphs();
    ui->customPlot->replot();


}
void  MainWindow::contextMenuRequest(QPoint pos){
    bool flag =false;
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);



    if (ui->customPlot->selectedGraphs().size() > 0)
    {
        menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
        flag =true;
    }
    if (ui->customPlot->graphCount() > 0)
    {
        menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
        flag =true;
    }
    if(flag)
    {
        menu->popup(ui->customPlot->mapToGlobal(pos));
    }
}
void  MainWindow::moveLegend(){}
void  MainWindow::graphClicked(QCPAbstractPlottable *plottable){}
