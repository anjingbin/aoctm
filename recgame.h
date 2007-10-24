// Recgame.h: CRecgame 
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "db/persistentinterface.h"

#define	TIME_4_INCOMPLETE  (20 * 60)		// as seconds. if game time is less than 20 min, then consider is as an incomplete one.

extern CString civ_name[19];
extern COLORREF player_color[9];

#if 1
class CMapInfo
{	
public:
	//huangjie 07-08-03, default constructor
	CMapInfo(){}
	//huangjie 07-08-03, copy constructor and operator =
	CMapInfo(CMapInfo&);
	CMapInfo& operator = (CMapInfo&);

	CString	Name;									//��ͼ������
	int	MapSizeLevel;								//��ͼ��С
	int	HardLevel;									//�Ѷ�
	int	Victory;									//ʤ������
	bool FullTechTree;								//��ȫ�Ƽ���
};	

class CPlayerInGame
{	
public:
	//pubb, 07-08-03, add for assignment of CPlayerInGame
	CPlayerInGame(void);
	CPlayerInGame(CPlayerInGame & player);			//copy construction
	//huangjie, 07-08-03, operator = 
	CPlayerInGame& operator = (CPlayerInGame&);

	CString	Name;									//�������
	int	Team;										//���
	int	Civ;										//����ID
	int	Color;										//��ɫID
	int	PositionX;									//���λ��
	int	PositionY;									//���λ��

	CTimeSpan	FeudTime;								//����ʱ��
	CTimeSpan	CstlTime;								//����ʱ��
	CTimeSpan	ImplTime;								//����ʱ��
	CTimeSpan	ResignTime;								//Ͷ��ʱ��

	/* pubb, 07-08-02, copy from original, not sure for their meanings */
	char		name_key[256];
	int			number_key;
};					

class CChatInfo
{	
public:
	//huangjie, 07-08-03, copy constructor and operator = 
	CChatInfo(CChatInfo&);
	CChatInfo& operator = (CChatInfo&);

	CChatInfo(unsigned char *chat, CTimeSpan time);
	CChatInfo(void);

	//07-10-20, pubb, change to store ID instead of name
	//CString Name;
	int	Player_Num;									//˵����
	CTimeSpan	Time;									//˵��ʱ��
	int	Target;										//˵������0-��ͨ��1-ȫ�壨��*������2-ͬ�飨����������3-�Է�����!��������ʱû����
	CString	Content;								//˵������
};

class CRecgame
{	
	//huangjie, 07-08-03, hide copy constractor and operator =  
	CRecgame(CRecgame&);
	CRecgame& operator = (CRecgame&);
public:
	CRecgame();
	~CRecgame();

	int	ID;											//¼��Ψһ��ʶ
	CString	FileName;								//¼���ļ�����CString����
	CTime	RecordTime;								//¼������ʱ��
	//pubb, 07-08-03, an interval of time should be a CTimeSpan. the same changes to other Time Interval members
	//CTime	PlayTime;
	CTimeSpan	PlayTime;								//¼�����ʱ��
	//pubb, 07-08-02, need a player num				
	int		PlayerNum;								//�����

	CArray<CPlayerInGame, CPlayerInGame &>	Players;						//����¼�������Ϣ, ��1����GAIA����Ȼ��
	//pubb, 07-08-02
	//CString	ViewerName;								
	int	ViewerID;									//���ӽ����
	CMapInfo	Map;								//��ͼ��Ϣ
	//pubb, 07-08-02, it has to be points newed at time
	//CArray<CChatInfo, CChatInfo&>	ChatB4Game;		//��Ϸ��ʼǰ��������Ϣ
	//CArray<CChatInfo, CChatInfo&>	ChatInGame;		//��Ϸ�е�������Ϣ
	CArray<CChatInfo *, CChatInfo *> ChatB4Game;
	CArray<CChatInfo *, CChatInfo *>	ChatInGame;

	bool	Loaded;

public:
	bool	Read(CString file);
	bool	Load(class IPersistentInterface * engine);
	bool	Save(class IPersistentInterface * engine);
	bool	LoadChatInfo(class IPersistentInterface * engine);
	/* pubb, 07-07-26 */
	int		GetWinnerTeam(void);
	bool	IsWinner(int player_id);

private:
	/* pubb, 07-08,02, copy from original recgame structure */
	/*
	CString m_feudal_msg;
	CString m_castle_msg;
	CString m_imperial_msg;
	CString m_resign_msg;
	
	CString m_str_table[100];
	*/

	//pubb, 07-10-24, to specify 'RecordTime from file modified time', that'll be subbed by PlayTime
	bool	bSpecialRecordTime;

	SIZE		map_size;
	int			map_load_flg;
	char		map_data[57600];
	int			data_ref[9];
	bool		player_resign[9];
	unsigned char * m_pt_header;
	unsigned char * m_pt_body;
	unsigned long m_body_len;
	unsigned long m_header_len;

	void getGameData(void);
	//pubb, 07-08-02, no use
	/*
	void getBody(unsigned char ** pt_body, unsigned int* length);
	void getHeader(unsigned char ** pt_header, unsigned int* length);
	*/

	//pubb, 07-08-11, clear memory of data not stored into DB
	void ClearMem(void);

	int readFile(CString filename);
	
	/* pubb, 07-08-02, no need now. we have our own method to deal with chat msg */
	//CString AnalyzeChat(CString chat);
	
	/* pubb, 07-07-25, add */
	/* pubb, 07-07-31, move to 'private' */
	void	FillWinner(void);
	//07-10-20, pubb, add for more accurate winner msg
	//07-10-21, pubb, return void instead of int
	void	SetResignFromChat(void);
	/* pubb, 07-08-02, construct CTime object */
	CTimeSpan	Timecnt2CTimeSpan(int timecnt);
	
};			

#else

typedef struct player_info_s {
	char		name_key[256];
	int			number_key;
	int			player_civ;
	int			player_color;
	int			feudal_time;
	int			castle_time;
	int			imperial_time;
} player_info;

class CRecgame  
{
public:
	/* pubb, 07-07-26, merged from rec_info */
    CString		file_name;
    CString		file_ext;
    ULONG		file_attr;
    SYSTEMTIME	write_time;
    long		file_size;
	int			icon_idx;
	int			map_id;
    CString		map_name;
	SIZE		map_size;
	int			map_load_flg;
	char		map_data[57600];
	int			play_time;
	int			rec_player;
	int			player_num;
    CString		team_num;
	int			data_ref[9];
	player_info player_data[9];
    int			player_team[9];
    CString		player_name[9];

	/* pubb, 07-07-25
	 * for winner/loser information
	 */
	bool		player_resign[9];

	CString		chat_b4_game;
	CString		chat_in_game;
	/* end of rec_info */

	CString m_feudal_msg;
	CString m_castle_msg;
	CString m_imperial_msg;
	CString m_resign_msg;
	CString m_str_table[100];
	void getGameData(void);
	void getBody(unsigned char ** pt_body, unsigned int* length);
	void getHeader(unsigned char ** pt_header, unsigned int* length);
	int readFile(CString filename);
	
	/* pubb, 07-07-24 */
	CString GetAOCRegistry(void);
	/* pubb, 07-07-26 */
	int		GetWinnerTeam(void);

	CRecgame();
	virtual ~CRecgame();

private:
	CString AnalyzeChat(CString chat);
	unsigned char * m_pt_header;
	unsigned char * m_pt_body;
	unsigned long m_body_len;
	unsigned long m_header_len;
	/* pubb, 07-07-25, add */
	/* pubb, 07-07-31, move to 'private' */
	void	FillWinner(void);

};
#endif