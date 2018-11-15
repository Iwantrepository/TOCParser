#pragma once

///////////////////////////////////////////////////////////////////////////
// �������������� ���������� ����������
//
// ��������� ��������������� ����������� ����������
// 1. ���������� ������ ���������� � ��������� ����� - '����������'
// 2. ���������� ������ ������������� �� ������ ��� ������ �������� � ����� ������

#include <string>	// ������������ string STL
#include <vector>	// ������������ vector STL

using namespace std;

// ����� - ����������
class CTOCParser
{
public:
	// ����������� �� ��������� ������ ����������
	CTOCParser();

	// ����������� c ��������� � ��������
	// ������� ��������
	// const string& strFilePath - ���� �� txt ����
	CTOCParser(const string& strFilePath);

	// ���������� �� ��������� ������ ����������
	~CTOCParser();

	// ����� �������� �����
	// ������� ��������
	// const string& strFilePath - ���� �� txt ����
	void LoadFile(const string& strFilePath);

	// ����� ������� ����������
	void Parse();

	// ����� ������ �� ������� ������� ����������
	void PrintContents() const;

	// ����� ������ �� ������� ��������� � ������
	void PrintRelationshipsAndLinks() const;

private:
	typedef vector<string> vecText;
	typedef vector<int> vecTitleNumber;
	typedef vector<int> vecChildren;

	// ��������� ������ - ����������
	struct SContent
	{
		SContent()
			: nPageNumber(-1)
			, nParent(-1)
			, nStartLine(-1)
			, nEndLine(-1)
		{}
		vecTitleNumber vTitleNumber;	// ����� �������
		string strTitleName;			// �������� �������
		int nPageNumber;				// ����� ��������
		int nParent;					// ������ �� ������������ ������
		vecChildren vChildren;			// ������ �� �������� �������
		int nStartLine;					// ����� ������ ������ �������
		int nEndLine;					// ����� ������ ����� �������
	};
	typedef vector<SContent> vecContents;	// ����������� ������� ���� - ����������

	// ����� �������� ������ � ��������� ������
	void ParseLine(const string& strLine, SContent& rContent) const;

	// ����� ����������� ������ ����������
	void ParseTitleNumber(const string& strTitleNumber, vecTitleNumber& vTitleNumber) const;

	// ����� ����������� ���������
	void BuildRelationships();

	// ����� ����������� ������
	void BuildLinks();

	// ����� ����������� ������
	bool IsChild(const SContent& rParent, const SContent& rChild) const;

	// ����� ����������� ��������
	bool IsParent(const SContent& rParent, const SContent& rChild) const;

	string m_strText;			// txt ����
	int m_nStartContentLine;	// ����� ������ ������ ����������
	int m_nEndContentLine;		// ����� ������ ����� ����������
	vecText m_vLines;			// ������ - ������
	vecContents m_vContents;	// ������ - ����������
};
