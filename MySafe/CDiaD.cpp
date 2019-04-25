﻿// CDiaD.cpp: 实现文件
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
	DDX_Control(pDX, IDC_LIST3, m_ModeInfo);
	DDX_Control(pDX, IDC_LIST5, m_Mode_XiangX_Info);
}


BEGIN_MESSAGE_MAP(CDiaD, CDialogEx)
	
	ON_WM_DROPFILES()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaD::OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CDiaD::OnNMClickList2)
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
	m_List_Quduan.DeleteAllItems();
	Get_PE_InFo();

	CDialogEx::OnDropFiles(hDropInfo);
}
//----------点击打开文件
void CDiaD::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_FilePath.Empty();
	memset(m_FilePathA, 0, sizeof(m_FilePathA));
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY,
		_T("Describe Files (*.dll)|*.dll|(*.exe)|*.exe|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		//获取到文件路径
		m_FilePath = dlgFile.GetPathName();
		if (!wcscmp(m_FilePath, L""))
			return;
	}
	wsprintfA(m_FilePathA, "%S", m_FilePath);
	//写处理函数
	ReadFileToMemory();
	if (!IsPeFile()) {
		MessageBox(L"打开的不是PE文件");
		return;
	}
	m_List_Quduan.DeleteAllItems();
	Get_PE_InFo();
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
			return dwRVA - pSec->VirtualAddress + pSec->PointerToRawData+((DWORD)m_pBuf);
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


	m_Directory.InsertColumn(0, L"目录名称", LVCFMT_LEFT, 70);
	m_Directory.InsertColumn(1, L"表RVA", LVCFMT_LEFT, 70);
	m_Directory.InsertColumn(2, L"表大小", LVCFMT_LEFT, 70);
	m_Directory.InsertColumn(3, L"表FOA", LVCFMT_LEFT, 70);
	m_Directory.InsertItem(0, L"导出表");
	m_Directory.InsertItem(1, L"导入表");
	m_Directory.InsertItem(2, L"资源表");
	m_Directory.InsertItem(3, L"重定位表");
	m_Directory.InsertItem(4, L"延迟加载");
	m_Directory.InsertItem(5, L"STL表");
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
//获取表信息
BOOL CDiaD::Get_EXPORT_Directory()
{
	//获取目录信息 并显示
	//PIMAGE_DATA_DIRECTORY l_Directoty = &m_pNt->OptionalHeader.DataDirectory[0];
	

	CString str = {};
	CString str1 = {};
	CString str2 = {};
	int i = 0;
	for (int j = 0; j < 15; j++) {
		if (j < 3 || j == 5 || j == 9 || j == 13) {
			str.Empty(); str.Format(L"%08X", m_pNt->OptionalHeader.DataDirectory[j].VirtualAddress);
			str1.Empty(); str1.Format(L"%d", m_pNt->OptionalHeader.DataDirectory[j].Size);
			str2.Empty(); str2.Format(L"%d", RVAtoFOA(m_pNt->OptionalHeader.DataDirectory[j].VirtualAddress));
			m_Directory.SetItemText(i, 1, str);
			m_Directory.SetItemText(i, 2, str1);
			m_Directory.SetItemText(i, 3, str2);
			i++;
		}
	}

	//计算导出表的文件偏移具体在文件中的位置
	//PIMAGE_EXPORT_DIRECTORY l_Export = (PIMAGE_EXPORT_DIRECTORY)RVAtoFOA(l_Directoty->VirtualAddress);
	//获取模块名
	//char*pModeName =(char*)RVAtoFOA(l_Export->Name);

	return TRUE;
}

void CDiaD::Clear_m_ModeInfo() {
	m_ModeInfo.DeleteAllItems();
	int n = m_ModeInfo.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < n; i++) {
		m_ModeInfo.DeleteColumn(0);
	}
}
//初始化函数
void CDiaD::Clear_m_Mode_XiangX_Info() {
	m_Mode_XiangX_Info.DeleteAllItems();
	int n1 = m_Mode_XiangX_Info.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < n1; i++) {
		m_Mode_XiangX_Info.DeleteColumn(0);
	}
}

//各种表的点击事件根据不同的表显示不同的信息
void CDiaD::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString str = {};
	switch (pNMItemActivate->iItem)
	{
	case 0: 
		str = L"导出表";
		{
		//获取到导出表在文件中的首地址
		PIMAGE_EXPORT_DIRECTORY l_Export = (PIMAGE_EXPORT_DIRECTORY)(RVAtoFOA(m_pNt->OptionalHeader.DataDirectory[0].VirtualAddress));
		  //初始化模块显示工作去
			
			Clear_m_ModeInfo();
			m_ModeInfo.InsertColumn(0, L"模块名称", LVCFMT_LEFT,100);
			m_ModeInfo.InsertColumn(1, L"hs地址RVA", LVCFMT_LEFT, 80);
			m_ModeInfo.InsertColumn(2, L"hs名称表RVA", LVCFMT_LEFT, 80);
			m_ModeInfo.InsertColumn(3, L"hs序号表RVA", LVCFMT_LEFT, 80);
			if (l_Export == NULL)
				break;
			//获取模块名称
			wchar_t l_Name[20] = {};
			wsprintfW(l_Name, L"%S", (char*)(RVAtoFOA(l_Export->Name)));
			m_ModeInfo.InsertItem(0, l_Name);
			//获取函数地址RVA
			CString l_HS_DZRVA = {};
			l_HS_DZRVA.Format(L"%08X", l_Export->AddressOfFunctions);
			m_ModeInfo.SetItemText(0, 1, l_HS_DZRVA);

			//获取函数名称表RVA
			CString l_HS_MCRVA = {};
			l_HS_MCRVA.Format(L"%08X",l_Export->AddressOfNames);
			m_ModeInfo.SetItemText(0, 2, l_HS_MCRVA);

			//获取函数序号表RVA
			CString  l_HS_XUHRVA = {};
			l_HS_XUHRVA.Format(L"%08X", l_Export->AddressOfNameOrdinals);
			m_ModeInfo.SetItemText(0, 3, l_HS_XUHRVA);

			//初始化模块详细信息
			Clear_m_Mode_XiangX_Info();
			m_Mode_XiangX_Info.InsertColumn(0, L"函数地址", LVCFMT_LEFT,60);
			m_Mode_XiangX_Info.InsertColumn(1, L"函数名称", LVCFMT_LEFT, 120);
			m_Mode_XiangX_Info.InsertColumn(2, L"函数序号", LVCFMT_LEFT, 60);
			//获取函数数量
			DWORD l_Number = l_Export->NumberOfFunctions;
			DWORD dwFunNameCount = l_Export->NumberOfNames;
			//函数地址表
			PDWORD pFunAddr = (PDWORD)(RVAtoFOA(l_Export->AddressOfFunctions));
			//序号表
			PWORD pFunOrdinal = (PWORD)(RVAtoFOA(l_Export->AddressOfNameOrdinals));
			//函数名称表
			PDWORD pFunName = (PDWORD)(RVAtoFOA(l_Export->AddressOfNames));
			//临时变量函数地址
			CString l_HS_Adders = {};
			//临时函数名称
			wchar_t l_FunName[100] = {};
			//临时函数序号
			CString l_Xuhao = {};

			int l_i = 0;
			for (int i = 0; i < l_Number; i++) {
				//如果有无效地址，直接下一个
				if (pFunAddr[i] == 0)
					continue;
				//获取地址
				l_HS_Adders.Format(L"%08X", pFunAddr[i]);
				//上传地址
				m_Mode_XiangX_Info.InsertItem(l_i, l_HS_Adders);

				//判断是否是符号导出（是否有函数名字）
				//遍历序号表，看是否存在此序号（地址表下标 i ）
				//bool bFalg = false; //标识默认没有名字
				for (int j = 0; j < dwFunNameCount; j++)
				{
					if (i == pFunOrdinal[j])
					{
						//存在说明有函数名称
						//bFalg = true;
						DWORD dwNameAddr = pFunName[j];
						wsprintfW(l_FunName,L"%S",(char*)RVAtoFOA(dwNameAddr));
						//上传函数名
						m_Mode_XiangX_Info.SetItemText(l_i, 1, l_FunName);

						break;
					}
				}

					l_Xuhao.Format(L"%d", i + l_Export->Base);
					m_Mode_XiangX_Info.SetItemText(l_i, 2, l_Xuhao);
			

				l_i++;
			}

		}
		break;
	case 1:
		str = L"导入表"; 
		{
			//初始化列表
			m_ModeInfo.DeleteAllItems();
			Clear_m_Mode_XiangX_Info();

			m_Mode_XiangX_Info.InsertColumn(0, L"导入模块", LVCFMT_LEFT, 150);
			m_Mode_XiangX_Info.InsertColumn(1, L"函数名称", LVCFMT_LEFT, 60);
			m_Mode_XiangX_Info.InsertColumn(2, L"函数序号", LVCFMT_LEFT, 60);

			//获取导入表文件首地址
			PIMAGE_IMPORT_DESCRIPTOR l_Import=(PIMAGE_IMPORT_DESCRIPTOR)(RVAtoFOA(m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress));
			//临时变量
			wchar_t l_ModeName[100] = {};
			int l_Mode_j = 0;
			CString l_str = {};
			while (l_Import->Name)
			{
				//导入模块的名称
				memset(l_ModeName, 0, sizeof(l_ModeName));
				wsprintfW(l_ModeName, L"%S", (char*)RVAtoFOA(l_Import->Name));

				m_Mode_XiangX_Info.InsertItem(l_Mode_j, l_ModeName);
				BOOL l_ZJ = FALSE;
				//通过INT来遍历
				PIMAGE_THUNK_DATA pINT =(PIMAGE_THUNK_DATA)(RVAtoFOA(l_Import->OriginalFirstThunk));
				while (pINT->u1.AddressOfData)
				{
					
					//判断到方式，如果IMAGE_THUNK_DATA最高为为1说明是序号导入
					//否则是符号导入
					if (l_ZJ) {
						m_Mode_XiangX_Info.InsertItem(l_Mode_j,L"0");
					}

					if (pINT->u1.AddressOfData & 0x80000000)
					{
						//序号导入
						l_str.Format(L"%d", pINT->u1.AddressOfData & 0xFFFF);
						m_Mode_XiangX_Info.SetItemText(l_Mode_j,2,l_str);
					}
					else
					{
						PIMAGE_IMPORT_BY_NAME pName =(PIMAGE_IMPORT_BY_NAME)(RVAtoFOA(pINT->u1.AddressOfData));
						memset(l_ModeName, 0, sizeof(l_ModeName));
						wsprintfW(l_ModeName,L"%S", pName->Name);

						m_Mode_XiangX_Info.SetItemText(l_Mode_j, 1, l_ModeName);

						l_str.Empty();
						l_str.Format(L"%d", pName->Hint);
						m_Mode_XiangX_Info.SetItemText(l_Mode_j, 2, l_str);
					}
					//下一个导入函数
					pINT++;
					l_Mode_j++;//下一个函数名称或者序号
					l_ZJ = TRUE;
				}
				//下一个导入的dll
				l_Import++;
				
			}

		}
		break;
	case 2:
		str = L"资源表";
		break;
	case 3:
		str = L"重定位表";
		break;
	case 4:
		str = L"延迟加载表";
		break;
	case 5:
		str = L"STL表";
		break;
	default:str = L"??????";
		break;
	}
	SetDlgItemText(IDC_BNCXG, str);
	//



	*pResult = 0;
}
