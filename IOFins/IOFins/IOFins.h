/***********************************************************************
文件功能：FINSC++封装类
版权所有：广东正业科技股份有限公司
作者：黎健铭
================================修改历史================================
当前版本：V1.0.0.1
修改时间：2019-07-23
修改者：黎健铭
修改原因：创建
修改内容：无
注意事项：无
***********************************************************************/

#include <map>  
#include <vector>
#include <mutex>
#include <sstream>

#ifdef IOFINS_EXPORTS
#define IOFINS_API __declspec(dllexport)
#else
#define IOFINS_API __declspec(dllimport)
#endif

namespace nsIOFINS
{
	enum
	{
		FINS_OK = 1,					//表示命令正确 读取或者设置成功
		FINS_NG = 0,					//表示命令正确 读取或者设置失败
		INIT_ERROR = -1,				//初始化FINS错误,调用Init前需要先设置PC,PLC的IP,设置端口号,设置数据类型和标记信号点位
		WSASTARTUP_ERROR = -2,			//创建套接字失败
		WASDATA_ERROR = -3,				//套接字处理失败
		SETSOCKOPT_ERROR = -4,			//创建SOCKET失败
		FINDSIGN_ERROR = -5,			//查找FINS标记信号错误,原因为该信号没有预先调用MarkFinsSign标记
		FINSTYOE_ERROR = -6,			//设置数据类型错误
		SETSIGNDATE_ERROR = -7,			//设置数据为空
	};

	typedef enum
	{
		EMPTY = 0x00,					//表示FINS的数据类型,默认为空
		W_BIT = 0x31,					//表示是W区的BIT位
		W_BYTE = 0xB1,					//表示是W区的BYTE位
		D_BIT = 0x02,					//表示是D区的BIT位
		D_BYTE = 0x82					//表示是D区的BYTE位
	}FinsType;

	class IOFINS_API CIOFins
	{
	public:
		//默认构造函数
		CIOFins(void);

		//********************************************************************
		// 功能: 构造函数
		// 输入参数: PcIP,PlcIP,端口号,数据类型
		// 输出参数: 无
		// 返回值:
		//	备注: 必须先调用MarkFinsSign。否则初始化缺少参数
		//********************************************************************
		CIOFins(std::string PcIp, std::string PlcIp, int port, FinsType tfFinsType);
		~CIOFins(void);

	private:
		typedef std::pair<int, int> PAIR;		//用于map存放fins信号的位置

	public:
		//********************************************************************
		// 功能: 初始化FINS
		// 输入参数: 无
		// 输出参数: 无
		// 返回值:	 成功返回1，错误返回对应错误代码
		//	备注: 必须先调用SetPlcIp，SetPcIp，SetPlcPort，SetFinsType，MarkFinsSign。否则初始化缺少参数
		//********************************************************************
		int Init();

		//********************************************************************
		// 功能: 关闭FINS
		// 输入参数: 无
		// 输出参数: 无
		// 返回值:	 无
		//********************************************************************
		void Close();

		//********************************************************************
		// 功能: 设置PLC的IP
		// 输入参数: string 例 x.x.x.x
		// 输出参数: 无
		// 返回值:	 无
		//********************************************************************
		void SetPlcIp(std::string PlcIp);

		//********************************************************************
		// 功能: 设置PC的IP
		// 输入参数: string 例 x.x.x.x
		// 输出参数: 无
		// 返回值:	 无
		//********************************************************************
		void SetPcIp(std::string PcIp);

		//********************************************************************
		// 功能: 网络通讯端口号
		// 输入参数: int 例 9600
		// 输出参数: 无
		// 返回值:	 无
		//********************************************************************
		void SetPlcPort(int port);

		//********************************************************************
		// 功能: 设置FINS数据格式
		// 输入参数: FinsType 需要明确是D区还是W区，BIT还是BYTE
		// 输出参数: 无
		// 返回值:	 无
		//********************************************************************
		void SetFinsType(FinsType tfFinsType);

		//********************************************************************
		// 功能: 标记FINS的地址
		// 输入参数: SignName表示该地址的表示，address表示该地址的字，content表示该地址的位
		// 输出参数: 无
		// 返回值:	 无
		//********************************************************************
		void MarkFinsSign(std::string SignName, int address, int content);

		//********************************************************************
		// 功能: 设置FINS的内容
		// 输入参数: SignName表示该地址的表示，data表示需要设置的内容
		// 输出参数: 无
		// 返回值:	 无
		// 备注:  SignName做为开始设置的地址的启始位置，根据data的容量大小设置内容，
		// 因此允许连续对一段地址设置，不支持中间跳跃某段地址设置
		//********************************************************************
		int setSignal(std::string SignalName, int value, FinsType tfFinsType);
		int setSignal(std::string SignalName, int value, int count, FinsType tfFinsType);
		int setSignal(std::string SignalName, std::vector<int> data, FinsType tfFinsType);

		//********************************************************************
		// 功能: 读取FINS的内容
		// 输入参数: SignName表示该地址的表示，Length表示读取的长度
		// 输出参数: Value表示读取的内容
		// 返回值:	 无
		// 备注:  SignName做为开始读取的地址的启始位置，根据Length长度读取对应的内容，
		// 因此允许连续对一段地址读取，不支持中间跳跃某段地址读取
		//********************************************************************
		int getSignal(std::string SignalName, int &Value, FinsType tfFinsType);
		int getSignal(std::string SignalName, std::vector<int> &Value, FinsType tfFinsType, int Length = 1);

	private:
		int WriteArea(int ByteArea, int BitArea, std::vector<int> data, FinsType tfFinsType);
		int ReadArea(int ByteArea, int BitArea, std::vector<int> &data, int SignalNum, FinsType tfFinsType);
		int CommandAction(BYTE* Command, int CommandLen, char* Response);
		int SockInit(const char* cp, u_short port);
		int FindFinsSign(std::string SignName, int &address, int &content);
		int GetNode(std::string IP);
		int GetPlcPort();
		std::string GetPlcIp();
		std::string GetPcIp();
		FinsType GetFinsType();

	private:
		int m_iConnectPlcSuccess;
		int m_iPort;
		std::map<std::string, PAIR> m_mapSign;
		std::string m_strPlcIP;
		std::string m_strPcIp;
		FinsType m_FinsTyp;
		SOCKET m_ClientSock;
		WORD m_wVersionRequested;
		WSADATA m_wsaData;
		SOCKADDR_IN m_SrvAddr;
		HANDLE m_hStateEvent;
	};
}
extern IOFINS_API int nIOFins;

IOFINS_API int fnIOFins(void);
