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

class CPayedFee
{
public:
	int	Money;					//���ν��ɽ��
	CTime	PayTime;			//���νɷ�ʱ��
};

class CPlayer
{
	//huangjie, 07-08-03, hide copy constractor and operator =  
	CPlayer(CPlayer&);
	CPlayer& operator = (CPlayer&);
public:
	CPlayer(void);
	~CPlayer(void);

	int	ID;						//���Ψһ��ʶ
	//pubb, 07-08-02, change variable name
	//CStringArray	NickName;
	CStringArray	NickNames;	//���ID���ɶ��
	int InitRating;				//��ҳ�ʼ�趨�Ļ���
	int	Rating;					//��ҵ�ǰ����
	int	PlayCount;				//���򳡴��ܼ�
	int	WinCount;				//��Ӯ�����ܼ�
	CTime	UpdateTime;			//�������ʱ��
	int	Fee;					//��ҵ�ǰʣ����

	//by mep
	//for technic statistic
	int MinFeud;
	int AvgFeud;
	int MinCstl;
	int AvgCstl;
	int MinImpl;
	int AvgImpl;
	//����ʹ�õ�������ÿһ���ʾ��������ʹ��һ��
	//255��ʾ�Ǹ�������
	int Civs[19];

	//pubb,07-08-02, change to pointer for better construction
	//CArray<CPayedFee, CPayedFee&> PayedFee;	//��ҽɷѼ�¼
	CArray<CPayedFee , CPayedFee &>	Record_PayedFee;	//��ҽɷѼ�¼

	bool	Load(class IPersistentInterface * engine);
	bool	Save(IPersistentInterface * engine);
};			

#endif

#define	DEF_RATING	1500
#define	DEF_RATING_STR	_T("1500")