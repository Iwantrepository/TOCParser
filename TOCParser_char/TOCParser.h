#pragma once

///////////////////////////////////////////////////////////////////////////
// Синтаксический Анализатор Содержания
//
// Допущения Синтаксического анализатора Содержания
// 1. Содержание должно начинаться с ключевого слова - 'Содержание'
// 2. Содержание должно заканчиваться со строки без номера страницы в конце строки

#include <string>	// подсоединяем string STL
#include <vector>	// подсоединяем vector STL

using namespace std;

// класс - Содержание
class CTOCParser
{
public:
	// конструктор по умолчанию класса Содержания
	CTOCParser();

	// конструктор c загрузкой и анализом
	// входной параметр
	// const string& strFilePath - путь на txt файл
	CTOCParser(const string& strFilePath);

	// деструктор по умолчанию класса Содержания
	~CTOCParser();

	// метод загрузки файла
	// входной параметр
	// const string& strFilePath - путь на txt файл
	void LoadFile(const string& strFilePath);

	// метод анализа Оглавления
	void Parse();

	// метод вывода на консоль массива Оглавления
	void PrintContents() const;

	// метод вывода на консоль Отношений и ссылок
	void PrintRelationshipsAndLinks() const;

private:
	typedef vector<string> vecText;
	typedef vector<int> vecTitleNumber;
	typedef vector<int> vecChildren;

	// структура данных - Содержание
	struct SContent
	{
		SContent()
			: nPageNumber(-1)
			, nParent(-1)
			, nStartLine(-1)
			, nEndLine(-1)
		{}
		vecTitleNumber vTitleNumber;	// номер раздела
		string strTitleName;			// название раздела
		int nPageNumber;				// номер страницы
		int nParent;					// ссылка на родительский раздел
		vecChildren vChildren;			// ссылки на дочерние разделы
		int nStartLine;					// номер строки начала раздела
		int nEndLine;					// номер строки конца раздела
	};
	typedef vector<SContent> vecContents;	// определение массива типа - Содержания

	// метод разборки строки в структуру данных
	void ParseLine(const string& strLine, SContent& rContent) const;

	// метод определения номера Оглавления
	void ParseTitleNumber(const string& strTitleNumber, vecTitleNumber& vTitleNumber) const;

	// метод определения Отношений
	void BuildRelationships();

	// метод определения ссылок
	void BuildLinks();

	// метод определения ребёнок
	bool IsChild(const SContent& rParent, const SContent& rChild) const;

	// метод определения родитель
	bool IsParent(const SContent& rParent, const SContent& rChild) const;

	string m_strText;			// txt файл
	int m_nStartContentLine;	// номер строки начала Оглавления
	int m_nEndContentLine;		// номер строки конца Оглавления
	vecText m_vLines;			// массив - строки
	vecContents m_vContents;	// массив - Содержания
};
