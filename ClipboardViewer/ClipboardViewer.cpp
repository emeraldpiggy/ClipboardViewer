// ClipboardViewer.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ClipboardViewer.h"

BEGIN_MESSAGE_MAP(ClipboardViewer, CListView)
	ON_COMMAND(IDC_READ_CLIPBOARD, OnReadClipboard)
END_MESSAGE_MAP()

int _tmain(int argc, _TCHAR* argv[])
{
	ClipboardViewer co;
	co.OnReadClipboard();
	cout << "Please enter to quit";
	std::cin.ignore();
	return 0;

}

void ClipboardViewer::OnReadClipboard()
{
	COleDataObject xDataObj;
	if (!xDataObj.AttachClipboard())
		return;
	ReadData(&xDataObj);
}

ClipboardViewer::ClipboardViewer()
{


}
ClipboardViewer::~ClipboardViewer()
{

}
BOOL ClipboardViewer::ReadData(COleDataObject* pDO)
{
	std::vector<TCHAR*>   list;
	FORMATETC        etc;
	UINT             uNumFormats = 0;
	CClipFormatData* pClip;
	TCHAR            szFormat[256];
	HGLOBAL          hgData;
	UINT             uDataSize;
	int              nItem = 0;

	ASSERT(AfxIsValidAddress(pDO, sizeof(COleDataObject)));

	// Empty the list control and the doc.
	list.clear();

	// Determine how many formats are available on the clipboard.

	pDO->BeginEnumFormats();

	while (pDO->GetNextFormat(&etc))
	{
		if (pDO->IsDataAvailable(etc.cfFormat))
		{
			uNumFormats++;
		}
	}

	// Now get all the data and pass it to the doc for storage.

	pDO->BeginEnumFormats();

	//return TRUE;

	while (pDO->GetNextFormat(&etc))
	{
		bool bReadData = false;

		if (!pDO->IsDataAvailable(etc.cfFormat))
			continue;

		for (pClip = &g_aClipData[0]; 0 != pClip->uFormat; pClip++)
		{
			if (etc.cfFormat == pClip->uFormat)
			{
				lstrcpy(szFormat, pClip->szFormatName);
				break;
			}
		}

		// If we didn't find the format in g_aClipData, then it's a custom
		// format, and we need to get the name from Windows.
		if (0 == pClip->uFormat)
		{
			GetClipboardFormatName(etc.cfFormat, szFormat, 256);
		}

		// Get an HGLOBAL of the data.
		hgData = pDO->GetGlobalData(etc.cfFormat);

		if (NULL != hgData)
		{
			uDataSize = GlobalSize ( hgData );

			cout <<"1::"<< uDataSize<<"\n";
			cout <<"2::"<< hgData<<"\n";

			// Free the memory that GetGlobalData() allocated for us.
			GlobalFree ( hgData );

			bReadData = true;
		}
		else
		{
			// The data isn't in global memory, so try getting an IStream 
			// interface to it.
			STGMEDIUM stg;

			if (pDO->GetData(etc.cfFormat, &stg))
			{
				switch (stg.tymed)
				{
				case TYMED_HGLOBAL:
				{
					uDataSize = GlobalSize(stg.hGlobal);

					cout << "3::" << uDataSize << "\n";
					bReadData = true;
				}
				break;

				case TYMED_ISTREAM:
				{
					LARGE_INTEGER li;
					ULARGE_INTEGER uli;

					li.HighPart = li.LowPart = 0;

					if (SUCCEEDED(stg.pstm->Seek(li, STREAM_SEEK_END, &uli)))
					{
						HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE,
							uli.LowPart);
						void* pv = GlobalLock(hg);

						stg.pstm->Seek(li, STREAM_SEEK_SET, NULL);

						if (SUCCEEDED(stg.pstm->Read(pv, uli.LowPart, (PULONG)&uDataSize)))
						{
							GlobalUnlock(hg);

							cout << "4::" << uDataSize << "\n";
							
							GlobalFree(hg);

							bReadData = true;
						}
						else
						{
							GlobalUnlock(hg);
						}
					}
				}
				break;  // case TYMED_ISTREAM
				}

				ReleaseStgMedium(&stg);
			}
		}
		list.push_back(szFormat);

		if (bReadData)
		{
			cout <<"5::"<< bReadData << "\n";
		}
		else
		{
			list.push_back(_T("<Data unavailable>"));
		}

		nItem++;
	}   


	for (TCHAR* c : list)
	{
		cout <<"6::"<< c << "\n";
	}

	return TRUE;

}