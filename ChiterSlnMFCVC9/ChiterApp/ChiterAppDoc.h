// ChiterAppDoc.h : interface of the CChiterAppDoc class
//


#pragma once

#include "Library/AuxiliaryTypedefs.h"

class CChiterAppDoc : public CDocument
{
protected: // create from serialization only
	CChiterAppDoc();
	DECLARE_DYNCREATE(CChiterAppDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CChiterAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


