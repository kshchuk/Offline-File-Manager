#pragma once
#include<QString>
#include<QList>


QList<QString> removableDrives();

#if defined(_WIN32)
#include <Windows.h>

LPCWSTR drive2[26] = { L"A:\\", L"B:\\", L"C:\\", L"D:\\", L"E:\\", L"F:\\",
						L"G:\\", L"H:\\",L"I:\\", L"J:\\", L"K:\\", L"L:\\",
						L"M:\\", L"N:\\",L"O:\\", L"P:\\", L"Q:\\", L"R:\\",
						L"S:\\", L"T:\\",L"U:\\", L"V:\\", L"W:\\", L"X:\\",
						L"Y:\\", L"Z:\\" };

QList<QString> removableDrives()
{
	QList<QString> drives;
	for (int i = 0; i < 26; i++)
	{
		UINT test = GetDriveType(drive2[i]);
		if (test == DRIVE_REMOVABLE || test == DRIVE_REMOTE ||
			test == DRIVE_CDROM)
		{
#ifdef UNICODE
			drives.append(QString::fromWCharArray(drive2[i]));
#else
			drives.append(QString::fromLocal8Bit(drive2[i]));
#endif
		}
	}
}

#endif
#if defined(unix) || defined(__unix__) || defined(__unix)

// TODO: Get removable drives at unix device

#endif
#if defined(__APPLE__)

// TODO: Get removable drives at apple device

#endif