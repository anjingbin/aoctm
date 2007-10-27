#pragma once


#include "recgame.h"
#include "DB/persistentinterface.h"

#if 0
class CPlayer :
	public CObject
{
public:
	CPlayer(void);
	~CPlayer(void);

	CStringArray	m_Names;
	int	m_PlayCount;
	int m_WinCount;
	int m_Rating;
};

#else

class CPaidFee
{
public:
	CPaidFee(void);
	int	Money;					//���ν��ɽ��
	CTime	PayTime;			//���νɷ�ʱ��
};

class CPlayer
{
	//pubb, 07-08-23, no use of copy constructor
	/*
	//huangjie, 07-08-03, hide copy constractor and operator =  
	CPlayer(CPlayer&);
	CPlayer& operator = (CPlayer&);
	*/
public:
	CPlayer(void);
	~CPlayer(void);
	void Initialize(void);

	int	ID;						//���Ψһ��ʶ
	//pubb, 07-08-02, change variable name
	//CStringArray	NickName;
	CStringArray	NickNames;	//���ID���ɶ��
	int InitRating;				//��ҳ�ʼ�趨�Ļ���
	int	Rating;					//��ҵ�ǰ����
	int	PlayCount;				//���򳡴��ܼ�
	int	WinCount;				//��Ӯ�����ܼ�
	CTime	UpdateTime;			//�������ʱ��
	//pubb, 07-10-26, no Fee property now
	//int	Fee;					//��ҵ�ǰʣ����
	bool	IsComputer;			//�ǵ������

	//by mep
	//for technic statistic
	CTimeSpan MinFeudTime;
	CTimeSpan TotalFeudTime;
	int FeudCount;
	CTimeSpan MinCstlTime;
	CTimeSpan TotalCstlTime;
	int CstlCount;
	CTimeSpan MinImplTime;
	CTimeSpan TotalImplTime;
	int ImplCount;
	//����ʹ�õ�������ÿһ���ʾ��������ʹ��һ��
	//255��ʾ�Ǹ�������
	int Civs[19];

	//pubb,07-08-02, change to pointer for better construction
	//CArray<CPaidFee, CPaidFee&> PaidFee;	//��ҽɷѼ�¼
	CArray<CPaidFee * , CPaidFee *>	Record_PaidFee;	//��ҽɷѼ�¼

	int	GetPaidFee(void);
//	int GetPlayCountWithFee(void);

	bool	Load(class IPersistentInterface * engine);
	bool	Save(IPersistentInterface * engine);
};			

#endif

#define	DEF_RATING	1500
#define	DEF_RATING_STR	_T("1500")
#define	MAX_TIMESPAN	CTimeSpan(99999999)

//pubb, 07-10-25, for computer players
#define COMPUTER    (_T("@#-"))