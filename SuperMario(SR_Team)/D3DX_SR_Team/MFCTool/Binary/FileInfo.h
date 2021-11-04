#pragma once

#ifndef __FILEINFO_H_
#define __FILEINFO_H_

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString ConvertRelativePath(CString wstrFileFullPath);

};

#endif // !__FILEINFO_H_
