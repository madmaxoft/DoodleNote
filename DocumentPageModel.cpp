// DocumentPageModel.cpp

// Implements the DocumentPageModel class representing the model of Document's pages





#include "DocumentPageModel.h"
#include "Page.h"
#include "Document.h"





DocumentPageModel::DocumentPageModel(int a_PreviewWidth, int a_PreviewHeight):
	m_PreviewWidth(a_PreviewWidth),
	m_PreviewHeight(a_PreviewHeight)
{
}





void DocumentPageModel::setDocument(std::shared_ptr<Document> a_Document)
{
	// Disconnect the previous model:
	if (m_Document != nullptr)
	{
		disconnect(m_Document.get());
	}

	// Set the new model:
	m_Document = a_Document;

	// Connect the new model:
	if (m_Document != nullptr)
	{
		connect(m_Document.get(), &Document::pageAdded,    this, &DocumentPageModel::pageAdded);
		connect(m_Document.get(), &Document::pageInserted, this, &DocumentPageModel::pageInserted);
		connect(m_Document.get(), &Document::pageChanged,  this, &DocumentPageModel::pageChanged);
	}
}





int DocumentPageModel::rowCount(const QModelIndex & a_Parent) const
{
	Q_UNUSED(a_Parent);

	if (m_Document == nullptr)
	{
		return 0;
	}
	return static_cast<int>(m_Document->pages().size());
}





QVariant DocumentPageModel::data(const QModelIndex & a_Index, int a_Role) const
{
	if (
		!a_Index.isValid() ||
		(m_Document == nullptr) ||
		(a_Index.row() >= static_cast<int>(m_Document->pages().size()))
	)
	{
		return QVariant();
	}

	switch (a_Role)
	{
		case Qt::DisplayRole:
		{
			return a_Index.row();
		}
		case Qt::DecorationRole:
		{
			const auto & page = m_Document->pages()[a_Index.row()];
			return page->render(m_PreviewWidth, m_PreviewHeight);
		}
	}
	return QVariant();
}





int DocumentPageModel::rowFromPage(Page * a_Page)
{
	if (m_Document == nullptr)
	{
		return -1;
	}

	const auto & pages = m_Document->pages();
	const auto num = pages.size();
	for (size_t i = 0; i < num; ++i)
	{
		if (pages[i].get() == a_Page)
		{
			return static_cast<int>(i);
		}
	}
	return -1;
}





void DocumentPageModel::pageAdded()
{
	auto row = static_cast<int>(m_Document->pages().size()) - 1;
	beginInsertRows(QModelIndex(), row, row);
	endInsertRows();
}





void DocumentPageModel::pageInserted(int a_InsertedAfterIndex)
{
	beginInsertRows(QModelIndex(), a_InsertedAfterIndex, a_InsertedAfterIndex);
	endInsertRows();
}





void DocumentPageModel::pageChanged(Page * a_Page)
{
	// Notify the view of the changed data:
	int row = rowFromPage(a_Page);
	if (row >= 0)
	{
		QModelIndex idx = index(row, 0);
		dataChanged(idx, idx);
	}
}




