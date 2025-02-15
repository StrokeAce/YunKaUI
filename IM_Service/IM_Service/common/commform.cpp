// comm pack commcom.cpp: 数据传输中公用包的实现函数
//
//////////////////////////////////////////////////////////////////////
#include "../StdAfx.h"

#include "../include/tstring.h"
#include <stdlib.h>
#include "../include/comdef.h"
#include "../include/comfunc.h"
#include "../include/commtran.h"

#include "../include/Pack.h"
#include "../include/commcom.h"
#include "../include/commform.h"

//#define CMD_SRV_STATUS_FRDONLINE			0x006E	// 110 通知我我的朋友上线
//服务器告我，我的朋友在线
//CMD_SRV_STATUS_FRDONLINE
SRV_STATUS_FRDONLINE::SRV_STATUS_FRDONLINE(unsigned short ver)
{
	this->version = ver;

	this->ver = 0;
	langtype = LANGUAGE_NORMAL;

	uin = 0;
	_tcscpy(nickname, _T(""));

	ip = 0;
	port = 0;
	status = 0;

	SetVideo(0);
	SetStatus(STATUS_OFFLINE);

	m_Head.head.cmd = CMD_SRV_STATUS_FRDONLINE;

	connecttype=0;
	mpid=0;
	szRand[0]=0;
	szThirdId[0]=0;
}

SRV_STATUS_FRDONLINE::~SRV_STATUS_FRDONLINE()
{
}


bool SRV_STATUS_FRDONLINE::unpack()  //read
{
	//read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> uin))
		goto RETURN;

	if(!(m_Pack.unpackString(nickname,len,MAX_USERNAME_LEN)))
		goto RETURN;

	if(!(m_Pack >> ip))
		goto RETURN;
	if(!(m_Pack >> port))
		goto RETURN;

	if(!(m_Pack >> ver))
		goto RETURN;
	if(!(m_Pack >> langtype))
		goto RETURN;

	if(!(m_Pack >> status))
		goto RETURN;
	if(!(m_Pack.unpackString(strstatus,len,MAX_STATUS_LEN)))
		goto RETURN;

	
	if (!(m_Pack>>sterminaltype))
	{
		sterminaltype=0;
	}

	if (version>=3070)
	{ //需要服务器版本>=3070
		if (!(m_Pack>>connecttype))
		{
			goto RETURN;
		}
		if (!(m_Pack>>mpid))
		{
			goto RETURN;
		}
	}
	
	if (version>=3073)
	{
		if(!m_Pack.unpackString(szRand,len,MAX_WEBCLIENID_LEN))
		{
			szRand[0]=0;
		}
		
		if(!m_Pack.unpackString(szThirdId,len,MAX_THIRDID_LEN))
		{
			szThirdId[0]=0;
		}
	}

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_STATUS_FRDONLINE::pack()  //write
{
	//write
	bool bRtn = false;

	if(!(m_Pack << uin))
		goto RETURN;

	if(!m_Pack.packString(nickname,MAX_USERNAME_LEN))
		goto RETURN;

	if(!(m_Pack << ip))
		goto RETURN;
	if(!(m_Pack << port))
		goto RETURN;

	if(!(m_Pack << ver))
		goto RETURN;
	if(!(m_Pack << langtype))
		goto RETURN;

	if(!(m_Pack << status))
		goto RETURN;
	if(!m_Pack.packString(strstatus,MAX_STATUS_LEN))
		goto RETURN;

	if (!(m_Pack<<sterminaltype))
	{
		goto RETURN;
	}

//	if (version>=3070)
	{ //需要服务器版本>=3070
		if (!(m_Pack<<connecttype))
		{
			goto RETURN;
		}
		if (!(m_Pack<<mpid))
		{
			goto RETURN;
		}
	}

	//	if (version>=3073)
	{
		if(!m_Pack.packString(szRand,MAX_WEBCLIENID_LEN))
			goto RETURN;
		if(!m_Pack.packString(szThirdId,MAX_THIRDID_LEN))
			goto RETURN;
		
	}

	bRtn = true;
RETURN:
	return bRtn;
}

int SRV_STATUS_FRDONLINE::GetStatus()
{
	return GetMutiByte(status, ONLINE_STATUS, ONLINE_STATUS_LEN);
}

void SRV_STATUS_FRDONLINE::SetStatus(int flag)
{
	status = SetMutiByte(status, ONLINE_STATUS, ONLINE_STATUS_LEN, (unsigned short)flag);
}

int SRV_STATUS_FRDONLINE::GetConnectType()
{
	return GetMutiByte(status, ONLINE_CONNECTTYPE, ONLINE_CONNECTTYPE_LEN);
}

void SRV_STATUS_FRDONLINE::SetConnectType(int flag)
{
	status = (unsigned short)SetMutiByte(status, ONLINE_CONNECTTYPE, ONLINE_CONNECTTYPE_LEN, (unsigned short)flag);
}

int SRV_STATUS_FRDONLINE::GetClientType()
{
	return GetMutiByte(status, ONLINE_USERTYPE, ONLINE_USERTYPE_LEN);
}

void SRV_STATUS_FRDONLINE::SetClientType(int flag)
{
	status = SetMutiByte(status, ONLINE_USERTYPE, ONLINE_USERTYPE_LEN, (unsigned short)flag);
}

int SRV_STATUS_FRDONLINE::GetVideoType()
{
	return GetMutiByte(status, ONLINE_VIDEO, ONLINE_VIDEO_LEN);
}

void SRV_STATUS_FRDONLINE::SetVideo(int type)
{
	status = SetMutiByte(status, ONLINE_VIDEO, ONLINE_VIDEO_LEN, (unsigned short)type);
}

//#define CMD_SRV_STATUS_FRDOFFLINE			0x0078	// 120 通知我我的朋友离线
//服务器告我， 我的朋友离线
SRV_STATUS_FRDOFFLINE::SRV_STATUS_FRDOFFLINE(unsigned short ver)
{
	this->version = ver;

	this->uin = 0;

	this->m_Head.head.cmd = CMD_SRV_STATUS_FRDOFFLINE;
}

SRV_STATUS_FRDOFFLINE::~SRV_STATUS_FRDOFFLINE()
{
}


bool SRV_STATUS_FRDOFFLINE::unpack()  //read
{
	//read
	bool bRtn = false;

	if(!(m_Pack >> uin))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_STATUS_FRDOFFLINE::pack()  //write
{
	//write
	bool bRtn = false;

	if(!(m_Pack << uin))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}


//#define CMD_SRV_SEARCH_FAIL					0x0096	// 150 查询朋友失败
SRV_SEARCH_FAIL::SRV_SEARCH_FAIL(unsigned short ver)
{
	this->version = ver;
	
	seq = 0;
	type = 0;
	_tcscpy(reason, _T(""));

	this->m_Head.head.cmd = CMD_SRV_SEARCH_FAIL;
}

SRV_SEARCH_FAIL::~SRV_SEARCH_FAIL()
{
	;
}

bool SRV_SEARCH_FAIL::unpack()
{
	//read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> type))
		goto RETURN;

	if(!(m_Pack.unpackString(this->reason, len, MAX_RESPONSE_LEN)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_SEARCH_FAIL::pack()
{
	//write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;
	if(!(m_Pack << type))
		goto RETURN;

	if(!(m_Pack.packString(this->reason, MAX_RESPONSE_LEN)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//更新失败
SRV_UPDATE_FAIL::SRV_UPDATE_FAIL(unsigned short ver)
{
	this->version = ver;

	seq = 0;
	this->type = UPDATE_STATUS;
	uin = 0;

	this->deny = 0;
	_tcscpy(reason, _T(""));

	this->m_Head.head.cmd = CMD_SRV_UPDATE_FAIL;
}

SRV_UPDATE_FAIL::~SRV_UPDATE_FAIL()
{
}


bool SRV_UPDATE_FAIL::unpack()  //read
{
	//read
	bool bRtn = false;
	unsigned short len = 0;
	
	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> type))
		goto RETURN;
	if(!(m_Pack >> uin))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_UPDATE_FAIL::pack()  //write
{
	//write
	bool bRtn = false;
	
	if(!(m_Pack << seq))
		goto RETURN;
	if(!(m_Pack << type))
		goto RETURN;
	if(!(m_Pack << uin))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_SRV_STATUS_USER_FORM					0x01A4	// 420 用户在线状态
SRV_STATUS_USER_FORM::SRV_STATUS_USER_FORM(unsigned short version)
{
	this->version = version;
	this->type = 0;
	this->uin = 0;
	this->status = 0;
	_tcscpy(strstatus, _T(""));

	this->ip = 0;
	this->port = 0;
	this->m_Head.head.cmd = CMD_SRV_STATUS_USER_FORM;
}

SRV_STATUS_USER_FORM::~SRV_STATUS_USER_FORM()
{
}


bool SRV_STATUS_USER_FORM::unpack() //read
{
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> type))
		goto RETURN;
	if(!(m_Pack >> uin))
		goto RETURN;
	
	if(!(m_Pack >> status))
		goto RETURN;
	if(!m_Pack.unpackString(strstatus,len,MAX_STATUS_LEN))
		goto RETURN;

	if(!(m_Pack >> ip))
		goto RETURN;
	if(!(m_Pack >> port))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_STATUS_USER_FORM::pack() //write
{
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack << type))
		goto RETURN;
	if(!(m_Pack << uin))
		goto RETURN;
	
	if(!(m_Pack << status))
		goto RETURN;
	if(!m_Pack.packString(strstatus,MAX_STATUS_LEN))
		goto RETURN;

	if(!(m_Pack << ip))
		goto RETURN;
	if(!(m_Pack << port))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

int SRV_STATUS_USER_FORM::GetOnlineStatus()
{
	return GetMutiByte(status, ONLINE_STATUS, ONLINE_STATUS_LEN);
}


int SRV_STATUS_USER_FORM::GetConnectType()
{
	return GetMutiByte(status, ONLINE_CONNECTTYPE, ONLINE_CONNECTTYPE_LEN);
}

int SRV_STATUS_USER_FORM::GetUserType()
{
	return GetMutiByte(status, ONLINE_USERTYPE, ONLINE_USERTYPE_LEN);
}

/*
//0x0244	// 580 用户登录发送的包结束
//CMD_SRV_FINISH_CLTLOGON
SRV_FINISH_CLTLOGON::SRV_FINISH_CLTLOGON(unsigned short ver)
{
	this->version = ver;

	this->seq = 0;
	this->ip = 0;
	this->port = 0;

	this->m_Head.head.cmd = CMD_SRV_FINISH_CLTLOGON;
}

SRV_FINISH_CLTLOGON::~SRV_FINISH_CLTLOGON()
{
}


bool SRV_FINISH_CLTLOGON::unpack()  //read
{
	bool bRtn = false;
	
	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> ip))
		goto RETURN;
	if(!(m_Pack >> port))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool SRV_FINISH_CLTLOGON::pack()  //write
{
	bool bRtn = false;
	if(!(m_Pack << seq))
		goto RETURN;
		
	if(!(m_Pack << ip))
		goto RETURN;
	if(!(m_Pack << port))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}
*/

//客户端的包的处理
//#define CMD_CLT_SEND_ALLMSG					0x03F2  //  1010 发送圈子呼消息
//发送圈子呼留言
CLT_SEND_ALLMSG::CLT_SEND_ALLMSG(unsigned short ver)
{
	this->version = ver;
	num = 0;

	for(int i = 0; i < MAX_LIST_UIN; i ++)
	{
		this->listuin[i] = 0;
	}
	
	maxnum = MAX_LIST_UIN;

	this->m_Head.head.cmd = CMD_CLT_SEND_ALLMSG;
}

CLT_SEND_ALLMSG::~CLT_SEND_ALLMSG()
{
}

bool CLT_SEND_ALLMSG::unpack()  //read
{
	//read
	bool bRtn = false;
	int i;

	if(!(m_Pack >> num))
		goto RETURN;

	if(num > maxnum)
		goto RETURN;

	for(i=0; i < num ; i++)
	{
		if(!(m_Pack >> listuin[i]))
			goto RETURN;
	}

	if(!(msg.unpack(m_Pack, version)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool CLT_SEND_ALLMSG::pack()  //write
{
	//<<write
	bool bRtn = false;
	int i;

	if(num > maxnum)
		num = maxnum;

	if(!(m_Pack << num))
		goto RETURN;

	for(i=0; i < num ; i++)
	{
		if(!(m_Pack << listuin[i]))
			goto RETURN;
	}

	if(!(msg.pack(m_Pack, version)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//#define CMD_CLT_GET_CLT_INFO				0x0460  // 1120 获得个人资料
//用户查询个人资料
//CMD_CLT_GET_CLT_INFO
CLT_GET_CLT_INFO::CLT_GET_CLT_INFO(unsigned short ver)
{
	this->version = ver;
	this->seq = 0;
	this->type = UPDATE_ALLINFO;

	this->m_Head.head.cmd = CMD_CLT_GET_CLT_INFO;
}

CLT_GET_CLT_INFO::~CLT_GET_CLT_INFO()
{
}

bool CLT_GET_CLT_INFO::unpack()  //read
{
	//read
	bool bRtn = false;

	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> uin))
		goto RETURN;
	if(!(m_Pack >> type))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool CLT_GET_CLT_INFO::pack()  //write
{
	//<<write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack << uin))
		goto RETURN;
	if(!(m_Pack << type))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//更新状态
CLT_UPDATE_STATUS::CLT_UPDATE_STATUS(unsigned short ver)
{
	this->version = ver;
	this->seq = 0;

	this->commstatus = 0;
	this->extstatus = 0;
	this->onlinestatus = 0;

	_tcscpy(this->strstatus, _T(""));

	this->m_Head.head.cmd = CMD_CLT_UPDATE_STATUS;
}

CLT_UPDATE_STATUS::~CLT_UPDATE_STATUS()
{
}

bool CLT_UPDATE_STATUS::unpack()  //read
{
	//read
	bool bRtn = false;
	unsigned short len = 0;
	
	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> type))
		goto RETURN;
	if(!(m_Pack >> commstatus))
		goto RETURN;
	if(!(m_Pack >> extstatus))
		goto RETURN;

	if(!(m_Pack >> onlinestatus))
		goto RETURN;

	if(!m_Pack.unpackString(strstatus, len, MAX_STATUS_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn ;
}

bool CLT_UPDATE_STATUS::pack()  //write
{
	//write
	bool bRtn = false;
	
	if(!(m_Pack << seq))
		goto RETURN;
	if(!(m_Pack << type))
		goto RETURN;
	if(!(m_Pack << commstatus))
		goto RETURN;
	if(!(m_Pack << extstatus))
		goto RETURN;

	if(!(m_Pack << onlinestatus))
		goto RETURN;

	if(!m_Pack.packString(strstatus, MAX_STATUS_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn ;
}

//更新用户资料
CLT_UPDATE_INFO::CLT_UPDATE_INFO(unsigned short ver)
{
	this->version = ver;
	this->seq = seq;

	this->m_Head.head.cmd = CMD_CLT_UPDATE_INFO;
}

CLT_UPDATE_INFO::~CLT_UPDATE_INFO()
{
}

bool CLT_UPDATE_INFO::unpack()  //read
{
	//>>read
	bool bRtn = false;
	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> this->type))
		goto RETURN;

	switch(type)
	{
	case UPDATE_USERINFO:
		if(!(userinfo.unpack(m_Pack, version)))
			goto RETURN;
		break;
	case UPDATE_TRADEINFO:
		if(!(tradeinfo.unpack(m_Pack, version)))
			goto RETURN;
		break;
	case UPDATE_ALLINFO:
		if(!(userinfo.unpack(m_Pack, version)))
			goto RETURN;
		if(!(tradeinfo.unpack(m_Pack, version)))
			goto RETURN;
		break;
	}

	bRtn = true;
RETURN:
	return bRtn;
}

bool CLT_UPDATE_INFO::pack()
{
	//write
	bool bRtn = false;
	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack << this->type))
		goto RETURN;

	switch(type)
	{
	case UPDATE_USERINFO:
		if(!(userinfo.pack(m_Pack, version)))
			goto RETURN;
		break;
	case UPDATE_TRADEINFO:
		if(!(tradeinfo.pack(m_Pack, version)))
			goto RETURN;
		break;
	case UPDATE_ALLINFO:
		if(!(userinfo.pack(m_Pack, version)))
			goto RETURN;
		if(!(tradeinfo.pack(m_Pack, version)))
			goto RETURN;
		break;
	}

	bRtn = true;
RETURN:
	return bRtn;
}


//更新密码
CLT_UPDATE_PW::CLT_UPDATE_PW(unsigned short ver)
{
	this->version = ver;
	this->seq = 0;
	_tcscpy(this->pw, _T(""));

	this->m_Head.head.cmd = CMD_CLT_UPDATE_PW;
}

CLT_UPDATE_PW::~CLT_UPDATE_PW()
{
}

bool CLT_UPDATE_PW::unpack()  //read
{
	//read
	bool bRtn = false;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;

	if(!m_Pack.unpackString(pw, len, MAX_PASSWORD_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool CLT_UPDATE_PW::pack()  //write
{
	//write
	bool bRtn = false;

	if(!(m_Pack << seq))
		goto RETURN;

	if(!m_Pack.packString(pw, MAX_PASSWORD_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}




//更新登录的名称
//CLT_UPDATE_LOGINSTRING
CLT_UPDATE_LOGINSTRING::CLT_UPDATE_LOGINSTRING(unsigned short ver)
{
	this->version = ver;
	this->seq = 0;
	_tcscpy(this->strid, _T(""));

	this->m_Head.head.cmd = CMD_CLT_UPDATE_LOGINSTRING;
}

CLT_UPDATE_LOGINSTRING::~CLT_UPDATE_LOGINSTRING()
{
}

bool CLT_UPDATE_LOGINSTRING::unpack()  //read
{
	//read
	bool bRtn = false;
	unsigned short len = 0;
	
	if(!(m_Pack >> seq))
		goto RETURN;

	if(!m_Pack.unpackString(strid, len, MAX_SID_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn ;
}

bool CLT_UPDATE_LOGINSTRING::pack()  //write
{
	//<<write
	bool bRtn = false;
	
	if(!(m_Pack << seq))
		goto RETURN;

	if(!m_Pack.packString(strid, MAX_SID_LEN))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn ;
}


//申请发送某一个文件
COM_FILE_SEND::COM_FILE_SEND(unsigned short version)
{
	seq = 0;
	recvuin = 0;
	senduin = 0;

	_tcscpy(sendname, _T(""));
	sendtoserver = 0;
	random = 0;

	_tcscpy(strip,_T(""));
	port = 0;
	localip = 0;
	localport = 0;

	filetotal = 0;
	filesize = 0;
	filenum = 0;

	startpos = 0;	//开始的位置, 0表示为第一次发送

	isauth = 0;          //是否加了验证  0  没有增加；1 增加
	strcpy(strauth, "");         //验证信息 

	maxfilenum = 3u;// (unsigned short)MAX_LIST_FILE;

	this->m_Head.head.cmd = CMD_COM_FILE_SEND;
}

COM_FILE_SEND::~COM_FILE_SEND()
{
}

bool COM_FILE_SEND::unpack()  //read
{
	bool bRtn = false;
	int i;
	unsigned short len = 0;

	if(!(m_Pack >> seq))
		goto RETURN;

	if(!(m_Pack >> recvuin))
		goto RETURN;
	if(!(m_Pack >> senduin))
		goto RETURN;
	if(!(m_Pack.unpackString(sendname, len, MAX_USERNAME_LEN)))
		goto RETURN;

	if(version >= 2510)
	{
		if(!(m_Pack >> sendtoserver))
			goto RETURN;
		if(!(m_Pack >> random))
			goto RETURN;
	}

	if(!(m_Pack.unpackString(strip, len, MAX_IPNUM_LEN)))
		goto RETURN;
	if(!(m_Pack >> port))
		goto RETURN;

	
	if(!(m_Pack >> filetotal))
		goto RETURN;
	if(!(m_Pack >> filesize))
		goto RETURN;

	if(!(m_Pack >> filenum))
		goto RETURN;
	if(filenum > maxfilenum)
		filenum = maxfilenum;
	
	for(i = 0; i < filenum; i++)
	{
		if(!(m_Pack.UnpackFileInfo(m_ListFile[i], version)))
			goto RETURN;
	}

	if(!(m_Pack >> startpos))
		goto RETURN;
	if(!(m_Pack >> isauth))
		goto RETURN;
	if(!(m_Pack.unpackString(strauth, len, MAX_RESPONSE_LEN)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

bool COM_FILE_SEND::pack()  //write
{
	bool bRtn = false;
	int i = 0;

	if(!(m_Pack << seq))
		goto RETURN;

	if(!(m_Pack << recvuin))
		goto RETURN;
	if(!(m_Pack << senduin))
		goto RETURN;
	if(!(m_Pack.packString(sendname, MAX_USERNAME_LEN)))
		goto RETURN;

	if(version >= 2510)
	{
		if(!(m_Pack << sendtoserver))
			goto RETURN;
		if(!(m_Pack << random))
			goto RETURN;
	}

	if(!(m_Pack.packString(strip, MAX_IPNUM_LEN)))
		goto RETURN;
	if(!(m_Pack << port))
		goto RETURN;

	
	if(!(m_Pack << filetotal))
		goto RETURN;
	if(!(m_Pack << filesize))
		goto RETURN;

	if(filenum > maxfilenum)
		filenum = maxfilenum;

	if(!(m_Pack << filenum))
		goto RETURN;
	for(; i<filenum; i++)
	{
		if(!(m_Pack.PackFileInfo(m_ListFile[i], version)))
			goto RETURN;
/*		if(!(m_Pack.packString(m_ListFile[i].strFile, MAX_FILENAME_LEN)))
			goto RETURN;

		if(!(m_Pack << m_ListFile[i].dwSize))
			goto RETURN;
		if(!(m_Pack << m_ListFile[i].IsDirectory))
			goto RETURN;
*/	}

	if(!(m_Pack << startpos))
		goto RETURN;
	if(!(m_Pack << isauth))
		goto RETURN;
	if(!(m_Pack.packString(strauth, MAX_RESPONSE_LEN)))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

//视音频包使用
//#define CMD_COM_VIDEO_FORM					0x109A  // 4250	 
//#define CMD_COM_AUDIO_FORM					0x10A4  // 4260	 
//#define CMD_COM_VIDEOTEXT_FORM				0x10AE  // 4270	
COM_VIDEOAUDIOTEXT_FORM::COM_VIDEOAUDIOTEXT_FORM(unsigned short version)
{
	this->version = version;

	for(int i = 0; i<4; i++)
	{
		recvip[i] = 0;
		recvport[i] = 0;
	}

	this->seq = 0;
	this->num = 0;
	this->index = 0;

	this->len = 0;
	this->port = 0;

	len = 0;
	sendtoserver = false;

//	this->m_Head.head.cmd = CMD_COM_VIDEOAUDIOTEXT_FORM;
}

COM_VIDEOAUDIOTEXT_FORM::~COM_VIDEOAUDIOTEXT_FORM()
{
}


bool COM_VIDEOAUDIOTEXT_FORM::unpack()
{
	//read
	bool bRtn = false;
	int i;
//	unsigned short len = 0;

	for(i = 0; i<4; i++)
	{
		if(!(m_Pack >> recvip[i]))
			goto RETURN;
		if(!(m_Pack >> recvport[i]))
			goto RETURN;
	}

	if(!(m_Pack >> seq))
		goto RETURN;
	if(!(m_Pack >> num))
		goto RETURN;
	if(!(m_Pack >> index))
		goto RETURN;

	if(!m_Pack.unpackString(sBuff, (unsigned short&)len, BUFFMAXLEN))
		goto RETURN;

	if(!(m_Pack >> uin))
		goto RETURN;
	bRtn = true;
RETURN:
	return bRtn;
}

bool COM_VIDEOAUDIOTEXT_FORM::pack()
{
	//write
	bool bRtn = false;
	int i;

	for(i = 0; i<4; i++)
	{
		if(!(m_Pack << recvip[i]))
			goto RETURN;
		if(!(m_Pack << recvport[i]))
			goto RETURN;
	}

	if(!(m_Pack << seq))
		goto RETURN;
	if(!(m_Pack << num))
		goto RETURN;
	if(!(m_Pack << index))
		goto RETURN;

	if (len>BUFFMAXLEN)
	{
		len=BUFFMAXLEN;
	}
	if(!m_Pack.packString(sBuff,len))
		goto RETURN;

	if(!(m_Pack << uin))
		goto RETURN;

	bRtn = true;
RETURN:
	return bRtn;
}

void COM_VIDEOAUDIOTEXT_FORM::SetBuff(int len, TCHAR *pBuff)
{
	if(pBuff == NULL || len <0 || len > BUFFMAXLEN)
		return;

	this->len = len;
	memcpy(sBuff, pBuff, len);
}

