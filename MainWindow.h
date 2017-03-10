// MainWindow.h

// Declares the MainWindow class representing the app's main window






#ifndef MAINWINDOW_H
#define MAINWINDOW_H





#include <memory>
#include <QMainWindow>




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


protected slots:

	/** Triggered by the Tool selection actions.
	Uses a_Action's property("tool") to distinguish between the individual tools. */
	void toolSelected(QAction * a_Action);


private:

	/** The UI. */
	std::shared_ptr<Ui::MainWindow> m_UI;

	/** The document currently loaded. */
	std::shared_ptr<Document> m_Document;

	/** The page (out of m_Document) currently displayed. */
	std::shared_ptr<Page> m_CurrentPage;
};





#endif // MAINWINDOW_H
