#include "TOCParser.h"

#include <iostream>	// подсоединяем iostream STL
#include <fstream>	// подсоединяем iostream STL
#include <sstream>	// подсоединяем sstream STL

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Функции помощники

// сравнить две строки без регистра
bool iequals(const string& a, const string& b)
{
	if (a.size() != b.size())
		return false;

	for (unsigned int i = 0; i < a.size(); i++)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}

// убрать символы (пробелы) слева
string trimleft(string str, const string strTrim = " ")
{
	str.erase(0, str.find_first_not_of(strTrim));
	return str;
}

// убрать символы (пробелы) справа
string trimright(string str, const string strTrim = " ")
{
	str.erase(str.find_last_not_of(strTrim) + 1);
	return str;
}

// убрать символы (пробелы) слева и справа
string trim(string str, const string strTrim = " ")
{
	str = trimleft(str, strTrim);
	str = trimright(str, strTrim);
	return str;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Класс CTOCParser

CTOCParser::CTOCParser()
	: m_nStartContentLine(-1)
	, m_nEndContentLine(-1)
{
}

CTOCParser::CTOCParser(const string& strFilePath)
	: m_nStartContentLine(-1)
	, m_nEndContentLine(-1)
{
	LoadFile(strFilePath);
	Parse();
}

CTOCParser::~CTOCParser()
{
}

void CTOCParser::LoadFile(const string& strFilePath)
{
	m_strText.clear();	// очищаем старую строку

	cout << "Открываю файл: " << strFilePath << endl;

	ifstream ifs;	// создаём объект потокового файла
	ifs.open(strFilePath);	// открываем файл

	if (!ifs.is_open())	// проверяем, открылся или нет файл
		throw runtime_error("Ошибка открытия файла: " + strFilePath);	// генерируем исключение

	// определяем размер файла
	ifs.seekg(0, ios::end);	// перемещаем файловый указатель в конец файла
	size_t nFileSize = (size_t)ifs.tellg();	// размер файла
	ifs.seekg(0, ios::beg);	// перемещаем файловый указатель в начало файла

	m_strText.reserve(nFileSize);	// резервируем строчку под размер файла
	m_strText.assign((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());	// читаем файл в строчку

	ifs.close();	// закрываем файл

	cout << "Файл прочитан успешно" << endl;
	cout << "Размер файла " << nFileSize << " байт" << endl;
}

void CTOCParser::Parse()
{
	m_nStartContentLine = -1;
	m_nEndContentLine = -1;
	m_vLines.clear();	// очищаем старый массив строк
	m_vContents.clear();	// очищаем старый массив Содержания

	if (m_strText.empty())
		throw runtime_error("Ошибка - Пустой текст");	// генерируем исключение

	cout << "Анализирую файл... "<< endl;

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Алгоритм восстановления сломанных строк

	// Пример из двух строк (сломанных):
	//Зачем нужны
	//компьютерные сети? ................................ 18

	// Объединяем две строки в одну
	//Зачем нужны компьютерные сети? ................................ 18

	const int nMaxBrokenLines = 1;
	int nBrokenLines = 0;
	int nTotalBrokenLines = 0;
	string strBrokenLine;

	int nLine = 0;
	string strLine;

	// создадим поток из строчки для использования метода getline
	stringstream strstr(m_strText);

	while (getline(strstr, strLine))	// перебираем все строчки
	{
		m_vLines.push_back(strLine);
		nLine++;

		strLine = trim(strLine);

		if (strLine.empty())
			continue;

		if (m_nStartContentLine == -1)
		{
			// Содержание должно начинаться с ключевого слова - 'Содержание'
			if (iequals(strLine, "Содержание"))
			{
				m_nStartContentLine = nLine;	// найдено начало Содержания
				continue;
			}
		}
		else
		if (m_nEndContentLine == -1)
		{
			// Содержание должно заканчиваться со строки без номера страницы в конце строки
			basic_string<char>::size_type index = strLine.find_last_not_of("0123456789");
			if (index == string::npos || index == strLine.size() - 1)
			{
				if (nBrokenLines == nMaxBrokenLines)
				{
					nTotalBrokenLines -= nMaxBrokenLines;
					m_nEndContentLine = nLine - nMaxBrokenLines;	// найден конец Содержания
					continue;
				}

				nBrokenLines++;
				nTotalBrokenLines++;
				strBrokenLine += strLine;
			}
			else
			{
				if (nBrokenLines > 0)
				{
					strBrokenLine += ' ';
					strBrokenLine += strLine;	// соединяем сломанные строки
					strLine = strBrokenLine;
					nBrokenLines = 0;
					strBrokenLine.clear();
				}
			}

			if (nBrokenLines == 0)
			{
				SContent Content;
				ParseLine(strLine, Content);
				m_vContents.push_back(Content);	// добавляем в массив
			}
		}
	}

	if (m_vContents.empty())
		throw runtime_error("Ошибка - Содержание не найдено");	// генерируем исключение

	BuildRelationships();
	BuildLinks();

	cout << "Найдено " << m_vLines.size() << " строк и " << m_vContents.size() << " оглавлений" << endl;

	if (nTotalBrokenLines > 0)
		cout << "Востановленно " << nTotalBrokenLines << " сломанных строк" << endl;

	cout << "Номер строки начала Оглавления " << m_nStartContentLine << endl;
	cout << "Номер строки конца Оглавления " << m_nEndContentLine << endl;
}

void CTOCParser::ParseLine(const string& strLine, SContent& rContent) const
{
	// ищем и отделяем номер раздела
	basic_string<char>::size_type index = strLine.find_first_not_of("0123456789.");
	if (index > 0 && index < strLine.size())
	{
		const string strTitleNumber = strLine.substr(0, index);

		ParseTitleNumber(strTitleNumber, rContent.vTitleNumber);

		rContent.strTitleName = strLine.substr(index);
		rContent.strTitleName = trimleft(rContent.strTitleName, " \t");
	}
	else
		rContent.strTitleName = strLine;

	// ищем и отделяем номер страницы
	index = rContent.strTitleName.find_last_not_of("0123456789");
	if (index > 0 && index < rContent.strTitleName.size() - 1)
	{
		const string strPageNumber = rContent.strTitleName.substr(index + 1);
		rContent.nPageNumber = stoi(strPageNumber);

		rContent.strTitleName = rContent.strTitleName.substr(0, index);
		rContent.strTitleName = trimright(rContent.strTitleName, " \t.");
	}
}

void CTOCParser::ParseTitleNumber(const string& strTitleNumber, vecTitleNumber& vTitleNumber) const
{
	vTitleNumber.clear();

	if (strTitleNumber.empty())
		return;

	basic_string<char>::size_type index1 = 0;
	basic_string<char>::size_type index2 = string::npos;

	while (true)
	{
		index2 = strTitleNumber.find('.', index1);
		if (index2 == string::npos)
			break;

		const string strSubTitleNumber = strTitleNumber.substr(index1, index2 - index1);

		index1 = index2 + 1;

		int nSubTitleNumber = stoi(strSubTitleNumber);
		vTitleNumber.push_back(nSubTitleNumber);
	}
}

bool CTOCParser::IsChild(const SContent& rParent, const SContent& rChild) const
{
	if (rParent.vTitleNumber.empty())
		return false;
	if (rChild.vTitleNumber.empty())
		return false;
	if (rChild.vTitleNumber.size() < rParent.vTitleNumber.size())
		return false;

	for (unsigned int i = 0; i < rParent.vTitleNumber.size() && i < rChild.vTitleNumber.size(); i++)
	{
		int nParent = rParent.vTitleNumber[i];
		int nChild = rChild.vTitleNumber[i];

		if (nParent != nChild)
			return false;
	}

	return rParent.vTitleNumber.size() != rChild.vTitleNumber.size();
}

bool CTOCParser::IsParent(const SContent& rParent, const SContent& rChild) const
{
	if (rParent.vTitleNumber.empty())
		return false;
	if (rChild.vTitleNumber.empty())
		return false;
	if (rChild.vTitleNumber.size() > rParent.vTitleNumber.size())
		return false;

	for (unsigned int i = 0; i < rParent.vTitleNumber.size() && i < rChild.vTitleNumber.size(); i++)
	{
		int nParent = rParent.vTitleNumber[i];
		int nChild = rChild.vTitleNumber[i];

		if (nParent != nChild)
			return false;
	}

	return rParent.vTitleNumber.size() != rChild.vTitleNumber.size();
}

void CTOCParser::BuildRelationships()
{
	for (unsigned int i = 0; i < m_vContents.size(); i++)
	{
		SContent& rParent = m_vContents[i];

		for (unsigned int j = 0; j < m_vContents.size(); j++)
		{
			if (i == j)
				continue;

			const SContent& rChild = m_vContents[j];

			if (IsParent(rParent, rChild))
				rParent.nParent = j;

			if (IsChild(rParent, rChild))
				rParent.vChildren.push_back(j);
		}
	}
}

void CTOCParser::BuildLinks()
{
	SContent* pPrevContent = NULL;

	for (unsigned int i = 0; i < m_vContents.size(); i++)
	{
		SContent& rContent = m_vContents[i];

		string strTitle;
		string strTitleNumber;

		for (unsigned int j = 0; j < rContent.vTitleNumber.size(); j++)
		{
			int nSubTitleNumber = rContent.vTitleNumber[j];

			strTitleNumber += to_string(nSubTitleNumber);
			strTitleNumber += '.';
		}

		if (!strTitleNumber.empty())
			strTitle = strTitleNumber;
		else
			strTitle = rContent.strTitleName;

		if (strTitle.empty())
			continue;

		for (unsigned int j = m_nEndContentLine; j < m_vLines.size(); j++)
		{
			const string& rstrLine = m_vLines[j];

			basic_string<char>::size_type pos = rstrLine.find(strTitle);
			if (pos == 0)
			{
				rContent.nStartLine = j;

				if (pPrevContent)
					pPrevContent->nEndLine = j - 1;

				pPrevContent = &rContent;
				break;
			}
		}
	}

	if (pPrevContent)
		pPrevContent->nEndLine = (int)m_vLines.size() - 1;
}

void CTOCParser::PrintContents() const
{
	cout << endl;
	cout << "******************** Содержание ********************" << endl;
	cout << endl;

	for (unsigned int i = 0; i < m_vContents.size(); i++)
	{
		const SContent& rContent = m_vContents[i];

		// сборка структуры данных обратно в строку
		string strContent;

		string strTitleNumber;

		for (unsigned int j = 0; j < rContent.vTitleNumber.size(); j++)
		{
			int nSubTitleNumber = rContent.vTitleNumber[j];

			strTitleNumber += to_string(nSubTitleNumber);
			strTitleNumber += '.';
		}

		strContent += strTitleNumber;

		if (!strContent.empty())
			strContent += '\t';

		strContent += rContent.strTitleName;

		if (rContent.nPageNumber != -1)
		{
			string strPageNumber = to_string(rContent.nPageNumber);
			if (!strPageNumber.empty())
			{
				strContent += " - ";
				strContent += strPageNumber;
			}
		}

		cout << strContent << endl;
	}
	cout << endl;
}

void CTOCParser::PrintRelationshipsAndLinks() const
{
	cout << endl;
	cout << "******************** Отношения и ссылки ********************" << endl;
	cout << endl;

	cout << "Номер строки\tНомер оглавления\tРодитель\tДети\tНачало раздела\tКонец раздела" << endl;

	const string strNo = "-";

	for (unsigned int i = 0; i < m_vContents.size(); i++)
	{
		const SContent& rContent = m_vContents[i];

		// сборка структуры данных обратно в строку
		string strContent;

		// Номер строки
		strContent = to_string(i);

		// Номер оглавления
		strContent += '\t';

		if (!rContent.vTitleNumber.empty())
		{
			string strTitleNumber;

			for (unsigned int j = 0; j < rContent.vTitleNumber.size(); j++)
			{
				int nSubTitleNumber = rContent.vTitleNumber[j];

				strTitleNumber += to_string(nSubTitleNumber);
				strTitleNumber += '.';
			}

			strContent += strTitleNumber;
		}
		else
			strContent += strNo;

		// Родитель
		strContent += "\t";

		if (rContent.nParent != -1)
		{
			string strParent = to_string(rContent.nParent);
			if (!strParent.empty())
				strContent += strParent;
		}
		else
			strContent += strNo;

		// Дети
		strContent += "\t";

		if (!rContent.vChildren.empty())
		{
			string strChildren;

			for (unsigned int j = 0; j < rContent.vChildren.size(); j++)
			{
				int nChild = rContent.vChildren[j];

				string strChild = to_string(nChild);
				if (!strChild.empty())
				{
					strChildren += strChild;
					strChildren += ' ';
				}
			}
			strChildren = trimright(strChildren);

			strContent += strChildren;
		}
		else
			strContent += strNo;

		// Начало раздела
		strContent += "\t";
		if (rContent.nStartLine != -1)
			strContent += to_string(rContent.nStartLine);
		else
			strContent += strNo;

		// Конец раздела
		strContent += "\t";
		if (rContent.nEndLine != -1)
			strContent += to_string(rContent.nEndLine);
		else
			strContent += strNo;

		cout << strContent << endl;
	}
	cout << endl;
}
