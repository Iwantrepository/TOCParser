��������� TOCParser.exe (x86/x64) ���������� � Microsoft Windows Vista x86/x64, Windows 7 x86/x64, Windows 8 x86/x64, Windows 8.1 x86/x64, Windows 10 x86/x64.

��� ������� � Microsoft Visual Studio 2017 Version 15.5.7

������������:
1. Visual C++
2. STL

��������� ������ Microsoft Visual Studio 2017 ��� ��������� �������� �����:
https://www.visualstudio.com/downloads/

�������� ��� ��������� TOCParser:
Main.cpp
TOCParser.cpp
TOCParser.h

����� Visual Studio �������:
TOCParser.sln � Visual Studio Solution
TOCParser.vcxproj � VC++ Project
TOCParser.vcxproj.filters � V�++ Project Filters File
TOCParser.vcxproj.user � Per-User Project Options File

�TOCParser\x64\Release\TOCParser.exe� � 64 bit ��������� ��� Windows x64
�TOCParser\Release\TOCParser.exe� � 32 bit ��������� ��� Windows x86 � Windows x64 WOW (Windows x86 On Windows x64)

��� ������� ���������:
0. ���������� txt ����� �� ����� �samples� � �TOCParser\x64\Release� �/��� �TOCParser\Release�.
1. ��������� �������. ����������� �cmd� � Run.
2. ��������� ������� �cd� ��������� � ����� � �TOCParser\x64\Release� � 64bit Windows ��� �TOCParser\Release� � 32bit Windows.
3. ���������� � ��������� ������ �TOCParser.exe 0202.txt� � ������� Enter.

��������� TOCParser ������� �� ����� ��� ������:
1.	******************** ���������� ********************
�����������/��������������� (parsed) � ������ ��������� ������ ����� �� ����� ����� �����������, � ����������� �������� � ������� ��������� � ����� ��� ������ �� �����.
2.	******************** ��������� � ������ ********************
������� ����������� ��������� � ������

��� ���������� �������, �������� � ���������� ������������� ���������� �������:
1. � ��������� ������� ����������� ������� 'C' - Class (class CTOCParser)
2. � ��������� �������� ����������� ������� 'S' - Struct  (struct SContent)
3. � ���������� ���� 'char*' � 'char[]' ����������� ������� 'sz' - String Zero (char szBuf[100])
4. � ���������� ���� 'int' ����������� ������� 'n' (int nBrokenLines)
5. � ���������� ���� 'bool' ����������� ������� 'b' (bool bInContents)
6. � ���������� ���� 'string' ����������� ������� 'str' (string strBrokenLine)
7. � ���������� ���� 'vector' ����������� ������� 'v' (vector<int> vTitleNumber)
8. � ���������� ������� ����������� ������� 'm_' - Member Variable (string m_strText)
9. � ���������� ������� '&' ����������� ������� 'r' - Reference (SContent& rContent)
9. � ���������� ���������� '*' ����������� ������� 'p' - Pointer (SContent* pContent)
https://ru.wikipedia.org/wiki/����������_�������
