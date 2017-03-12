// Document.cpp

// Implements the Document class representing a single document consisting of multiple pages





#include "Document.h"
#include "Page.h"





Document::Document()
{
	addPage();
	m_CurrentPage = m_Pages[0];
	m_IsModified = false;
}





Document::Document(const QString & a_FileName)
{
	m_FileName = a_FileName;
}





void Document::saveToStream(QDataStream & a_Stream) const
{
	a_Stream << "DoodleNoteDocument";
	a_Stream << static_cast<quint32>(0);  // Version
	// TODO: Save a preview image
	a_Stream << static_cast<quint32>(m_Pages.size());
	quint32 idx = 0;
	quint32 curPageIdx = 0;
	for (const auto & page: m_Pages)
	{
		page->saveToStream(a_Stream);
		if (page == m_CurrentPage)
		{
			curPageIdx = idx;
		}
		++idx;
	}
	a_Stream << curPageIdx;
}





void Document::loadFromStream(QDataStream & a_Stream)
{
	char * ident;
	a_Stream >> ident;
	if (ident == nullptr)
	{
		throw std::runtime_error("Cannot read file identification string, probably not a DoodleNote file");
	}
	if (strcmp(ident, "DoodleNoteDocument") != 0)
	{
		throw std::runtime_error("Wrong identification string, probably not a DoodleNote file");
	}
	delete[] ident;
	quint32 version;
	a_Stream >> version;
	if (version == 0)
	{
		loadFromStreamV0(a_Stream);
		return;
	}
	throw std::runtime_error("The file is from an unrecognized version of DoodleNote.");
}





PagePtr Document::addPage()
{
	auto page = std::make_shared<Page>();
	m_Pages.push_back(page);
	connect(page.get(), SIGNAL(changed()), this, SLOT(pageChanged()));
	m_IsModified = true;
	return page;
}





void Document::loadFromStreamV0(QDataStream & a_Stream)
{
	// Read the number of pages:
	quint32 numPages = 0;
	a_Stream >> numPages;
	if (numPages > 30000)
	{
		throw std::runtime_error("Probably a broken file, too many pages");
	}

	// Read each page:
	for (quint32 i = 0; i < numPages; i++)
	{
		auto page = addPage();
		page->loadFromStream(a_Stream);
	}

	// Read and set the current page:
	quint32 curPageIdx = 0;
	a_Stream >> curPageIdx;
	if (curPageIdx >= numPages)
	{
		throw std::runtime_error("Invalid current page index");
	}
	m_CurrentPage = m_Pages[curPageIdx];
}





void Document::pageChanged()
{
	m_IsModified = true;
}




