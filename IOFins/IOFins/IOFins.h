/***********************************************************************
�ļ����ܣ�FINSC++��װ��
��Ȩ���У��㶫��ҵ�Ƽ��ɷ����޹�˾
���ߣ��轡��
================================�޸���ʷ================================
��ǰ�汾��V1.0.0.1
�޸�ʱ�䣺2019-07-23
�޸��ߣ��轡��
�޸�ԭ�򣺴���
�޸����ݣ���
ע�������
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
		FINS_OK = 1,					//��ʾ������ȷ ��ȡ�������óɹ�
		FINS_NG = 0,					//��ʾ������ȷ ��ȡ��������ʧ��
		INIT_ERROR = -1,				//��ʼ��FINS����,����Initǰ��Ҫ������PC,PLC��IP,���ö˿ں�,�����������ͺͱ���źŵ�λ
		WSASTARTUP_ERROR = -2,			//�����׽���ʧ��
		WASDATA_ERROR = -3,				//�׽��ִ���ʧ��
		SETSOCKOPT_ERROR = -4,			//����SOCKETʧ��
		FINDSIGN_ERROR = -5,			//����FINS����źŴ���,ԭ��Ϊ���ź�û��Ԥ�ȵ���MarkFinsSign���
		FINSTYOE_ERROR = -6,			//�����������ʹ���
		SETSIGNDATE_ERROR = -7,			//��������Ϊ��
	};

	typedef enum
	{
		EMPTY = 0x00,					//��ʾFINS����������,Ĭ��Ϊ��
		W_BIT = 0x31,					//��ʾ��W����BITλ
		W_BYTE = 0xB1,					//��ʾ��W����BYTEλ
		D_BIT = 0x02,					//��ʾ��D����BITλ
		D_BYTE = 0x82					//��ʾ��D����BYTEλ
	}FinsType;

	class IOFINS_API CIOFins
	{
	public:
		//Ĭ�Ϲ��캯��
		CIOFins(void);

		//********************************************************************
		// ����: ���캯��
		// �������: PcIP,PlcIP,�˿ں�,��������
		// �������: ��
		// ����ֵ:
		//	��ע: �����ȵ���MarkFinsSign�������ʼ��ȱ�ٲ���
		//********************************************************************
		CIOFins(std::string PcIp, std::string PlcIp, int port, FinsType tfFinsType);
		~CIOFins(void);

	private:
		typedef std::pair<int, int> PAIR;		//����map���fins�źŵ�λ��

	public:
		//********************************************************************
		// ����: ��ʼ��FINS
		// �������: ��
		// �������: ��
		// ����ֵ:	 �ɹ�����1�����󷵻ض�Ӧ�������
		//	��ע: �����ȵ���SetPlcIp��SetPcIp��SetPlcPort��SetFinsType��MarkFinsSign�������ʼ��ȱ�ٲ���
		//********************************************************************
		int Init();

		//********************************************************************
		// ����: �ر�FINS
		// �������: ��
		// �������: ��
		// ����ֵ:	 ��
		//********************************************************************
		void Close();

		//********************************************************************
		// ����: ����PLC��IP
		// �������: string �� x.x.x.x
		// �������: ��
		// ����ֵ:	 ��
		//********************************************************************
		void SetPlcIp(std::string PlcIp);

		//********************************************************************
		// ����: ����PC��IP
		// �������: string �� x.x.x.x
		// �������: ��
		// ����ֵ:	 ��
		//********************************************************************
		void SetPcIp(std::string PcIp);

		//********************************************************************
		// ����: ����ͨѶ�˿ں�
		// �������: int �� 9600
		// �������: ��
		// ����ֵ:	 ��
		//********************************************************************
		void SetPlcPort(int port);

		//********************************************************************
		// ����: ����FINS���ݸ�ʽ
		// �������: FinsType ��Ҫ��ȷ��D������W����BIT����BYTE
		// �������: ��
		// ����ֵ:	 ��
		//********************************************************************
		void SetFinsType(FinsType tfFinsType);

		//********************************************************************
		// ����: ���FINS�ĵ�ַ
		// �������: SignName��ʾ�õ�ַ�ı�ʾ��address��ʾ�õ�ַ���֣�content��ʾ�õ�ַ��λ
		// �������: ��
		// ����ֵ:	 ��
		//********************************************************************
		void MarkFinsSign(std::string SignName, int address, int content);

		//********************************************************************
		// ����: ����FINS������
		// �������: SignName��ʾ�õ�ַ�ı�ʾ��data��ʾ��Ҫ���õ�����
		// �������: ��
		// ����ֵ:	 ��
		// ��ע:  SignName��Ϊ��ʼ���õĵ�ַ����ʼλ�ã�����data��������С�������ݣ�
		// �������������һ�ε�ַ���ã���֧���м���Ծĳ�ε�ַ����
		//********************************************************************
		int setSignal(std::string SignalName, int value, FinsType tfFinsType);
		int setSignal(std::string SignalName, int value, int count, FinsType tfFinsType);
		int setSignal(std::string SignalName, std::vector<int> data, FinsType tfFinsType);

		//********************************************************************
		// ����: ��ȡFINS������
		// �������: SignName��ʾ�õ�ַ�ı�ʾ��Length��ʾ��ȡ�ĳ���
		// �������: Value��ʾ��ȡ������
		// ����ֵ:	 ��
		// ��ע:  SignName��Ϊ��ʼ��ȡ�ĵ�ַ����ʼλ�ã�����Length���ȶ�ȡ��Ӧ�����ݣ�
		// �������������һ�ε�ַ��ȡ����֧���м���Ծĳ�ε�ַ��ȡ
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
