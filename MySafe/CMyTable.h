#ifndef CMYTABLE_H
#define CMYTABLE_H

// CMyTable

class CMyTable : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTable)

public:
	CMyTable();
	virtual ~CMyTable();

protected:
	DECLARE_MESSAGE_MAP()
public:

	CDialogEx *m_Dialg[10];

	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};

#endif
