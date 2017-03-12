// MainWindow.cpp

// Implements the MainWindow class representing the app's main window





#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "ui_MainWindow.h"
#include "Document.h"
#include "Page.h"





MainWindow::MainWindow(QWidget * a_Parent):
	Super(a_Parent),
	m_UI(new Ui::MainWindow),
	m_Document(std::make_shared<Document>())
{
	auto currentPage = m_Document->currentPage();
	currentPage->setCurrentTool(Page::toolDraw);
	m_UI->setupUi(this);
	m_UI->gvMain->setScene(currentPage.get());
	m_UI->gvMain->setRenderHint(QPainter::Antialiasing, true);
	// TODO: m_UI->gvMain->setDragMode(QGraphicsView::RubberBandDrag);

	// Set up Tool selection:
	auto grp = new QActionGroup(this);
	grp->addAction(m_UI->actActionSelect);
	grp->addAction(m_UI->actActionDraw);
	m_UI->actActionSelect->setProperty("tool", Page::toolSelect);
	m_UI->actActionDraw->setProperty  ("tool", Page::toolDraw);
	connect(grp, SIGNAL(triggered(QAction *)), this, SLOT(toolSelected(QAction *)));

	// Set up the action connections:
	connect(m_UI->actFileOpen,   SIGNAL(triggered()), this, SLOT(openFile()));
	connect(m_UI->actFileSave,   SIGNAL(triggered()), this, SLOT(saveFile()));
	connect(m_UI->actFileSaveAs, SIGNAL(triggered()), this, SLOT(saveFileAs()));
}





MainWindow::~MainWindow()
{
	// Reset the scene before possibly deallocating it (just in case):
	m_UI->gvMain->setScene(nullptr);
}





void MainWindow::openFile()
{
	if (!checkDocumentModifiedSave())
	{
		return;
	}
	auto fnam = QFileDialog::getOpenFileName(
		this,
		tr("Open file:"),
		QString(),
		tr("DoodleNote documents (*.doodlenote)")
	);
	if (fnam.isEmpty())
	{
		return;
	}
	openFile(fnam);
}





void MainWindow::openFile(const QString & a_FileName)
{
	QFile f(a_FileName);
	if (!f.open(QFile::ReadOnly))
	{
		QMessageBox::warning(this, tr("Cannot open file"), tr("Cannot open file %1 for reading").arg(a_FileName));
		return;
	}
	auto doc = std::make_shared<Document>(a_FileName);
	QDataStream ds(&f);
	try
	{
		doc->loadFromStream(ds);
	}
	catch (const std::exception & exc)
	{
		QMessageBox::warning(this, tr("Cannot open file"), tr("Cannot open file %1: %2").arg(a_FileName).arg(exc.what()));
		return;
	}
	m_Document = doc;
	m_UI->gvMain->setScene(m_Document->currentPage().get());
}





bool MainWindow::saveFile()
{
	if (m_Document->fileName().isEmpty())
	{
		return saveFileAs();
	}
	return saveFileAs(m_Document->fileName());
}





bool MainWindow::saveFileAs()
{
	auto fnam = QFileDialog::getSaveFileName(
		this,
		tr("Save as:"),
		QString(),
		tr("DoodleNote document (*.doodlenote)")
	);
	if (fnam.isEmpty())
	{
		return false;
	}
	return saveFileAs(fnam);
}





bool MainWindow::saveFileAs(const QString & a_FileName)
{
	QFile f(a_FileName);
	if (!f.open(QFile::WriteOnly))
	{
		QMessageBox::warning(this, tr("Cannot save file"), tr("Cannot save to file %1, inaccessible").arg(a_FileName));
		return false;
	}
	QDataStream ds(&f);
	try
	{
		m_Document->saveToStream(ds);
	}
	catch (const std::exception & exc)
	{
		QMessageBox::warning(
			this,
			tr("Cannot save file"),
			tr("Cannot save to file %1: %2").arg(a_FileName).arg(exc.what())
		);
		return false;
	}
	m_Document->setFileName(a_FileName);
	return true;
}





void MainWindow::toolSelected(QAction * a_Action)
{
	m_Document->currentPage()->setCurrentTool(static_cast<Page::Tool>(a_Action->property("tool").toInt()));
}





bool MainWindow::checkDocumentModifiedSave()
{
	if (!m_Document->isModified())
	{
		return true;
	}
	auto dlgRes = QMessageBox::question(
		this,
		tr("File not saved"),
		tr("Current file is not saved. Save the changes?"),
		QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
	);
	switch (dlgRes)
	{
		case QMessageBox::Save:
		{
			// Save the file, Cancel if saving fails:
			if (!saveFile())
			{
				return false;
			}
			return true;
		}
		case QMessageBox::Cancel:
		{
			// Cancel altogether:
			return false;
		}
		default:
		{
			// Continue:
			return true;
		}
	}
}




