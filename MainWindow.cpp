// MainWindow.cpp

// Implements the MainWindow class representing the app's main window





#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Document.h"
#include "Page.h"





MainWindow::MainWindow(QWidget * a_Parent):
	Super(a_Parent),
	m_UI(new Ui::MainWindow),
	m_Document(std::make_shared<Document>())
{
	m_CurrentPage = m_Document->pages()[0];
	m_CurrentPage->setCurrentTool(Page::toolDraw);
	m_UI->setupUi(this);
	m_UI->gvMain->setScene(m_CurrentPage.get());
	m_UI->gvMain->setRenderHint(QPainter::Antialiasing, true);
	// TODO: m_UI->gvMain->setDragMode(QGraphicsView::RubberBandDrag);

	// Set up Tool selection:
	auto grp = new QActionGroup(this);
	grp->addAction(m_UI->actActionSelect);
	grp->addAction(m_UI->actActionDraw);
	m_UI->actActionSelect->setProperty("tool", Page::toolSelect);
	m_UI->actActionDraw->setProperty  ("tool", Page::toolDraw);
	connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(toolSelected(QAction *)));
}





MainWindow::~MainWindow()
{
	// Reset the scene before possibly deallocating it (just in case):
	m_UI->gvMain->setScene(nullptr);
}





void MainWindow::toolSelected(QAction * a_Action)
{
	m_CurrentPage->setCurrentTool(static_cast<Page::Tool>(a_Action->property("tool").toInt()));
}




