#include <QtWidgets>
#include "scribblearea.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scribbleArea = new ScribbleArea;
    setCentralWidget(scribbleArea);
    createActions();
    createMenus();
    setWindowTitle(tr("painTQ"));
    resize(500, 500);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::open()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
        {
            scribbleArea->openImage(fileName);
        }
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByte fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid())
    {
        scribbleArea->setPenColor(newColor);
    }
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("painTQ"),
                                    tr("Select pen width: "),
                                    scribbleArea->penWidth(),
                                    1, 50, 1, &ok);
    if(ok)
    {
        scribbleArea->setPenWidth(newWidth);
    }
}

void MainWindow::about()
{
                         QMessageBox::about(this, tr("About painTQ"),
                       tr("<p>This paint app was coded by Zach Shaffer</p>"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
