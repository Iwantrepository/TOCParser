Программа TOCParser.exe (x86/x64) совместима с Microsoft Windows Vista x86/x64, Windows 7 x86/x64, Windows 8 x86/x64, Windows 8.1 x86/x64, Windows 10 x86/x64.

Код написан в Microsoft Visual Studio 2017 Version 15.5.7

Использовано:
1. Visual C++
2. STL

Бесплатая версия Microsoft Visual Studio 2017 для студентов доступна здесь:
https://www.visualstudio.com/downloads/

Исходный код программы TOCParser:
Main.cpp
TOCParser.cpp
TOCParser.h

Файлы Visual Studio проекта:
TOCParser.sln – Visual Studio Solution
TOCParser.vcxproj – VC++ Project
TOCParser.vcxproj.filters – VС++ Project Filters File
TOCParser.vcxproj.user – Per-User Project Options File

“TOCParser\x64\Release\TOCParser.exe” – 64 bit программа для Windows x64
“TOCParser\Release\TOCParser.exe” – 32 bit программа для Windows x86 и Windows x64 WOW (Windows x86 On Windows x64)

Для запуска программы:
0. Скопируйте txt файлы из папки “samples” в “TOCParser\x64\Release” и/или “TOCParser\Release”.
1. Запустите консоль. Напечатайте “cmd” в Run.
2. Используя команду “cd” перейдите в папку с “TOCParser\x64\Release” в 64bit Windows или “TOCParser\Release” в 32bit Windows.
3. Напечатате в командной строке “TOCParser.exe 0202.txt” и нажмите Enter.

Программа TOCParser выведет на экран две секции:
1.	******************** Содержание ********************
Разобранный/сосканированный (parsed) в массив структуры данных текст из части файла “Содержание”, с восстнаными строками и обратно собранный в текст для вывода на экран.
2.	******************** Отношения и ссылки ********************
Таблицу родственных отношений и ссылки

Для именования классов, структур и переменных использованна Венгерская нотация:
1. К названиям классов добавляется префикс 'C' - Class (class CTOCParser)
2. К названиям структур добавляется префикс 'S' - Struct  (struct SContent)
3. К переменным типа 'char*' и 'char[]' добавляется префикс 'sz' - String Zero (char szBuf[100])
4. К переменным типа 'int' добавляется префикс 'n' (int nBrokenLines)
5. К переменным типа 'bool' добавляется префикс 'b' (bool bInContents)
6. К переменным типа 'string' добавляется префикс 'str' (string strBrokenLine)
7. К переменным типа 'vector' добавляется префикс 'v' (vector<int> vTitleNumber)
8. К переменным классов добавляется префикс 'm_' - Member Variable (string m_strText)
9. К переменным ссылкам '&' добавляется префикс 'r' - Reference (SContent& rContent)
9. К переменным указателям '*' добавляется префикс 'p' - Pointer (SContent* pContent)
https://ru.wikipedia.org/wiki/Венгерская_нотация
