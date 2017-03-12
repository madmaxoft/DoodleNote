// MainWindow.h

// Declares the MainWindow class representing the app's main window






#ifndef MAINWINDOW_H
#define MAINWINDOW_H





#include <memory>
#include <QMainWindow>
#include "PageScene.h"




// fwd:
class Document;
class Page;





namespace Ui
{
	class MainWindow;
}





class MainWindow:
	public QMainWindow
{
	Q_OBJECT

	typedef QMainWindow Super;


public:

	explicit MainWindow(QWidget * a_Parent = nullptr);

	~MainWindow();


public slots:

	/** Shows an "open file" dialog and opens the selected file. */
	void openFile();

	/** Opens the specified file. */
	void openFile(const QString & a_FileName);

	/** Saves the current file to m_FileName.
	If no filename is stored, calls saveFileAs to show the "save file as" dialog.
	Returns true if the file is saved successfully, false on failure / cancellation. */
	bool saveFile();

	/** Shows a "save file as" dialog and saves the document to the specified file.
	Returns true on success, false on failure / cancellation. */
	bool saveFileAs();

	/** Saves the document data into the specified file.
	Returns true on success, false on failure / cancellation. */
	bool saveFileAs(const QString & a_FileName);

protected slots:

	/** Triggered by the Tool selection actions.
	Uses a_Action's property("tool") to distinguish between the individual tools. */
	void toolSelected(QAction * a_Action);


protected:

	/** The UI. */
	std::shared_ptr<Ui::MainWindow> m_UI;

	/** The document currently loaded. */
	std::shared_ptr<Document> m_Document;

	/** The convertor from Page to QGraphicScene, used for display and interaction. */
	PageScene m_PageScene;


	/** If the document is modified, asks the user whether to save.
	Returns true if either the document is saved or the user wants to discard changes.
	Returns false if the next action (opening / exiting / ...) should be aborted. */
	bool checkDocumentModifiedSave();
};





#endif // MAINWINDOW_H
