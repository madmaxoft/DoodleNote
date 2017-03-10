// Document.cpp

// Implements the Document class representing a single document consisting of multiple pages





#include "Document.h"
#include "Page.h"





Document::Document()
{
	m_Pages.push_back(std::make_shared<Page>());
}
