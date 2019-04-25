// CDiaD.cpp: 实现文件
//

#include "stdafx.h"
#include "MySafe.h"
#include "CDiaD.h"
#include "afxdialogex.h"


// CDiaD 对话框

IMPLEMENT_DYNAMIC(CDiaD, CDialogEx)

CDiaD::CDiaD(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, m_RuKouDian(_T(""))
	, m_JxJizhi(_T(""))
	, m_DaiMaJizhi(_T(""))
	, m_QuDuanShuMu(_T(""))
{

}

CDiaD::~CDiaD()
{
}

void CDiaD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_RuKouDian);
	DDX_Text(pDX, IDC_EDIT2, m_JxJizhi);
	DDX_Text(pDX, IDC_EDIT5, m_DaiMaJizhi);
	DDX_Text(pDX, IDC_EDIT7, m_QuDuanShuMu);
	DDX_Control(pDX, IDC_LIST1, m_List_Quduan);
	DDX_Control(pDX, IDC_LIST2, m_Directory);
}


BEGIN_MESSAGE_MAP(CDiaD, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON1, &CDiaD::OnBnClickedButton1)
	ON_WM_DROPFILES()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CDiaD 消息处理程序

//---------拖拽文件-------------
void CDiaD::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	memset(m_FilePathA, 0, MAX_PATH);
	TCHAR str[MAX_PATH] = {};
	DragQueryFile(hDropInfo, 0, str, MAX_PATH); //获取文件路径
	DragFinish(hDropInfo);
	if (!wcscmp(str, L""))
		return;
	//写处理函数
	wsprintfA(m_FilePathA, "%S", str);
	ReadFileToMemory();
	if (!IsPeFile()) {
		MessageBox(L"打开的不是PE文件");
		return;
	}
	Get_PE_InFo();

	CDialogEx::OnDropFiles(hDropInfo);
}
//将文件读取到内存中
void CDiaD::ReadFileToMemory()
{
	FILE* pFile = NULL;

	fopen_s(&pFile, m_FilePathA, "rb");
	if (!pFile)
	{
		MessageBox(L"文件打开失败");
		return;
	}
	//获取文件大小
	fseek(pFile, 0, SEEK_END);
	int nSize = ftell(pFile);
	m_pBuf = new char[nSize] {};
	//读文件到内存中
	fseek(pFile, 0, SEEK_SET);
	fread(m_pBuf, nSize, 1, pFile);
	//关闭文件
	fclose(pFile);
	return;
}
//判断是否是PE文件
BOOL CDiaD::IsPeFile()
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pBuf;
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)//0x5A4D
	{
		return false;
	}
	//NT头
	m_pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + m_pBuf);
	if (m_pNt->Signature != IMAGE_NT_SIGNATURE) //0x00004550
	{
		return FALSE;
	}
	return TRUE;
}
//RVA --> FOA
DWORD CDiaD::RVAtoFOA(DWORD dwRVA)
{
	//区段表首地址
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(m_pNt);
	//区段表中的个数
	DWORD dwCount = m_pNt->FileHeader.NumberOfSections;
	for (unsigned int i = 0; i < dwCount; i++)
	{						//该块在内存中的RVA
		if (dwRVA >= pSec->VirtualAddress &&	//该块在磁盘中所占的大小
			dwRVA < (pSec->VirtualAddress + pSec->SizeOfRawData))
		{	//返回FOA=相对虚拟地址(RVA)-在文件中所占的大小
			return dwRVA - pSec->VirtualAddress + pSec->PointerToRawData + (DWORD)m_pBuf;
		}
		//下一个区段
		pSec++;
	}
	return 0;
}
//获取pe信息
void CDiaD::Get_PE_InFo()
{
	//获取入口点
	m_RuKouDian.Format(L"%08X", m_pNt->OptionalHeader.AddressOfEntryPoint);
	UpdateData(FALSE);
	//获取镜像基址
	m_JxJizhi.Format(L"%08X", m_pNt->OptionalHeader.ImageBase);
	UpdateData(FALSE);
	//获取代码基址
	m_DaiMaJizhi.Format(L"%08X", m_pNt->OptionalHeader.BaseOfCode);
	UpdateData(FALSE);
	//获取区段数量
	m_QuDuanShuMu.Format(L"%u", m_pNt->FileHeader.NumberOfSections);
	UpdateData(FALSE);

	//获取区段信息

	//获取区段表首地址 
	PIMAGE_SECTION_HEADER l_pSec = IMAGE_FIRST_SECTION(m_pNt);
	//获取区段个数
	WORD nNum = m_pNt->FileHeader.NumberOfSections;
	
	//遍历区段信息
	CString str = {};
	for (WORD i = 0; i < nNum; i++) {
		str.Empty();
		
		for (int i = 0; l_pSec->Name[i] != '\0'; i++) {
			str += l_pSec->Name[i];
		}
		//显示区段名
		m_List_Quduan.InsertItem(i, str);
		//显示内存RVA
		str.Empty();
		str.Format(L"%d", l_pSec->VirtualAddress);
		m_List_Quduan.SetItemText(i, 1, str);
		//显示磁盘中占得大小
		str.Empty();
		str.Format(L"%d", l_pSec->SizeOfRawData);
		m_List_Quduan.SetItemText(i, 2, str);
		//显示磁盘中的偏移
		str.Empty();
		str.Format(L"%d", l_pSec->PointerToRawData);
		m_List_Quduan.SetItemText(i, 3, str);
		//SetDlgItemText(IDC_GEZB, _T("新标题"));
		//下一个区段
		l_pSec++;
	}
	Get_EXPORT_Directory();
}


BOOL CDiaD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_List_Quduan.InsertColumn(0, L"段名", LVCFMT_LEFT, 50);
	m_List_Quduan.InsertColumn(1, L"内存中的偏移", LVCFMT_LEFT, 90);
	m_List_Quduan.InsertColumn(2, L"磁盘中大小", LVCFMT_LEFT, 90);
	m_List_Quduan.InsertColumn(3, L"磁盘中的偏移", LVCFMT_LEFT, 90);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
//获取导出表信息
BOOL CDiaD::Get_EXPORT_Directory()
{
	//获取目录信息 并显示
	//PIMAGE_DATA_DIRECTORY l_Directoty = &m_pNt->OptionalHeader.DataDirectory[0];
	m_Directory.InsertColumn(0, L"目录名称", LVCFMT_LEFT,70);
	m_Directory.InsertColumn(1, L"表RVA", LVCFMT_LEFT,70);
	m_Directory.InsertColumn(2, L"表大小", LVCFMT_LEFT, 70);
	m_Directory.InsertColumn(3, L"表FOA", LVCFMT_LEFT, 70);
	m_Directory.InsertItem(0, L"导入表");
	m_Directory.InsertItem(1, L"导出表");
	m_Directory.InsertItem(2, L"资源表");
	m_Directory.InsertItem(3, L"重定位表");
	m_Directory.InsertItem(4, L"延迟加载");
	m_Directory.InsertItem(5, L"STL表");

	CString str = {};
	CString str1 = {};
	CString str2 = {};

	for (int i = 0; i < 6; i++) {
		str.Empty(); str.Format(L"%08X", m_pNt->OptionalHeader.DataDirectory[i].VirtualAddress);
		str1.Empty(); str1.Format(L"%d", m_pNt->OptionalHeader.DataDirectory[i].Size);
		str2.Empty(); str1.Format(L"%08X", RVAtoFOA( m_pNt->OptionalHeader.DataDirectory[i].VirtualAddress));
		m_Directory.SetItemText(i, 1, str);
		m_Directory.SetItemText(i, 2, str1);
		m_Directory.SetItemText(i, 3, str2);
	}

	//计算导出表的文件偏移具体在文件中的位置
	//PIMAGE_EXPORT_DIRECTORY l_Export = (PIMAGE_EXPORT_DIRECTORY)RVAtoFOA(l_Directoty->VirtualAddress);
	//获取模块名
	//char*pModeName =(char*)RVAtoFOA(l_Export->Name);

	return TRUE;
}
