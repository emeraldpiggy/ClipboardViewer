#include <iostream>
#include <io.h>
#include <shlguid.h>
#include <shlobj.h>
#include <vector>

using namespace std;

struct CClipFormatData { UINT uFormat; LPCTSTR szFormatName; };
extern CClipFormatData g_aClipData[];

#define IDC_READ_CLIPBOARD              32772

CClipFormatData g_aClipData[] = {
	{ CF_TEXT, _T("CF_TEXT") },
	{ CF_BITMAP, _T("CF_BITMAP") },
	{ CF_METAFILEPICT, _T("CF_METAFILEPICT") },
	{ CF_SYLK, _T("CF_SYLK") },
	{ CF_DIF, _T("CF_DIF") },
	{ CF_TIFF, _T("CF_TIFF") },
	{ CF_OEMTEXT, _T("CF_OEMTEXT") },
	{ CF_DIB, _T("CF_DIB") },
	{ CF_PALETTE, _T("CF_PALETTE") },
	{ CF_PENDATA, _T("CF_PENDATA") },
	{ CF_RIFF, _T("CF_RIFF") },
	{ CF_WAVE, _T("CF_WAVE") },
	{ CF_UNICODETEXT, _T("CF_UNICODETEXT") },
	{ CF_ENHMETAFILE, _T("CF_ENHMETAFILE") },
	{ CF_HDROP, _T("CF_HDROP") },
	{ CF_LOCALE, _T("CF_LOCALE") },
	{ CF_DIBV5, _T("CF_DIBV5") },
	{ CF_OWNERDISPLAY, _T("CF_OWNERDISPLAY") },
	{ CF_DSPTEXT, _T("CF_DSPTEXT") },
	{ CF_DSPBITMAP, _T("CF_DSPBITMAP") },
	{ CF_DSPMETAFILEPICT, _T("CF_DSPMETAFILEPICT") },
	{ CF_DSPENHMETAFILE, _T("CF_DSPENHMETAFILE") },
	{ 0, _T("") }
};



class ClipboardViewer :public CListView
{
public:
	ClipboardViewer();
	BOOL ReadData(COleDataObject* pDO);
	void OnReadClipboard();
	DECLARE_MESSAGE_MAP()

public:
	~ClipboardViewer();
};