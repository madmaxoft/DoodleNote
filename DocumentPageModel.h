// DocumentPageModel.h

// Declares the DocumentPageModel class representing the model of Document's pages





#ifndef DOCUMENTPAGEMODEL_H
#define DOCUMENTPAGEMODEL_H





#include <memory>
#include <QAbstractListModel>





// fwd:
class Document;
class Page;





class DocumentPageModel:
	public QAbstractListModel
{
public:
	DocumentPageModel(int a_PreviewWidth, int a_PreviewHeight);

	/** Resets the model to the specified Document. */
	void setDocument(std::shared_ptr<Document> a_Document);

	// QAbstractListModel overrides:
	virtual int rowCount(const QModelIndex & a_Parent) const override;
	virtual QVariant data(const QModelIndex & a_Index, int a_Role) const override;


protected:

	/** The document modelled by this class. */
	std::shared_ptr<Document> m_Document;

	/** Dimensions of the preview images. */
	int m_PreviewWidth, m_PreviewHeight;


	/** Returns the row on which the specified page is modelled.
	Returns -1 if the page is not modelled. */
	int rowFromPage(Page * a_Page);

protected slots:

	/** Emitted by m_Document after a page is added. */
	void pageAdded();

	/** Emitted by m_Document after a page is inserted.
	a_InsertedAfterIndex is the index of the page after which the page has been inserted. */
	void pageInserted(int a_InsertedAfterIndex);

	/** Emitted when the data in the page is changed. */
	void pageChanged(Page * a_Page);
};





#endif // DOCUMENTPAGEMODEL_H
