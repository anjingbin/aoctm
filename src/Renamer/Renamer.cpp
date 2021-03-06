
#include "stdafx.h"
#include "Renamer.h"

//07-10-11, pubb, m_Path is of no use now
//Renamer::Renamer(CString path)
Renamer::Renamer()
{
	m_Prefixes.Add(_T("recorded game"));
	m_Prefixes.Add(_T("已记录游戏"));
	m_Prefixes.Add(_T("游戏录象"));
}

Renamer::~Renamer()
{
	/*
	for( int i = 0;i < m_Files.GetCount();i++ )
	{
		delete m_Files.GetAt(i);
	}
	*/
}

void
Renamer::SetPattern(CString pattern)
{
}

/* pubb, 07-08-10, change the interface and function of it
 * input: rec filename with/without path
 * output: CTime object
 */

/*
void
Renamer::Parse()
{
	CString strWildcard = m_Path + _T("\\");
	strWildcard += _T("*.mgx");
	CFileFind finder;

	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory())
			continue;

		CString str = finder.GetFileName();
		RecFile *file = new RecFile(str);
		file->Parse(m_Prefixes);

		m_Files.Add(file);
	}
}
*/

/* pubb, 07-08-11, Parse(), almost rewrite */
/* pubb, 14-06-29. Parse() to recognize all file format as I konw */
CTime	Renamer::Parse2Playtime(CString file)
{
	enum { MGX, MGZ, UNKNOWN } filetype=UNKNOWN;
	int index;
	//trim path
	index = file.ReverseFind('\\');
	file = file.Mid(index + 1);

	//检查文件扩展名
	int len = file.GetLength();
	//.mgx (1.0c) or .mgz (1.4 userpatch or aofe)
	CString ext = file.Mid(len - 3, 3);
	if (ext.Compare(_T("mgx")) == 0)
	{
		filetype = MGX;
	}
	if (ext.Compare(_T("mgz")) == 0)
	{
		filetype = MGZ;
	}
	if (filetype == UNKNOWN)
	{
		return CTime(0);
	}

	//spite file extension
	file = file.Mid(0, len - 4);
	file.Trim();

	/* pubb, 07-08-10, don't care for any prefix before digitals */
	index = file.FindOneOf(_T("0123456789"));
	file = file.Mid(index);

	int year, month, day, hour, minute, second;

	//mgx
	if( filetype == MGX )
	{
		CString token;
		int curPos= 0;

		//得到时间字符串
		//pubb, 07-11-24, add '_' for token seperator
		token = file.Tokenize(_T("_-` "), curPos);
		int result[6];
		int j = 0;

		//pubb, 09-02-07, bug fix for j exceeding limit
		while( token != "" && j < 6 )
		{
			result[j] = _ttoi((LPCTSTR)token);

			if( result[j] == 0 )
			{
				//转换失败，可能是中文或者英文的月份
				result[j] = ToMonth(token);
			}

			token = file.Tokenize(_T("-` "), curPos);

			j++;
		}

		int year, day;
		if(result[0] > 31)	// //altered NormalName : 'recorded game YYYY-MM-DD hh`mm`ss'
		{
			year = result[0];
			day = result[2];
		}
		else	//orignial name : 'recorded game MM-DD-YYYY
		{
			year = result[2];
			day = result[0];
		}
		//pubb, 07-10-23, don't do that any more.
		/* XXX, SET year to be 2007 in case that the machine time is not correct in many places */
		//year = 2007;
		month = result[1];
		hour = result[3];
		minute = result[4];
		second = result[5];
	} else	//mgz
	{
		year = _ttoi((LPCTSTR)(file.Left(4)));
		month = _ttoi((LPCTSTR)(file.Mid(4,2)));
		day = _ttoi((LPCTSTR)(file.Mid(6,2)));
		hour = _ttoi((LPCTSTR)(file.Mid(9,2)));
		minute = _ttoi((LPCTSTR)(file.Mid(11,2)));
		second = _ttoi((LPCTSTR)(file.Mid(13,2)));
	}

	if(month < 1 || month > 12 || day < 1 || day > 31 || hour < 0 || hour > 24 || minute < 0 || minute > 60 || second < 0 || second > 60)
		return CTime(0);

	return CTime(year, month, day, hour, minute, second);
}

int
Renamer::ToMonth(CString& s)
{
	if( (s.CompareNoCase(_T("Jan")) == 0) ||
		(s.CompareNoCase(_T("一月")) == 0) )
	{
		return 1;
	}
	else if( (s.CompareNoCase(_T("Feb")) == 0) ||
		(s.CompareNoCase(_T("二月")) == 0) )
	{
		return 2;
	}
	else if( (s.CompareNoCase(_T("Mar")) == 0) ||
		(s.CompareNoCase(_T("三月")) == 0) )
	{
		return 3;
	}
	else if( (s.CompareNoCase(_T("Apr")) == 0) ||
		(s.CompareNoCase(_T("四月")) == 0) )
	{
		return 4;
	}
	else if( (s.CompareNoCase(_T("May")) == 0) ||
		(s.CompareNoCase(_T("五月")) == 0) )
	{
		return 5;
	}
	else if( (s.CompareNoCase(_T("Jun")) == 0) ||
		(s.CompareNoCase(_T("六月")) == 0) )
	{
		return 6;
	}
	else if( (s.CompareNoCase(_T("Jul")) == 0) ||
		(s.CompareNoCase(_T("七月")) == 0) )
	{
		return 7;
	}
	else if( (s.CompareNoCase(_T("Aug")) == 0) ||
		(s.CompareNoCase(_T("八月")) == 0) )
	{
		return 8;
	}
	else if( (s.CompareNoCase(_T("Sep")) == 0) ||
		(s.CompareNoCase(_T("九月")) == 0) )
	{
		return 9;
	}
	else if( (s.CompareNoCase(_T("Oct")) == 0) ||
		(s.CompareNoCase(_T("十月")) == 0) )
	{
		return 10;
	}
	else if( (s.CompareNoCase(_T("Nov")) == 0) ||
		(s.CompareNoCase(_T("十一月")) == 0) )
	{
		return 11;
	}
	else if( (s.CompareNoCase(_T("Dec")) == 0) ||
		(s.CompareNoCase(_T("十二月")) == 0) )
	{
		return 12;
	}

	return 0;
}