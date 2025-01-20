#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // First we need to add the template of Robot data from the file.
    addRobotDataTemplate();


    // Setup the 3D Playground
    setup3DPlayground();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Add the Robot Data Template
void MainWindow::addRobotDataTemplate() {

    QFile file(":/Resources/Json/Template.json");

    if (!file.open(QIODevice::ReadOnly) | QIODevice::Text) {
        qWarning("Template file is missing. Please add the template file.");
        return;
    }

    QByteArray templateJson = file.readAll();
    file.close();


    QJsonDocument templateDocument = QJsonDocument::fromJson(templateJson);
    QJsonObject templateObject = templateDocument.object();

    
    QStandardItemModel *model = new QStandardItemModel(0, 2, this);
}

// Setup the Main Playground for 3D Viewer
void MainWindow::setup3DPlayground() {

    // First creating 3D window
    view = new Qt3DExtras::Qt3DWindow();
    rootEntity = new Qt3DCore::QEntity();

    // Set the background color
    view->defaultFrameGraph()->setClearColor(QColor(Qt::darkCyan));

    // Set the root Entity
    view->setRootEntity(rootEntity);

    // Create a window container for view
    QWidget *container = QWidget::createWindowContainer(view);

    // Now its time to show this view on frontend.
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(container);
    ui->viewContainer->setLayout(layout);
}


