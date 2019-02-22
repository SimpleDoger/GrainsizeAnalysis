
// GrainsizeAnalysisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GrainsizeAnalysis.h"
#include "GrainsizeAnalysisDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGrainsizeAnalysisDlg 对话框

CGrainsizeAnalysisDlg::CGrainsizeAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGrainsizeAnalysisDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGrainsizeAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DRAWPANEL, m_DrawPanelControl);
	DDX_Control(pDX, IDC_LIST_DATA, m_RawDataListControl);
	DDX_Control(pDX, IDC_COMBO_NUMBER, m_NumberSelectControl);
	DDX_Control(pDX, IDC_COMBO_1ST, m_firBreakPointComboControl);
	DDX_Control(pDX, IDC_COMBO_2ND, m_secBreakPointComboControl);
	DDX_Control(pDX, IDC_COMBO_3RD, m_thrBreakPointComboControl);
	DDX_Control(pDX, IDC_COMBO_4TH, m_forBreakPointComboControl);
	DDX_Control(pDX, IDC_COMBO_SAMPLE, m_SampleListControl);
	DDX_Control(pDX, IDC_CHECK_SHOWBREAKPOINT, m_CheckToShowControl);
}

BEGIN_MESSAGE_MAP(CGrainsizeAnalysisDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENU_IMPORTDATA, &CGrainsizeAnalysisDlg::OnMenuImportdata)
	ON_BN_CLICKED(IDC_BTN_BEFOR, &CGrainsizeAnalysisDlg::OnBnClickedBtnBefor)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CGrainsizeAnalysisDlg::OnBnClickedBtnNext)
	ON_COMMAND(ID_TOOL_CLEANDATA, &CGrainsizeAnalysisDlg::OnToolCleandata)
	ON_CBN_SELCHANGE(IDC_COMBO_NUMBER, &CGrainsizeAnalysisDlg::OnCbnSelchangeComboNumber)
	ON_CBN_SELCHANGE(IDC_COMBO_1ST, &CGrainsizeAnalysisDlg::OnCbnSelchangeCombo1st)
	ON_CBN_SELCHANGE(IDC_COMBO_2ND, &CGrainsizeAnalysisDlg::OnCbnSelchangeCombo2nd)
	ON_CBN_SELCHANGE(IDC_COMBO_3RD, &CGrainsizeAnalysisDlg::OnCbnSelchangeCombo3rd)
	ON_CBN_SELCHANGE(IDC_COMBO_4TH, &CGrainsizeAnalysisDlg::OnCbnSelchangeCombo4th)
	ON_COMMAND(ID_ABOUT_ABOUTGRAINSIZEANALYSIS, &CGrainsizeAnalysisDlg::OnAboutAboutgrainsizeanalysis)
	ON_COMMAND(ID_ABOUT_HOWTOUSE, &CGrainsizeAnalysisDlg::OnAboutHowtouse)
	ON_COMMAND(ID_MENU_QUIT, &CGrainsizeAnalysisDlg::OnMenuQuit)
	ON_CBN_SELCHANGE(IDC_COMBO_SAMPLE, &CGrainsizeAnalysisDlg::OnCbnSelchangeComboSample)
	ON_COMMAND(ID_MENU_SAVE, &CGrainsizeAnalysisDlg::OnMenuSave)
	ON_COMMAND(ID_MENU_OPEN, &CGrainsizeAnalysisDlg::OnMenuOpen)
	ON_COMMAND(ID_TOOL_EXPORTASCSV, &CGrainsizeAnalysisDlg::OnToolExportascsv)
	ON_COMMAND(ID_TOOL_PREVIOUS, &CGrainsizeAnalysisDlg::OnToolPrevious)
	ON_COMMAND(ID_TOOL_NEXT, &CGrainsizeAnalysisDlg::OnToolNext)
	ON_COMMAND(ID_MENU_SAVETO, &CGrainsizeAnalysisDlg::OnMenuSaveto)
	ON_EN_CHANGE(IDC_STATIC_POSITION, &CGrainsizeAnalysisDlg::OnEnChangeStaticPosition)
	ON_EN_CHANGE(IDC_STATIC_LONGITUDE, &CGrainsizeAnalysisDlg::OnEnChangeStaticLongitude)
	ON_EN_CHANGE(IDC_STATIC_LATITUDE, &CGrainsizeAnalysisDlg::OnEnChangeStaticLatitude)
	ON_EN_CHANGE(IDC_STATIC_DEEP, &CGrainsizeAnalysisDlg::OnEnChangeStaticDeep)
	ON_COMMAND(ID_MENU_NEW, &CGrainsizeAnalysisDlg::OnMenuNew)
	ON_BN_CLICKED(IDC_CHECK_SHOWBREAKPOINT, &CGrainsizeAnalysisDlg::OnBnClickedCheckShowbreakpoint)
END_MESSAGE_MAP()


// CGrainsizeAnalysisDlg 消息处理程序

BOOL CGrainsizeAnalysisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	xStep = 0.25; // PHI 默认0.25
	this->filePath = "";
	this->fileIsModified = false;
	this->InitNoneSample();
	this->isShowBreakPoint = false;
	// 初始化全局X值
	for (int i = 0; i < 128; i++)
	{
		globleXValue[i] = -128;
	}

	currentSample = 0;
	this->SetDataStep(sampleArray[currentSample]);
	this->GetNormalDistributionMap();

	strncpy_s(breakPointTitle[0], "1st:", 5);
	strncpy_s(breakPointTitle[1], "2nd:", 5);
	strncpy_s(breakPointTitle[2], "3rd:", 5);
	strncpy_s(breakPointTitle[3], "4th:", 5);

	this->m_NumberSelectControl.AddString(_T("1"));
	this->m_NumberSelectControl.AddString(_T("2"));
	this->m_NumberSelectControl.AddString(_T("3"));
	this->m_NumberSelectControl.AddString(_T("4"));
	this->m_NumberSelectControl.SetCurSel(2); // 默认解释为3个点

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGrainsizeAnalysisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	// 在控件内绘制图像
	CDC *pDC = m_DrawPanelControl.GetDC();
	//绘制区域
	CRect rect, dataRect; 
	CBrush myBrush;
	myBrush.CreateSolidBrush(RGB(255, 255, 255));
	m_DrawPanelControl.GetClientRect(rect);
	rect.SetRect(rect.left + 3, rect.top + 10, rect.right - 3, rect.bottom - 3);
	dataRect.SetRect(rect.left + 3, rect.top + 20, rect.right - 3, rect.bottom - 3);
	pDC->FillRect(rect, &myBrush);

	pDC->SetViewportOrg(MARGIN, YLEN + 10); //设置坐标原点

	// 加载当前要显示的岩样
	EachSample crtSample = sampleArray[currentSample];
	// 绘制坐标底图
	this->ShowTheMap(pDC, rect, crtSample);
	// 绘制数据点与原始数据
	if (crtSample.dataCount != 0)
	{
		this->ShowThePoint(pDC, crtSample);

		CPen myRPen;
		myRPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		pDC->SelectObject(myRPen);
		switch (crtSample.breakPointCount)
		{
		case 1:
			this->DrawTheAnalysisLine1(pDC, crtSample);
			break;
		case 2:
			this->DrawTheAnalysisLine2(pDC, crtSample);
			break;
		case 3:
			this->DrawTheAnalysisLine3(pDC, crtSample);
			break;
		case 4:
			this->DrawTheAnalysisLine4(pDC, crtSample);
			break;
		default:
			break;
		}
	}
	// 显示粒径分布小图
	this->DrawSizeDitribution(pDC, rect, crtSample);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGrainsizeAnalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGrainsizeAnalysisDlg::ShowTheMap(CDC *pDC, CRect rect, EachSample sample)
{
	CFont font;
	font.CreateFontW(12, 0, 0, 0, 400, FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial"));
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(22, 22, 22));

	CPen myBPen, mySPen, mySubPen;
	myBPen.CreatePen(PS_SOLID, 2, RGB(33, 33, 33));
	mySPen.CreatePen(PS_SOLID, 1, RGB(33, 33, 33));
	mySubPen.CreatePen(PS_DOT, 1, RGB(66, 66, 66));

	pDC->SelectObject(font);
	pDC->SelectObject(mySPen);

	CString tempString;
	// 绘制横轴
	if (sample.dataCount == 0)
	{
		pDC->SelectObject(myBPen);
		pDC->MoveTo(0, 6);
		pDC->LineTo(GetShowPointX(16, sample), 6);
		pDC->SelectObject(mySPen);
		double mainScaleX[9] = { 0, 2, 4, 6, 8, 10, 12, 14, 16 };
		for (int i = 0; i < 9; i++)
		{
			int xt = GetShowPointX(mainScaleX[i], sample);
			pDC->MoveTo(xt, 5);
			pDC->LineTo(xt, 15);
			tempString.Format(_T("%.1f"), mainScaleX[i]);
			pDC->TextOutW(xt - 6, 15, tempString);
		}
		double subScaleX[8] = { 1, 3, 5, 7, 9, 11, 13, 15 };
		for (int i = 0; i < 8; i++)
		{
			pDC->MoveTo(GetShowPointX(subScaleX[i], sample), 5);
			pDC->LineTo(GetShowPointX(subScaleX[i], sample), 11);
		}
	}
	else
	{
		pDC->SelectObject(myBPen);
		pDC->MoveTo(0, 6);
		pDC->LineTo(GetShowPointX(sample.xDataArray[sample.dataCount - 1], sample), 6);
		pDC->SelectObject(mySPen);
		int c_x, b_x;
		for (int i = 0; i < sample.dataCount; i++)
		{
			c_x = GetShowPointX(sample.xDataArray[i], sample);
			pDC->MoveTo(c_x, 5);
			pDC->LineTo(c_x, 11);
			if (abs((sample.xDataArray[i] + 0.001) - (int)(sample.xDataArray[i] + 0.001)) < 0.01)
			{
				pDC->MoveTo(c_x, 5);
				pDC->LineTo(c_x, 15);
				tempString.Format(_T("%.1f"), sample.xDataArray[i]);
				pDC->TextOutW(c_x - 6, 15, tempString);
			}
		}
		// 绘制颜色条
		double spiltLine[3] = { -1, 4, 8 };
		int showSpiltLine[3] = { 0 };
		for (int i = 0; i < 3; i++)
		{
			showSpiltLine[i] = GetShowPointX(spiltLine[i], sample) + (int)(dataStep / 2.0 * (sample.xDataArray[1] - sample.xDataArray[0]));
			if (showSpiltLine[i] < 0) showSpiltLine[i] = 0; // 不绘制出界
		}
		CBrush gravelPpointBrash, sandPpointBrash, siltPpointBrash, clayPpointBrash;
		gravelPpointBrash.CreateSolidBrush(BS_GRAVEL);
		sandPpointBrash.CreateSolidBrush(BS_SAND);
		siltPpointBrash.CreateSolidBrush(BS_SILT);
		clayPpointBrash.CreateSolidBrush(BS_CLAY);
		pDC->FillRect(CRect(0, 0, showSpiltLine[0], 5), &gravelPpointBrash);
		pDC->FillRect(CRect(showSpiltLine[0], 1, showSpiltLine[1], 5), &sandPpointBrash);
		pDC->FillRect(CRect(showSpiltLine[1], 1, showSpiltLine[2], 5), &siltPpointBrash);
		pDC->FillRect(CRect(showSpiltLine[2], 1, GetShowPointX(sample.xDataArray[sample.dataCount - 1], sample), 5), &clayPpointBrash);
	}
	
	// 绘制纵轴
	pDC->SelectObject(myBPen);
	// 左纵轴
	pDC->MoveTo(-5, 0);
	pDC->LineTo(-5, 0 - GetShowPointY(0.9999));
	// 右纵轴
	pDC->MoveTo(rect.right - 2 * MARGIN + 6, 0);
	pDC->LineTo(rect.right - 2 * MARGIN + 6, 0 - GetShowPointY(0.9999));
	// 开始绘制刻度
	pDC->SelectObject(mySPen);
	pDC->MoveTo(-5, 0);
	double mainScaleY[25] = { 99.99, 99.95, 99.90, 99.80, 99.50, 99.00, 98.00, 95.00, 90.00, 80.00, 70.00, 60.00, 50.00, 40.00, 30.00, 20.00, 10.00, 5.00, 2.00, 1.00, 0.5, 0.2, 0.1, 0.05, 0.01 };
	for (int i = 0; i < 25; i++)
	{
		// 左主刻度
		tempString.Format(_T("%.2f"), mainScaleY[i]);
		pDC->MoveTo(-5, -GetShowPointY(mainScaleY[i] / 100));
		pDC->LineTo(-15, -GetShowPointY(mainScaleY[i] / 100));
		pDC->TextOutW(-50, -GetShowPointY(mainScaleY[i] / 100) - 6, tempString);
		// 右主刻度
		pDC->MoveTo(rect.right - 2 * MARGIN + 5, -GetShowPointY(mainScaleY[i] / 100));
		pDC->LineTo(rect.right - 2 * MARGIN + 15, -GetShowPointY(mainScaleY[i] / 100));
		pDC->TextOutW(rect.right - 2 * MARGIN + 20, -GetShowPointY(mainScaleY[i] / 100) - 6, tempString);
	}
	double subScaleY[74] = { 99.98, 99.97, 99.96, 99.94, 99.93, 99.92, 99.91, 0.02, 0.03, 0.04, 0.06, 0.07, 0.08, 0.09,
		99.85, 0.15,
		99.7, 99.6, 99.4, 99.3, 99.2, 99.1, 0.3, 0.4, 0.6, 0.7, 0.8, 0.9,
		98.5, 1.5,
		97, 96, 94, 93, 92, 91, 3, 4, 6, 7, 8, 9,
		12, 14, 16, 18, 22, 24, 26, 28, 32, 34, 36, 38, 42, 44, 46, 48,
		52, 54, 56, 58, 62, 64, 66, 68, 72, 74, 76, 78, 82, 84, 86, 88
	};
	for (int i = 0; i < 74; i++)
	{
		// 左副刻度
		pDC->MoveTo(-5, -GetShowPointY(subScaleY[i] / 100));
		pDC->LineTo(-11, -GetShowPointY(subScaleY[i] / 100));
		// 右副刻度
		pDC->MoveTo(rect.right - 2 * MARGIN + 5, -GetShowPointY(subScaleY[i] / 100));
		pDC->LineTo(rect.right - 2 * MARGIN + 11, -GetShowPointY(subScaleY[i] / 100));
	}
	// 绘制虚线边缘
	pDC->SelectObject(mySubPen);
	pDC->MoveTo(0, 0 - GetShowPointY(0.9999));
	pDC->LineTo(0, 0);
	pDC->LineTo(rect.right - 2 * MARGIN, 0);
	pDC->LineTo(rect.right - 2 * MARGIN, 0 - GetShowPointY(0.9999));
	pDC->LineTo(0, 0 - GetShowPointY(0.9999));

	pDC->SelectObject(mySPen);
}

void CGrainsizeAnalysisDlg::SetDataStep(EachSample sample)
{
	CRect cRect;
	m_DrawPanelControl.GetClientRect(cRect);
	cRect.SetRect(cRect.left + 3, cRect.top + 10, cRect.right - 3, cRect.bottom - 3);
	if (sample.dataCount == 0)
	{
		this->dataStep = (cRect.right - MARGIN * 2.0) / 16.0; // 默认画 0~16
		this->gs_dataStep = (double)(GS_WIDTH - GS_MARGIN * 2.0) / 16.0;
	}
	else
	{
		this->dataStep = (cRect.right - MARGIN * 2.0) / (sample.xDataArray[sample.dataCount - 1] - sample.xDataArray[0]);
		this->gs_dataStep = (double)(GS_WIDTH - GS_MARGIN * 2.0) / (sample.xDataArray[sample.dataCount - 1] - sample.xDataArray[0]);
	}
}

void CGrainsizeAnalysisDlg::GetNormalDistributionMap()
{
	// 数组容量： 2048 = 8 * 256  -->  X = -4 ~ +4; Fai(X) = 0 ~ 1;
	// 精度：1/256
	double x = -4.0;
	double eachS = 0;
	for (int i = 0; i < 2048; i++)
	{
		eachS += (1.0 / sqrt(2.0 * PI) * pow(E, (0.0 - x * x / 2.0))) * (1.0 / 256.0);
		distributionMap[i] = eachS;
		x += (1.0 / 256.0);
	}
}

int CGrainsizeAnalysisDlg::GetShowPointX(double in, EachSample sample)
{
	int result = 0;
	if (sample.dataCount != 0)
	{
		result = (int)((in - sample.xDataArray[0]) * dataStep); // 减去数组第一个元素值是为了将点提到绘制坐标系中X的0起始点
	}
	else
	{
		result = (int)((in - 0) * dataStep); // 默认 0~16
	}
	return result;
}

int CGrainsizeAnalysisDlg::GetShowPointY(double in)
{
	// distributionMap, 0 ~ 2047 --> Fai(x) 0 ~ 1 --> 0~YLEN
	// 找出特定值的数组下标，作为原始Y坐标值
	int result = 0;
	for (int i = 0; i < 2048; i++)
	{
		if (distributionMap[i] < in)
		{
			result = i;
		}
		else
		{
			break;
		}
	}
	// 基准点 0.01，修正
	for (int i = 0; i < 2048; i++)
	{
		if (distributionMap[i] < 0.0001) // 小于0.01%
		{
			base = i;
		}
		else
		{
			break;
		}
	}
	result = (int)((double)(result - base) * (double)YLEN / (double)2048);
	// 如果传入值大于最大刻度值99.99%, 则返回原长校准后的值
	if (in > 0.9999)
	{
		return (int)((double)(2048 - base) * (double)YLEN / (double)2048);
	}
	return result;
}

int CGrainsizeAnalysisDlg::GetGSShowPointX(double in, EachSample sample)
{
	int result = 0;
	if (sample.dataCount != 0)
	{
		result = (int)((in - sample.xDataArray[0]) * gs_dataStep); // 减去数组第一个元素值是为了将点提到绘制坐标系中X的0起始点
	}
	else
	{
		result = (int)((in - 0) * gs_dataStep); // 默认 0~16
	}
	return result;
}

int CGrainsizeAnalysisDlg::GetGSShowPointY(double in)
{
	// 输入值范围0~1，坐标轴最大显示0.1
	int result = 0 - (in / 10 * (double)(GS_HEIGHT - 2 * GS_MARGIN));
	return result;
}

void CGrainsizeAnalysisDlg::ShowPointToValue(int xP, int yP, double *x, double *y, EachSample sample)
{
	*x = xP / dataStep + sample.xDataArray[0];
	// yP 在显示坐标系负数区域，为0~-YLEN
	if (yP > 0) yP = 0; // 下不越界
	if (yP < 0 - YLEN) yP = 0 - YLEN + 1; // 上不越界
	*y = this->distributionMap[(int)((double)(0 - yP) * 2048.0 / (double)YLEN) + base] * 100.0;
}

int CGrainsizeAnalysisDlg::GetNormalY(double in)
{
	// distributionMap, 0 ~ 2047 --> Fai(x): 0 ~ 1
	// 找出特定值的数组下标，作为原始Y坐标值
	int result = 0;
	for (int i = 0; i < 2048; i++)
	{
		if (distributionMap[i] < (in / 100))
		{
			result = i;
		}
		else
		{
			break;
		}
	}
	return result;
}

void CGrainsizeAnalysisDlg::OnMenuImportdata()
{
	// TODO:  在此添加命令处理程序代码
	// 从TXT文件中导入数据
	if (sampleArray.GetCount() > 1)
	{
		if (MessageBox(_T("Replase current data? "), _T("Notice"), MB_ICONEXCLAMATION | MB_YESNO) == IDNO)
		{
			return;
		}
	}
	CFileDialog fileDlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CSV File(*.csv)|*.csv|All Files(*.*)|*.*||"), this);
	CString filePath;
	if (fileDlg.DoModal() == IDOK)
	{
		CStdioFile file;
		filePath = fileDlg.GetFolderPath() + "\\" + fileDlg.GetFileName();
		if (file.Open(filePath, CFile::modeReadWrite))
		{
			// 清空已有样品数据
			sampleArray.RemoveAll();
			m_SampleListControl.ResetContent();
			this->fileIsModified = true;

			sampleCount = 0;
			CString str;
			for (int i = 0; file.ReadString(str); i++)
			{
				if (i >= 1) // 舍弃第一行
				{
					int re = GetDataFromCSVLine(str); // 数据加载
					if (re == -1) // 数据加载出现错误
					{
						file.Close();
						sampleArray.RemoveAll();
						sampleCount = 0;
						this->InitNoneSample();
						return;
					}
					if (re == -2)
					{
						if (MessageBox(_T("Some errors are contained in samples, and ignore this current sample? "), _T("Notice"), MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
						{
							continue;
						}
						else
						{
							file.Close();
							sampleArray.RemoveAll();
							sampleCount = 0;
							this->InitNoneSample();
							return;
						}
					}
				}
			}
			file.Close();
			this->sampleCount = sampleArray.GetCount();
			this->currentSample = 0;
			this->SetDataStep(sampleArray[currentSample]); // 设置X坐标单元长，默认显示第一个岩样
			this->SortBreakPoint(&sampleArray[currentSample]);
			this->AddData2Combo();
			this->ShowSampleInfo();
			this->ShowRawData();
			this->Invalidate(); // 刷新界面
		}
	}
}

void CGrainsizeAnalysisDlg::ShowThePoint(CDC *pDC, EachSample sample)
{
	CPen myPen;
	myPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pDC->SelectObject(myPen);
	CBrush gravelPpointBrash, sandPpointBrash, siltPpointBrash, clayPpointBrash;
	gravelPpointBrash.CreateSolidBrush(BS_GRAVEL);
	sandPpointBrash.CreateSolidBrush(BS_SAND);
	siltPpointBrash.CreateSolidBrush(BS_SILT);
	clayPpointBrash.CreateSolidBrush(BS_CLAY);
	pDC->MoveTo(0, 0);
	for (int i = 0; i < sample.dataCount - 1; i++) // -1, 最后一个点不绘制
	{
		if (sample.xDataArray[i] < -1.001)
		{
			pDC->SelectObject(gravelPpointBrash);
		}
		else if (sample.xDataArray[i] >= -1.001 && sample.xDataArray[i] < 4.001)
		{
			pDC->SelectObject(sandPpointBrash);
		}
		else if (sample.xDataArray[i] >= 4.001 && sample.xDataArray[i] < 8.001)
		{
			pDC->SelectObject(siltPpointBrash);
		}
		else
		{
			pDC->SelectObject(clayPpointBrash);
		}
		int xp = GetShowPointX(sample.xDataArray[i], sample);
		int yp = 0 - GetShowPointY(sample.yDataArray[i] / 100);
		pDC->Ellipse(xp - 3, yp - 3, xp + 3, yp + 3);
	}
	// 绘制断点
	if (this->isShowBreakPoint)
	{
		CBrush breakPointBrash;
		breakPointBrash.CreateSolidBrush(RGB(0, 0, 0));
		pDC->SelectObject(breakPointBrash);
		for (int i = 0; i < sample.breakPointCount; i++)
		{
			if (sample.breakPointIndex[i] > 0) // 非默认点，才显示
			{
				double bx = sample.xDataArray[sample.breakPointIndex[i]];
				double by = sample.yDataArray[sample.breakPointIndex[i]];
				int xp = GetShowPointX(bx, sample);
				int yp = 0 - GetShowPointY(by / 100);
				pDC->Ellipse(xp - 3, yp - 3, xp + 3, yp + 3);
				USES_CONVERSION;
				CString tps = A2W(breakPointTitle[i]);
				pDC->TextOutW(xp - 30, yp - 14, tps);
			}
		}
	}
}

int CGrainsizeAnalysisDlg::GetDataFromCSVLine(CString eachLine)
{
	CString firstWrod, secondWord;
	AfxExtractSubString(firstWrod, (LPCTSTR)eachLine, 0, ',');
	AfxExtractSubString(secondWord, (LPCTSTR)eachLine, 1, ',');
	if (firstWrod == "" && secondWord != "") // 第2行（X轴，值）
	{
		// 添加全局X轴的值，找出PHI的步进值
		CString theWord;
		globleXCount = 0;
		for (int i = 0; i < 128; i++)
		{
			if (i == 127)
			{
				MessageBox(_T("There are some errors in data."), _T("Data leak!"), MB_ICONERROR);
				return -1;
			}
			AfxExtractSubString(theWord, (LPCTSTR)eachLine, i + 1, ',');
			int t_V = _wtoi(theWord);
			if (t_V == XMAX)
			{
				globleXValue[i] = _wtof(theWord);
				globleXValue[i] = 0 - log2(globleXValue[i] / 1000.0);
				globleXCount++;
				break;
			}
			if (globleXValue[i] > XMAX)
			{
				MessageBox(_T("There are some errors in data."), _T("Data overflow!"), MB_ICONERROR);
				return -1;
			}
			globleXValue[i] = _wtof(theWord);
			globleXValue[i] = 0 - log2(globleXValue[i] / 1000.0);
			globleXCount++;
		}
		xStep = abs(globleXValue[0] - globleXValue[1]);
	}
	else if (firstWrod != "" && secondWord == "")
	{
		EachSample eachSample;
		// 添加岩样基本信息
		AfxExtractSubString(eachSample.sampleID, (LPCTSTR)eachLine, 0, ',');
		AfxExtractSubString(eachSample.gravel, (LPCTSTR)eachLine, 1 + globleXCount, ',');
		AfxExtractSubString(eachSample.sand, (LPCTSTR)eachLine, 1 + globleXCount + 1, ',');
		AfxExtractSubString(eachSample.silt, (LPCTSTR)eachLine, 1 + globleXCount + 2, ',');
		AfxExtractSubString(eachSample.clay, (LPCTSTR)eachLine, 1 + globleXCount + 3, ',');
		AfxExtractSubString(eachSample.averageSize, (LPCTSTR)eachLine, 1 + globleXCount + 4, ',');
		AfxExtractSubString(eachSample.mediumSize, (LPCTSTR)eachLine, 1 + globleXCount + 4 + 1, ',');
		AfxExtractSubString(eachSample.kurtosisCoefficient, (LPCTSTR)eachLine, 1 + globleXCount + 4 + 2, ',');
		AfxExtractSubString(eachSample.skewnessCoefficient, (LPCTSTR)eachLine, 1 + globleXCount + 4 + 3, ',');
		AfxExtractSubString(eachSample.sortingCoefficient, (LPCTSTR)eachLine, 1 + globleXCount + 4 + 4, ',');
		// 添加X，累计频率Y，频率
		CString eachValue;
		int valueScan = globleXCount - 1;
		int valueCount = 0;
		double accumulativeValue = 0.0;
		for (int i = 0; i < 128; i++)
		{
			AfxExtractSubString(eachValue, (LPCTSTR)eachLine, valueScan, ',');
			accumulativeValue += _wtof(eachValue);
			if (accumulativeValue > 0.0001)
			{
				if (valueCount > 0 && accumulativeValue > 99.99 && eachSample.yDataArray[valueCount - 1] == accumulativeValue)
				{
					eachSample.dataCount = valueCount;
					break;
				}
				eachSample.sizeDistribution[valueCount] = _wtof(eachValue);
				eachSample.xDataArray[valueCount] = globleXValue[valueScan - 2];
				if (valueScan == 1) // Y值越界了
				{
					return -2;
				}
				eachSample.yDataArray[valueCount] = accumulativeValue;
				eachSample.normalY[valueCount + 2] = this->GetNormalY(accumulativeValue);
				valueCount++;
			}
			valueScan--;
		}
		// 外延曲线端点值
		for (int i = SPAN; i > 0; i--)
		{
			eachSample.normalY[i - 1] = 2 * eachSample.normalY[i] - eachSample.normalY[i + 1];
		}
		for (int i = 0; i < SPAN; i++)
		{
			eachSample.normalY[valueCount + i] = 2 * eachSample.normalY[valueCount - 1 + i] - eachSample.normalY[valueCount - 2 + i];
		}
		// 默认解释为3个点
		eachSample.breakPointCount = 3;
		// 识别自动断点
		this->GetBreakPointIndex(&eachSample);
		this->SortBreakPoint(&eachSample);
		// 添加每个岩样
		sampleArray.Add(eachSample);
		m_SampleListControl.AddString(eachSample.sampleID);
	}
	return 0;
}

void CGrainsizeAnalysisDlg::OnBnClickedBtnBefor()
{
	// 先保存当前样品基础信息
	this->SaveSampleInfo();

	// 显示前一个
	if (currentSample > 0)
	{
		currentSample--;
	}
	else
	{
		currentSample = sampleCount - 1;
	}
	this->m_NumberSelectControl.SetCurSel(sampleArray[currentSample].breakPointCount - 1);
	this->OnCbnSelchangeComboNumber();
	this->SetDataStep(sampleArray[currentSample]); // 设置X坐标单元长
	this->AddData2Combo();
	this->ShowSampleInfo();
	this->ShowRawData();
	this->SaveSampleInfo();
	this->Invalidate(); // 刷新界面
}

void CGrainsizeAnalysisDlg::OnBnClickedBtnNext()
{
	// 先保存当前样品基础信息
	this->SaveSampleInfo();

	// 显示后一个
	if (currentSample < sampleCount - 1)
	{
		currentSample++;
	}
	else
	{
		currentSample = 0;
	}
	this->m_NumberSelectControl.SetCurSel(sampleArray[currentSample].breakPointCount - 1);
	this->OnCbnSelchangeComboNumber();
	this->SetDataStep(sampleArray[currentSample]); // 设置X坐标单元长
	this->AddData2Combo();
	this->ShowSampleInfo();
	this->ShowRawData();
	this->Invalidate(); // 刷新界面
}

void CGrainsizeAnalysisDlg::ShowSampleInfo()
{
	m_SampleListControl.SetCurSel(currentSample);
	SetDlgItemText(IDC_STATIC_SAMPLEID, sampleArray[currentSample].sampleID);
	SetDlgItemText(IDC_STATIC_POSITION, sampleArray[currentSample].positionNum);
	SetDlgItemText(IDC_STATIC_LONGITUDE, sampleArray[currentSample].longitude);
	SetDlgItemText(IDC_STATIC_LATITUDE, sampleArray[currentSample].latitude);
	SetDlgItemText(IDC_STATIC_DEEP, sampleArray[currentSample].deep);
	SetDlgItemText(IDC_STATIC_GRAVEL, sampleArray[currentSample].gravel + "%");
	SetDlgItemText(IDC_STATIC_SAND, sampleArray[currentSample].sand + "%");
	SetDlgItemText(IDC_STATIC_SILT, sampleArray[currentSample].silt + "%");
	SetDlgItemText(IDC_STATIC_CLAY, sampleArray[currentSample].clay + "%");
	SetDlgItemText(IDC_STATIC_AVERAGESIZE, sampleArray[currentSample].averageSize);
	SetDlgItemText(IDC_STATIC_MEDIUMSIZE, sampleArray[currentSample].mediumSize);
	SetDlgItemText(IDC_STATIC_SKEWNESS, sampleArray[currentSample].skewnessCoefficient);
	SetDlgItemText(IDC_STATIC_KURTOSIS, sampleArray[currentSample].kurtosisCoefficient);
	SetDlgItemText(IDC_STATIC_SORTING, sampleArray[currentSample].sortingCoefficient);
}

void CGrainsizeAnalysisDlg::SaveSampleInfo()
{
	GetDlgItemText(IDC_STATIC_POSITION, sampleArray[currentSample].positionNum);
	GetDlgItemText(IDC_STATIC_LONGITUDE, sampleArray[currentSample].longitude);
	GetDlgItemText(IDC_STATIC_LATITUDE, sampleArray[currentSample].latitude);
	GetDlgItemText(IDC_STATIC_DEEP, sampleArray[currentSample].deep);
}

void CGrainsizeAnalysisDlg::ShowRawData()
{
	this->m_RawDataListControl.ResetContent();
	CString ts;
	for (int i = 0; i < sampleArray[currentSample].dataCount; i++)
	{
		ts.Format(_T("%d: D=%.2f,  S=%.3f"), i + 1, sampleArray[currentSample].xDataArray[i], sampleArray[currentSample].sizeDistribution[i]);
		this->m_RawDataListControl.AddString(ts);
	}
}

void CGrainsizeAnalysisDlg::InitNoneSample()
{
	// 一个空的样品数据
	EachSample noneSample;
	noneSample.dataCount = 0;
	noneSample.sampleID = "";
	noneSample.positionNum = "";
	noneSample.longitude = "";
	noneSample.latitude = "";
	noneSample.deep = "";
	noneSample.gravel = "";
	noneSample.sand = "";
	noneSample.silt = "";
	noneSample.clay = "";
	noneSample.averageSize = "";
	noneSample.mediumSize = "";
	noneSample.kurtosisCoefficient = "";
	noneSample.skewnessCoefficient = "";
	noneSample.sortingCoefficient = "";
	noneSample.breakPointCount = 3;
	noneSample.breakPointIndex[0] = -1; noneSample.breakPointIndex[1] = -1; noneSample.breakPointIndex[2] = -1; noneSample.breakPointIndex[3] = -1;
	sampleArray.Add(noneSample);
	sampleCount = 1;
}

void CGrainsizeAnalysisDlg::OnToolCleandata()
{
	// TODO:  清除数据
	sampleArray.RemoveAll();
	m_SampleListControl.ResetContent();
	sampleCount = 0;
	currentSample = 0;
	m_firBreakPointComboControl.ResetContent();
	m_secBreakPointComboControl.ResetContent();
	m_thrBreakPointComboControl.ResetContent();
	m_forBreakPointComboControl.ResetContent();
	this->InitNoneSample();
	this->SetDataStep(sampleArray[currentSample]); // 设置X坐标单元长
	this->ShowSampleInfo();
	this->ShowRawData();
	this->Invalidate();
	this->filePath = "";
	this->fileIsModified = false;
}

void CGrainsizeAnalysisDlg::GetBreakPointIndex(EachSample *pSample)
{
	pSample->breakPointIndex[0] = -1; pSample->breakPointIndex[1] = -1; pSample->breakPointIndex[2] = -1; pSample->breakPointIndex[3] = -1;
	int datacount = pSample->dataCount + 2 * SPAN; // 外延了点
	int tc = 0;
	double difference[128 + 2 * SPAN] = { 0 };
	
	for (int i = SPAN; i < datacount - SPAN; i++)
	{
		// 虚拟X轴步进长度为40
		// abs(80 * y1 - 40 * y2) / sqrt(6400 + y2^2)
		double y1 = pSample->normalY[i] - pSample->normalY[i - SPAN];
		double y2 = pSample->normalY[i + SPAN] - pSample->normalY[i - SPAN];
		difference[tc] = abs(80.0 * y1 - 40.0 * y2) / sqrt(6400.0 + y2 * y2);
		tc++;
	}
	// 找出四个极大值
	double d_difference[128 + 2 * SPAN] = { 0 };
	for (int i = 0; i < tc; i++)
	{
		if (i == tc - 1)
		{
			d_difference[i] = 0 - difference[i];
		}
		else
		{
			d_difference[i] = difference[i + 1] - difference[i];
		}
	}
	int b_scan = 0;
	for (int i = 0; i < tc - 1; i++)
	{
		if (d_difference[i] > 0 && d_difference[i + 1] < 0)
		{
			pSample->suspectBreakpoint[b_scan] = i + 1;
			b_scan++;
		}
	}
	// 当前样品疑似拐弯点数量
	pSample->suspectBreakpointCount = b_scan;
	double fir_breakpoint = 0.0;
	for (int i = 0; i < b_scan; i++)
	{
		if (pSample->suspectBreakpoint[i] > 1 && pSample->suspectBreakpoint[i]< tc - 2 && difference[pSample->suspectBreakpoint[i]] > fir_breakpoint)
		{
			fir_breakpoint = difference[pSample->suspectBreakpoint[i]];
			pSample->breakPointIndex[0] = pSample->suspectBreakpoint[i];
		}
	}
	double sec_breakpoint = 0.0;
	for (int i = 0; i < b_scan; i++)
	{
		if (pSample->suspectBreakpoint[i] > 1 && pSample->suspectBreakpoint[i]< tc - 2 && difference[pSample->suspectBreakpoint[i]] > sec_breakpoint && difference[pSample->suspectBreakpoint[i]] < fir_breakpoint)
		{
			sec_breakpoint = difference[pSample->suspectBreakpoint[i]];
			pSample->breakPointIndex[1] = pSample->suspectBreakpoint[i];
		}
	}
	double thi_breakpoint = 0.0;
	for (int i = 0; i < b_scan; i++)
	{
		if (pSample->suspectBreakpoint[i] > 1 && pSample->suspectBreakpoint[i]< tc - 2 && difference[pSample->suspectBreakpoint[i]] > thi_breakpoint && difference[pSample->suspectBreakpoint[i]] < sec_breakpoint)
		{
			thi_breakpoint = difference[pSample->suspectBreakpoint[i]];
			pSample->breakPointIndex[2] = pSample->suspectBreakpoint[i];
		}
	}
	double for_breakpoint = 0.0;
	for (int i = 0; i < b_scan; i++)
	{
		if (pSample->suspectBreakpoint[i] > 1 && pSample->suspectBreakpoint[i]< tc - 2 && difference[pSample->suspectBreakpoint[i]] > for_breakpoint && difference[pSample->suspectBreakpoint[i]] < thi_breakpoint)
		{
			for_breakpoint = difference[pSample->suspectBreakpoint[i]];
			pSample->breakPointIndex[3] = pSample->suspectBreakpoint[i];
		}
	}

	/*
	CString ts, tss;
	for (int i = 0; i < tc; i++)
	{
		tss.Format(_T("%.4f, "), difference[i]);
		ts += tss;
	}
	MessageBox(ts, _T(""), 0);
	*/
}

void CGrainsizeAnalysisDlg::OldDrawTheAnalysisLine1(CDC *pDC, EachSample sample)
{
	// 不能有相同X轴的点
	if (sample.breakPointIndex[0] == sample.breakPointIndex[1])
	{
		return;
	}

	int firBaseX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample);
	int firBaseY = 0 - GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0);

	// 绘制拐弯点左段
	double firLineK = 0.0;
	for (int i = 0; i < sample.breakPointIndex[0]; i++)
	{
		double deltaX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample) - GetShowPointX(sample.xDataArray[i], sample);
		double deltaY = GetShowPointY(sample.yDataArray[i] / 100.0) - GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0);
		firLineK += deltaY / deltaX;
	}
	firLineK /= sample.breakPointIndex[0];
	pDC->MoveTo(firBaseX + xStep * dataStep, firBaseY + firLineK * xStep * dataStep);
	pDC->LineTo(firBaseX - xStep * dataStep * sample.breakPointIndex[0], firBaseY - firLineK * xStep * dataStep * sample.breakPointIndex[0]);

	// 绘制拐弯点右段
	double forLineK = 0.0;
	for (int i = sample.breakPointIndex[0] + 1; i < sample.dataCount; i++)
	{
		double deltaX = GetShowPointX(sample.xDataArray[i], sample) - GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample);
		double deltaY = GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0) - GetShowPointY(sample.yDataArray[i] / 100.0);
		forLineK += deltaY / deltaX;
	}
	forLineK /= sample.dataCount - sample.breakPointIndex[0] - 2;
	pDC->MoveTo(firBaseX - xStep * dataStep, firBaseY - forLineK * xStep * dataStep);
	pDC->LineTo(firBaseX + xStep * dataStep * (sample.dataCount - sample.breakPointIndex[0] - 1), firBaseY + forLineK * xStep * dataStep * (sample.dataCount - sample.breakPointIndex[0] - 1));
}

void CGrainsizeAnalysisDlg::OldDrawTheAnalysisLine2(CDC *pDC, EachSample sample)
{
	// 不能有相同X轴的点
	if (sample.breakPointIndex[0] == sample.breakPointIndex[1] || sample.breakPointIndex[1] == sample.breakPointIndex[2])
	{
		return;
	}

	int firBaseX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample);
	int firBaseY = 0 - GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0);
	int secBaseX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[1]], sample);
	int secBaseY = 0 - GetShowPointY(sample.yDataArray[sample.breakPointIndex[1]] / 100.0);

	// 绘制第一个拐弯点左段
	double firLineK = 0.0;
	for (int i = 0; i < sample.breakPointIndex[0]; i++)
	{
		double deltaX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample) - GetShowPointX(sample.xDataArray[i], sample);
		double deltaY = GetShowPointY(sample.yDataArray[i] / 100.0) - GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0);
		firLineK += deltaY / deltaX;
	}
	firLineK /= sample.breakPointIndex[0];
	pDC->MoveTo(firBaseX + xStep * dataStep, firBaseY + firLineK * xStep * dataStep);
	pDC->LineTo(firBaseX - xStep * dataStep * sample.breakPointIndex[0], firBaseY - firLineK * xStep * dataStep * sample.breakPointIndex[0]);

	// 绘制第二个拐弯点右段
	double thrLineK = 0.0;
	for (int i = sample.breakPointIndex[1] + 1; i < sample.dataCount; i++)
	{
		double deltaX = GetShowPointX(sample.xDataArray[i], sample) - GetShowPointX(sample.xDataArray[sample.breakPointIndex[1]], sample);
		double deltaY = GetShowPointY(sample.yDataArray[sample.breakPointIndex[1]] / 100.0) - GetShowPointY(sample.yDataArray[i] / 100.0);
		thrLineK += deltaY / deltaX;
	}
	thrLineK /= sample.dataCount - sample.breakPointIndex[1] - 2;
	pDC->MoveTo(secBaseX - xStep * dataStep, secBaseY - thrLineK * xStep * dataStep);
	pDC->LineTo(secBaseX + xStep * dataStep * (sample.dataCount - sample.breakPointIndex[1] - 1), secBaseY + thrLineK * xStep * dataStep * (sample.dataCount - sample.breakPointIndex[1] - 1));

	// 连接中间线段
	double secDeltaX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[1]], sample) - GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample);
	double secDeltaY = GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0) - GetShowPointY(sample.yDataArray[sample.breakPointIndex[1]] / 100.0);
	double secLineK = secDeltaY / secDeltaX;
	pDC->MoveTo(firBaseX - xStep * dataStep, firBaseY - secLineK * xStep * dataStep);
	pDC->LineTo(firBaseX + xStep * dataStep * (sample.breakPointIndex[1] - sample.breakPointIndex[0] + 1), firBaseY + secLineK * xStep * dataStep * (sample.breakPointIndex[1] - sample.breakPointIndex[0] + 1));
}

void CGrainsizeAnalysisDlg::OldDrawTheAnalysisLine3(CDC *pDC, EachSample sample)
{
	// 不能有相同X轴的点
	if (sample.breakPointIndex[0] == sample.breakPointIndex[1] || sample.breakPointIndex[1] == sample.breakPointIndex[2] || sample.breakPointIndex[2] == sample.breakPointIndex[0])
	{
		return;
	}

	int firBaseX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample);
	int firBaseY = 0 - GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0);
	int secBaseX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[1]], sample);
	int secBaseY = 0 - GetShowPointY(sample.yDataArray[sample.breakPointIndex[1]] / 100.0);
	int thrBaseX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[2]], sample);
	int thrBaseY = 0 - GetShowPointY(sample.yDataArray[sample.breakPointIndex[2]] / 100.0);

	// 绘制第一个拐弯点左段
	double firLineK = 0.0;
	for (int i = 0; i < sample.breakPointIndex[0]; i++)
	{
		double deltaX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample) - GetShowPointX(sample.xDataArray[i], sample);
		double deltaY = GetShowPointY(sample.yDataArray[i] / 100.0) - GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0);
		firLineK += deltaY / deltaX;
	}
	firLineK /= sample.breakPointIndex[0];
	pDC->MoveTo(firBaseX + xStep * dataStep, firBaseY + firLineK * xStep * dataStep);
	pDC->LineTo(firBaseX - xStep * dataStep * sample.breakPointIndex[0], firBaseY - firLineK * xStep * dataStep * sample.breakPointIndex[0]);
	
	// 绘制第三个拐弯点右段
	double forLineK = 0.0;
	for (int i = sample.breakPointIndex[2] + 1; i < sample.dataCount; i++)
	{
		double deltaX = GetShowPointX(sample.xDataArray[i], sample) - GetShowPointX(sample.xDataArray[sample.breakPointIndex[2]], sample);
		double deltaY = GetShowPointY(sample.yDataArray[sample.breakPointIndex[2]] / 100.0) - GetShowPointY(sample.yDataArray[i] / 100.0);
		forLineK += deltaY / deltaX;
	}
	forLineK /= sample.dataCount - sample.breakPointIndex[2] - 2;
	pDC->MoveTo(thrBaseX - xStep * dataStep, thrBaseY - forLineK * xStep * dataStep);
	pDC->LineTo(thrBaseX + xStep * dataStep * (sample.dataCount - sample.breakPointIndex[2] - 1), thrBaseY + forLineK * xStep * dataStep * (sample.dataCount - sample.breakPointIndex[2] - 1));

	// 连接中间两线段
	double secDeltaX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[1]], sample) - GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample);
	double secDeltaY = GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0) - GetShowPointY(sample.yDataArray[sample.breakPointIndex[1]] / 100.0);
	double secLineK = secDeltaY / secDeltaX;
	pDC->MoveTo(firBaseX - xStep * dataStep, firBaseY - secLineK * xStep * dataStep);
	pDC->LineTo(firBaseX + xStep * dataStep * (sample.breakPointIndex[1] - sample.breakPointIndex[0] + 1), firBaseY + secLineK * xStep * dataStep * (sample.breakPointIndex[1] - sample.breakPointIndex[0] + 1));

	double thrDeltaX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[2]], sample) - GetShowPointX(sample.xDataArray[sample.breakPointIndex[1]], sample);
	double thrDeltaY = GetShowPointY(sample.yDataArray[sample.breakPointIndex[1]] / 100.0) - GetShowPointY(sample.yDataArray[sample.breakPointIndex[2]] / 100.0);
	double thrLineK = thrDeltaY / thrDeltaX;
	pDC->MoveTo(secBaseX - xStep * dataStep, secBaseY - thrLineK * xStep * dataStep);
	pDC->LineTo(secBaseX + xStep * dataStep * (sample.breakPointIndex[2] - sample.breakPointIndex[1] + 1), secBaseY + thrLineK * xStep * dataStep * (sample.breakPointIndex[2] - sample.breakPointIndex[1] + 1));
}

void CGrainsizeAnalysisDlg::OldDrawTheAnalysisLine4(CDC *pDC, EachSample sample)
{
	// 不能有相同X轴的点
	if (sample.breakPointIndex[0] == sample.breakPointIndex[1] || sample.breakPointIndex[1] == sample.breakPointIndex[2] || sample.breakPointIndex[2] == sample.breakPointIndex[3] || sample.breakPointIndex[3] == sample.breakPointIndex[0])
	{
		return;
	}

	int firBaseX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample);
	int firBaseY = 0 - GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0);
	int secBaseX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[1]], sample);
	int secBaseY = 0 - GetShowPointY(sample.yDataArray[sample.breakPointIndex[1]] / 100.0);
	int thrBaseX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[2]], sample);
	int thrBaseY = 0 - GetShowPointY(sample.yDataArray[sample.breakPointIndex[2]] / 100.0);
	int forBaseX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[3]], sample);
	int forBaseY = 0 - GetShowPointY(sample.yDataArray[sample.breakPointIndex[3]] / 100.0);

	// 绘制第一个拐弯点左段
	double firLineK = 0.0;
	for (int i = 0; i < sample.breakPointIndex[0]; i++)
	{
		double deltaX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample) - GetShowPointX(sample.xDataArray[i], sample);
		double deltaY = GetShowPointY(sample.yDataArray[i] / 100.0) - GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0);
		firLineK += deltaY / deltaX;
	}
	firLineK /= sample.breakPointIndex[0];
	pDC->MoveTo(firBaseX + xStep * dataStep, firBaseY + firLineK * xStep * dataStep);
	pDC->LineTo(firBaseX - xStep * dataStep * sample.breakPointIndex[0], firBaseY - firLineK * xStep * dataStep * sample.breakPointIndex[0]);

	// 绘制第四个拐弯点右段
	double fifLineK = 0.0;
	for (int i = sample.breakPointIndex[3] + 1; i < sample.dataCount; i++)
	{
		double deltaX = GetShowPointX(sample.xDataArray[i], sample) - GetShowPointX(sample.xDataArray[sample.breakPointIndex[3]], sample);
		double deltaY = GetShowPointY(sample.yDataArray[sample.breakPointIndex[3]] / 100.0) - GetShowPointY(sample.yDataArray[i] / 100.0);
		fifLineK += deltaY / deltaX;
	}
	fifLineK /= sample.dataCount - sample.breakPointIndex[3] - 2;
	pDC->MoveTo(forBaseX - xStep * dataStep, forBaseY - fifLineK * xStep * dataStep);
	pDC->LineTo(forBaseX + xStep * dataStep * (sample.dataCount - sample.breakPointIndex[3] - 1), forBaseY + fifLineK * xStep * dataStep * (sample.dataCount - sample.breakPointIndex[3] - 1));

	// 连接中间三线段
	double secDeltaX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[1]], sample) - GetShowPointX(sample.xDataArray[sample.breakPointIndex[0]], sample);
	double secDeltaY = GetShowPointY(sample.yDataArray[sample.breakPointIndex[0]] / 100.0) - GetShowPointY(sample.yDataArray[sample.breakPointIndex[1]] / 100.0);
	double secLineK = secDeltaY / secDeltaX;
	pDC->MoveTo(firBaseX - xStep * dataStep, firBaseY - secLineK * xStep * dataStep);
	pDC->LineTo(firBaseX + xStep * dataStep * (sample.breakPointIndex[1] - sample.breakPointIndex[0] + 1), firBaseY + secLineK * xStep * dataStep * (sample.breakPointIndex[1] - sample.breakPointIndex[0] + 1));

	double thrDeltaX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[2]], sample) - GetShowPointX(sample.xDataArray[sample.breakPointIndex[1]], sample);
	double thrDeltaY = GetShowPointY(sample.yDataArray[sample.breakPointIndex[1]] / 100.0) - GetShowPointY(sample.yDataArray[sample.breakPointIndex[2]] / 100.0);
	double thrLineK = thrDeltaY / thrDeltaX;
	pDC->MoveTo(secBaseX - xStep * dataStep, secBaseY - thrLineK * xStep * dataStep);
	pDC->LineTo(secBaseX + xStep * dataStep * (sample.breakPointIndex[2] - sample.breakPointIndex[1] + 1), secBaseY + thrLineK * xStep * dataStep * (sample.breakPointIndex[2] - sample.breakPointIndex[1] + 1));

	double forDeltaX = GetShowPointX(sample.xDataArray[sample.breakPointIndex[3]], sample) - GetShowPointX(sample.xDataArray[sample.breakPointIndex[2]], sample);
	double forDeltaY = GetShowPointY(sample.yDataArray[sample.breakPointIndex[2]] / 100.0) - GetShowPointY(sample.yDataArray[sample.breakPointIndex[3]] / 100.0);
	double forLineK = forDeltaY / forDeltaX;
	pDC->MoveTo(thrBaseX - xStep * dataStep, thrBaseY - forLineK * xStep * dataStep);
	pDC->LineTo(thrBaseX + xStep * dataStep * (sample.breakPointIndex[3] - sample.breakPointIndex[2] + 1), thrBaseY + forLineK * xStep * dataStep * (sample.breakPointIndex[3] - sample.breakPointIndex[2] + 1));
}

void CGrainsizeAnalysisDlg::LineFitting(double x[], double y[], int begin, int end, double *K, double *B)
{
	double xmean = 0.0;
	double ymean = 0.0;
	for (int i = begin; i < end; i++)
	{
		xmean += x[i];
		ymean += y[i];
	}
	xmean /= (double)(end - begin);
	ymean /= (double)(end - begin);

	double sumx2 = 0.0;
	double sumxy = 0.0;
	for (int i = begin; i < end; i++)
	{
		sumx2 += (x[i] - xmean) * (x[i] - xmean);
		sumxy += (x[i] - xmean) * (y[i] - ymean);
	}
	*K = sumxy / sumx2;
	*B = ymean - *K * xmean;
}

void CGrainsizeAnalysisDlg::DrawTheAnalysisLine1(CDC *pDC, EachSample sample)
{
	// 不能有相同X轴的点
	if (sample.breakPointIndex[0] == sample.breakPointIndex[1])
	{
		return;
	}
	// 使用绘图坐标点进行线性回归
	double xp[128], yp[128];
	for (int i = 0; i < sample.dataCount; i++)
	{
		xp[i] = GetShowPointX(sample.xDataArray[i], sample);
		yp[i] = 0 - GetShowPointY(sample.yDataArray[i] / 100);
	}
	// 第一条线
	double K1, B1;
	this->LineFitting(xp, yp, 0, sample.breakPointIndex[0], &K1, &B1);
	pDC->MoveTo(0, (int)(B1));
	pDC->LineTo(xStep * dataStep * (sample.breakPointIndex[0] + 1), (int)(xStep * dataStep * (sample.breakPointIndex[0] + 1) * K1 + B1));
	// 第二条线
	double K2, B2;
	this->LineFitting(xp, yp, sample.breakPointIndex[0], sample.dataCount - 1, &K2, &B2);
	pDC->MoveTo(xStep * dataStep * (sample.breakPointIndex[0] - 1), (int)(xStep * dataStep * (sample.breakPointIndex[0] - 1) * K2 + B2));
	pDC->LineTo(xp[sample.dataCount - 1], (int)(xp[sample.dataCount - 1] * K2 + B2));
	
	// 标记焦点
	double xP1, yP1, x1, y1;
	this->GetFocusPoint(K1, B1, K2, B2, &xP1, &yP1);
	this->ShowPointToValue(xP1, yP1, &x1, &y1, sample);
	CString tps;
	tps.Format(_T("(%.2f, %.2f)"), x1, y1);
	pDC->TextOutW((int)xP1 + 6, (int)yP1 + 6, tps);
}

void CGrainsizeAnalysisDlg::DrawTheAnalysisLine2(CDC *pDC, EachSample sample)
{
	// 不能有相同X轴的点
	if (sample.breakPointIndex[0] == sample.breakPointIndex[1] || sample.breakPointIndex[1] == sample.breakPointIndex[2])
	{
		return;
	}
	// 使用绘图坐标点进行线性回归
	double xp[128], yp[128];
	for (int i = 0; i < sample.dataCount; i++)
	{
		xp[i] = GetShowPointX(sample.xDataArray[i], sample);
		yp[i] = 0 - GetShowPointY(sample.yDataArray[i] / 100);
	}
	// 第一条线
	double K1, B1;
	this->LineFitting(xp, yp, 0, sample.breakPointIndex[0], &K1, &B1);
	pDC->MoveTo(0, (int)(B1));
	pDC->LineTo(xStep * dataStep * (sample.breakPointIndex[0] + 1), (int)(xStep * dataStep * (sample.breakPointIndex[0] + 1) * K1 + B1));
	// 第二条线
	double K2, B2;
	this->LineFitting(xp, yp, sample.breakPointIndex[0], sample.breakPointIndex[1], &K2, &B2);
	pDC->MoveTo(xStep * dataStep * (sample.breakPointIndex[0] - 2), (int)(xStep * dataStep * (sample.breakPointIndex[0] - 2) * K2 + B2));
	pDC->LineTo(xStep * dataStep * (sample.breakPointIndex[1] + 2), (int)(xStep * dataStep * (sample.breakPointIndex[1] + 2) * K2 + B2));
	// 第三条线
	double K3, B3;
	this->LineFitting(xp, yp, sample.breakPointIndex[1], sample.dataCount - 1, &K3, &B3);
	pDC->MoveTo(xStep * dataStep * (sample.breakPointIndex[1] - 1), (int)(xStep * dataStep * (sample.breakPointIndex[1] - 1) * K3 + B3));
	pDC->LineTo(xp[sample.dataCount - 1], (int)(xp[sample.dataCount - 1] * K3 + B3));

	// 标记焦点
	double xP1, yP1, x1, y1;
	this->GetFocusPoint(K1, B1, K2, B2, &xP1, &yP1);
	this->ShowPointToValue(xP1, yP1, &x1, &y1, sample);
	CString tps;
	tps.Format(_T("(%.2f, %.2f)"), x1, y1);
	pDC->TextOutW((int)xP1 + 8, (int)yP1 + 6, tps);

	double xP2, yP2, x2, y2;
	this->GetFocusPoint(K2, B2, K3, B3, &xP2, &yP2);
	this->ShowPointToValue(xP2, yP2, &x2, &y2, sample);
	tps.Format(_T("(%.2f, %.2f)"), x2, y2);
	pDC->TextOutW((int)xP2 - 30, (int)yP2 + 15, tps);
}

void CGrainsizeAnalysisDlg::DrawTheAnalysisLine3(CDC *pDC, EachSample sample)
{
	// 不能有相同X轴的点
	if (sample.breakPointIndex[0] == sample.breakPointIndex[1] || sample.breakPointIndex[1] == sample.breakPointIndex[2] || sample.breakPointIndex[2] == sample.breakPointIndex[0])
	{
		return;
	}
	// 使用绘图坐标点进行线性回归
	double xp[128], yp[128];
	for (int i = 0; i < sample.dataCount; i++)
	{
		xp[i] = GetShowPointX(sample.xDataArray[i], sample);
		yp[i] = 0 - GetShowPointY(sample.yDataArray[i] / 100);
	}
	// 第一条线
	double K1, B1;
	this->LineFitting(xp, yp, 0, sample.breakPointIndex[0], &K1, &B1);
	pDC->MoveTo(0, (int)(B1));
	pDC->LineTo(xStep * dataStep * (sample.breakPointIndex[0] + 1), (int)(xStep * dataStep * (sample.breakPointIndex[0] + 1) * K1 + B1));
	// 第二条线
	double K2, B2;
	this->LineFitting(xp, yp, sample.breakPointIndex[0], sample.breakPointIndex[1], &K2, &B2);
	pDC->MoveTo(xStep * dataStep * (sample.breakPointIndex[0] - 2), (int)(xStep * dataStep * (sample.breakPointIndex[0] - 2) * K2 + B2));
	pDC->LineTo(xStep * dataStep * (sample.breakPointIndex[1] + 2), (int)(xStep * dataStep * (sample.breakPointIndex[1] + 2) * K2 + B2));
	// 第三条线
	double K3, B3;
	this->LineFitting(xp, yp, sample.breakPointIndex[1], sample.breakPointIndex[2], &K3, &B3);
	pDC->MoveTo(xStep * dataStep * (sample.breakPointIndex[1] - 2), (int)(xStep * dataStep * (sample.breakPointIndex[1] - 2) * K3 + B3));
	pDC->LineTo(xStep * dataStep * (sample.breakPointIndex[2] + 2), (int)(xStep * dataStep * (sample.breakPointIndex[2] + 2) * K3 + B3));
	// 第四条线
	double K4, B4;
	this->LineFitting(xp, yp, sample.breakPointIndex[2], sample.dataCount - 1, &K4, &B4);
	pDC->MoveTo(xStep * dataStep * (sample.breakPointIndex[2] - 1), (int)(xStep * dataStep * (sample.breakPointIndex[2] - 1) * K4 + B4));
	pDC->LineTo(xp[sample.dataCount - 1], (int)(xp[sample.dataCount - 1] * K4 + B4));
	
	// 标记焦点
	double xP1, yP1, x1, y1;
	this->GetFocusPoint(K1, B1, K2, B2, &xP1, &yP1);
	this->ShowPointToValue(xP1, yP1, &x1, &y1, sample);
	CString tps;
	tps.Format(_T("(%.2f, %.2f)"), x1, y1);
	pDC->TextOutW((int)xP1 + 8, (int)yP1 + 6, tps);

	double xP2, yP2, x2, y2;
	this->GetFocusPoint(K2, B2, K3, B3, &xP2, &yP2);
	this->ShowPointToValue(xP2, yP2, &x2, &y2, sample);
	tps.Format(_T("(%.2f, %.2f)"), x2, y2);
	pDC->TextOutW((int)xP2 + 8, (int)yP2 + 6, tps);

	double xP3, yP3, x3, y3;
	this->GetFocusPoint(K3, B3, K4, B4, &xP3, &yP3);
	this->ShowPointToValue(xP3, yP3, &x3, &y3, sample);
	tps.Format(_T("(%.2f, %.2f)"), x3, y3);
	pDC->TextOutW((int)xP3 - 30, (int)yP3 + 15, tps);
}

void CGrainsizeAnalysisDlg::DrawTheAnalysisLine4(CDC *pDC, EachSample sample)
{
	// 不能有相同X轴的点
	if (sample.breakPointIndex[0] == sample.breakPointIndex[1] || sample.breakPointIndex[1] == sample.breakPointIndex[2] || sample.breakPointIndex[2] == sample.breakPointIndex[3] || sample.breakPointIndex[3] == sample.breakPointIndex[0])
	{
		return;
	}
	// 使用绘图坐标点进行线性回归
	double xp[128], yp[128];
	for (int i = 0; i < sample.dataCount; i++)
	{
		xp[i] = GetShowPointX(sample.xDataArray[i], sample);
		yp[i] = 0 - GetShowPointY(sample.yDataArray[i] / 100);
	}
	// 第一条线
	double K1, B1;
	this->LineFitting(xp, yp, 0, sample.breakPointIndex[0], &K1, &B1);
	pDC->MoveTo(0, (int)(B1));
	pDC->LineTo(xStep * dataStep * (sample.breakPointIndex[0] + 1), (int)(xStep * dataStep * (sample.breakPointIndex[0] + 1) * K1 + B1));
	// 第二条线
	double K2, B2;
	this->LineFitting(xp, yp, sample.breakPointIndex[0], sample.breakPointIndex[1], &K2, &B2);
	pDC->MoveTo(xStep * dataStep * (sample.breakPointIndex[0] - 2), (int)(xStep * dataStep * (sample.breakPointIndex[0] - 2) * K2 + B2));
	pDC->LineTo(xStep * dataStep * (sample.breakPointIndex[1] + 2), (int)(xStep * dataStep * (sample.breakPointIndex[1] + 2) * K2 + B2));
	// 第三条线
	double K3, B3;
	this->LineFitting(xp, yp, sample.breakPointIndex[1], sample.breakPointIndex[2], &K3, &B3);
	pDC->MoveTo(xStep * dataStep * (sample.breakPointIndex[1] - 2), (int)(xStep * dataStep * (sample.breakPointIndex[1] - 2) * K3 + B3));
	pDC->LineTo(xStep * dataStep * (sample.breakPointIndex[2] + 2), (int)(xStep * dataStep * (sample.breakPointIndex[2] + 2) * K3 + B3));
	// 第四条线
	double K4, B4;
	this->LineFitting(xp, yp, sample.breakPointIndex[2], sample.breakPointIndex[3], &K4, &B4);
	pDC->MoveTo(xStep * dataStep * (sample.breakPointIndex[2] - 2), (int)(xStep * dataStep * (sample.breakPointIndex[2] - 2) * K4 + B4));
	pDC->LineTo(xStep * dataStep * (sample.breakPointIndex[3] + 2), (int)(xStep * dataStep * (sample.breakPointIndex[3] + 2) * K4 + B4));
	// 第五条线
	double K5, B5;
	this->LineFitting(xp, yp, sample.breakPointIndex[3], sample.dataCount - 1, &K5, &B5);
	pDC->MoveTo(xStep * dataStep * (sample.breakPointIndex[3] - 1), (int)(xStep * dataStep * (sample.breakPointIndex[3] - 1) * K5 + B5));
	pDC->LineTo(xp[sample.dataCount - 1], (int)(xp[sample.dataCount - 1] * K5 + B5));

	// 标记焦点
	double xP1, yP1, x1, y1;
	this->GetFocusPoint(K1, B1, K2, B2, &xP1, &yP1);
	this->ShowPointToValue(xP1, yP1, &x1, &y1, sample);
	CString tps;
	tps.Format(_T("(%.2f, %.2f)"), x1, y1);
	pDC->TextOutW((int)xP1 + 8, (int)yP1 + 6, tps);

	double xP2, yP2, x2, y2;
	this->GetFocusPoint(K2, B2, K3, B3, &xP2, &yP2);
	this->ShowPointToValue(xP2, yP2, &x2, &y2, sample);
	tps.Format(_T("(%.2f, %.2f)"), x2, y2);
	pDC->TextOutW((int)xP2 + 8, (int)yP2 + 6, tps);

	double xP3, yP3, x3, y3;
	this->GetFocusPoint(K3, B3, K4, B4, &xP3, &yP3);
	this->ShowPointToValue(xP3, yP3, &x3, &y3, sample);
	tps.Format(_T("(%.2f, %.2f)"), x3, y3);
	pDC->TextOutW((int)xP3 + 8, (int)yP3 + 6, tps);

	double xP4, yP4, x4, y4;
	this->GetFocusPoint(K4, B4, K5, B5, &xP4, &yP4);
	this->ShowPointToValue(xP4, yP4, &x4, &y4, sample);
	tps.Format(_T("(%.2f, %.2f)"), x4, y4);
	pDC->TextOutW((int)xP4 - 30, (int)yP4 + 15, tps);
}

void CGrainsizeAnalysisDlg::DrawSizeDitribution(CDC *pDC, CRect rect, EachSample sample)
{
	pDC->SetViewportOrg(rect.right - MARGIN - GS_WIDTH, YLEN + 10 - GS_HEIGHT); //设置坐标原点

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(66, 66, 66));
	pDC->SelectObject(pen);
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));
	pDC->SelectObject(brush);
	//pDC->MoveTo(0, 0);
	//pDC->Rectangle(0, 0, GS_WIDTH + 1, GS_HEIGHT + 1);
	pDC->SetViewportOrg(rect.right - MARGIN - GS_WIDTH + GS_MARGIN, YLEN + 10 - GS_MARGIN); //设置坐标原点到矩形框左下角
	// 绘制坐标轴
	CFont font;
	font.CreateFontW(12, 0, 0, 0, 400, FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial"));
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(22, 22, 22));

	CPen myBPen, mySPen, mySubPen;
	myBPen.CreatePen(PS_SOLID, 2, RGB(33, 33, 33));
	mySPen.CreatePen(PS_SOLID, 1, RGB(33, 33, 33));
	mySubPen.CreatePen(PS_DOT, 1, RGB(66, 66, 66));

	pDC->SelectObject(font);
	pDC->SelectObject(mySPen);
	// 横轴
	CString tempString;
	if (sample.dataCount == 0)
	{
		pDC->MoveTo(0, 0);
		pDC->LineTo(GetGSShowPointX(16, sample), 0);
		double mainScaleX[5] = { 0, 4, 8, 12, 16 };
		for (int i = 0; i < 5; i++)
		{
			int xt = GetGSShowPointX(mainScaleX[i], sample);
			pDC->MoveTo(xt, 0);
			pDC->LineTo(xt, 8);
			tempString.Format(_T("%.0f"), mainScaleX[i]);
			pDC->TextOutW(xt - 3, 8, tempString);
		}
		double subScaleX[12] = { 1, 2, 3, 5, 6, 7, 9, 10, 11, 13, 14, 15 };
		for (int i = 0; i < 12; i++)
		{
			pDC->MoveTo(GetGSShowPointX(subScaleX[i], sample), 0);
			pDC->LineTo(GetGSShowPointX(subScaleX[i], sample), 5);
		}
	}
	else
	{
		pDC->MoveTo(0, 0);
		pDC->LineTo(GetGSShowPointX(sample.xDataArray[sample.dataCount - 1], sample), 0);
		int c_x, b_x;
		for (int i = 0; i < sample.dataCount; i++)
		{
			c_x = GetGSShowPointX(sample.xDataArray[i], sample);
			pDC->MoveTo(c_x, 0);
			pDC->LineTo(c_x, 5);
			if (abs((sample.xDataArray[i] + 0.001) - (int)(sample.xDataArray[i] + 0.001)) < 0.01 && (int)(sample.xDataArray[i] + 0.001) % 2 == 0)
			{
				pDC->MoveTo(c_x, 0);
				pDC->LineTo(c_x, 8);
				tempString.Format(_T("%.0f"), sample.xDataArray[i]);
				pDC->TextOutW(c_x - 3, 8, tempString);
			}
		}
	}
	// 纵轴
	// 左纵轴
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, GetGSShowPointY(10));
	// 开始绘制刻度
	double mainScaleY[6] = { 0, 2, 4, 6, 8, 10 };
	for (int i = 0; i < 6; i++)
	{
		// 左主刻度
		tempString.Format(_T("%.0f"), mainScaleY[i]);
		pDC->MoveTo(0, GetGSShowPointY(mainScaleY[i]));
		pDC->LineTo(-8, GetGSShowPointY(mainScaleY[i]));
		pDC->TextOutW(-20, GetGSShowPointY(mainScaleY[i]) - 6, tempString);
	}
	double subScaleY[15] = { 0.5, 1.0, 1.5, 2.5, 3.0, 3.5, 4.5, 5, 5.5, 6.5, 7.0, 7.5, 8.5, 9.0, 9.5 };
	for (int i = 0; i < 15; i++)
	{
		// 左副刻度
		pDC->MoveTo(0, GetGSShowPointY(subScaleY[i]));
		pDC->LineTo(-5, GetGSShowPointY(subScaleY[i]));
	}
	// 绘制曲线
	CPen gravelPen, sandPen, siltPen, clayPen;
	gravelPen.CreatePen(PS_SOLID, 2, BS_GRAVEL);
	sandPen.CreatePen(PS_SOLID, 2, BS_SAND);
	siltPen.CreatePen(PS_SOLID, 2, BS_SILT);
	clayPen.CreatePen(PS_SOLID, 2, BS_CLAY);
	pDC->SelectObject(myBPen);
	int x = GetGSShowPointX(sample.xDataArray[0], sample);
	int y = GetGSShowPointY(sample.sizeDistribution[0]);
	pDC->MoveTo(x, y);
	for (int i = 0; i < sample.dataCount; i++)
	{
		if (sample.xDataArray[i] < -1.001)
		{
			pDC->SelectObject(gravelPen);
		}
		else if (sample.xDataArray[i] >= -1.001 && sample.xDataArray[i] < 4.001)
		{
			pDC->SelectObject(sandPen);
		}
		else if (sample.xDataArray[i] >= 4.001 && sample.xDataArray[i] < 8.001)
		{
			pDC->SelectObject(siltPen);
		}
		else
		{
			pDC->SelectObject(clayPen);
		}
		x = GetGSShowPointX(sample.xDataArray[i],sample);
		y = GetGSShowPointY(sample.sizeDistribution[i]);
		pDC->LineTo(x, y);
	}
	// 绘制断点
	pDC->SelectObject(mySPen);
	CBrush breakPointBrash;
	breakPointBrash.CreateSolidBrush(RGB(255, 0, 0));
	pDC->SelectObject(breakPointBrash);
	for (int i = 0; i < sample.breakPointCount; i++)
	{
		if (sample.breakPointIndex[i] > 0) // 非默认点，才显示
		{
			double bx = sample.xDataArray[sample.breakPointIndex[i]];
			double by = sample.sizeDistribution[sample.breakPointIndex[i]];
			int xp = GetGSShowPointX(bx, sample);
			int yp = GetGSShowPointY(by);
			pDC->Ellipse(xp - 4, yp - 4, xp + 4, yp + 4);
		}
	}
	// 右、上框线
	pDC->SelectObject(mySubPen);
	pDC->MoveTo(GS_WIDTH - 2 * GS_MARGIN, 0);
	pDC->LineTo(GS_WIDTH - 2 * GS_MARGIN, GetGSShowPointY(10));
	pDC->LineTo(0, GetGSShowPointY(10));
}

void CGrainsizeAnalysisDlg::OnCbnSelchangeComboNumber()
{
	// TODO:  修改默认转折点数并界面响应
	// 加载当前要显示的岩样
	EachSample *pCurrenttSample = &sampleArray[currentSample];

	int curSelectNumber = this->m_NumberSelectControl.GetCurSel();
	if (curSelectNumber != pCurrenttSample->breakPointCount - 1)
	{
		switch (curSelectNumber)
		{
		case 0:
			GetDlgItem(IDC_COMBO_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_2ND)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_3RD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_4TH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_2ND)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_3RD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_4TH)->ShowWindow(SW_HIDE);
			pCurrenttSample->breakPointCount = 1;
			pCurrenttSample->breakPointIndex[1] = 127;
			pCurrenttSample->breakPointIndex[2] = 127;
			pCurrenttSample->breakPointIndex[3] = 127;
			this->SortBreakPoint(pCurrenttSample);
			m_firBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[0]);
			break;
		case 1:
			GetDlgItem(IDC_COMBO_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_3RD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_4TH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_3RD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_4TH)->ShowWindow(SW_HIDE);
			this->GetBreakPointIndex(pCurrenttSample);
			pCurrenttSample->breakPointCount = 2;
			pCurrenttSample->breakPointIndex[2] = 127;
			pCurrenttSample->breakPointIndex[3] = 127;
			this->SortBreakPoint(pCurrenttSample);
			m_firBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[0]);
			m_secBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[1]);
			break;
		case 2:
			GetDlgItem(IDC_COMBO_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_3RD)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_4TH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_3RD)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_4TH)->ShowWindow(SW_HIDE);
			this->GetBreakPointIndex(pCurrenttSample);
			pCurrenttSample->breakPointCount = 3;
			pCurrenttSample->breakPointIndex[3] = 127;
			this->SortBreakPoint(pCurrenttSample);
			m_firBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[0]);
			m_secBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[1]);
			m_thrBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[2]);
			break;
		case 3:
			GetDlgItem(IDC_COMBO_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_3RD)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_4TH)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_3RD)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_4TH)->ShowWindow(SW_SHOW);
			this->GetBreakPointIndex(pCurrenttSample);
			pCurrenttSample->breakPointCount = 4;
			this->SortBreakPoint(pCurrenttSample);
			m_firBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[0]);
			m_secBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[1]);
			m_thrBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[2]);
			m_forBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[3]);
			break;
		default:
			break;
		}
		this->fileIsModified = true;
	}
	else
	{
		switch (pCurrenttSample->breakPointCount - 1)
		{
		case 0:
			GetDlgItem(IDC_COMBO_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_2ND)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_3RD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_4TH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_2ND)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_3RD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_4TH)->ShowWindow(SW_HIDE);
			m_firBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[0]);
			break;
		case 1:
			GetDlgItem(IDC_COMBO_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_3RD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_4TH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_3RD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_4TH)->ShowWindow(SW_HIDE);
			m_firBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[0]);
			m_secBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[1]);
			break;
		case 2:
			GetDlgItem(IDC_COMBO_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_3RD)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_4TH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_3RD)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_4TH)->ShowWindow(SW_HIDE);
			m_firBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[0]);
			m_secBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[1]);
			m_thrBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[2]);
			break;
		case 3:
			GetDlgItem(IDC_COMBO_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_3RD)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_4TH)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_1ST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_2ND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_3RD)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_4TH)->ShowWindow(SW_SHOW);
			m_firBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[0]);
			m_secBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[1]);
			m_thrBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[2]);
			m_forBreakPointComboControl.SetCurSel(pCurrenttSample->breakPointIndex[3]);
			break;
		default:
			break;
		}
	}
	
	this->Invalidate();
}

void CGrainsizeAnalysisDlg::SortBreakPoint(EachSample *pSample)
{
	// 冒泡排序
	for (int j = 0; j < pSample->breakPointCount; j++)
	{
		for (int i = 0; i < pSample->breakPointCount - 1; i++)
		{
			if (pSample->breakPointIndex[i + 1] < pSample->breakPointIndex[i])
			{
				pSample->breakPointIndex[i] = pSample->breakPointIndex[i] + pSample->breakPointIndex[i + 1];
				pSample->breakPointIndex[i + 1] = pSample->breakPointIndex[i] - pSample->breakPointIndex[i + 1];
				pSample->breakPointIndex[i] = pSample->breakPointIndex[i] - pSample->breakPointIndex[i + 1];
			}
		}
	}
}

void CGrainsizeAnalysisDlg::OnCbnSelchangeCombo1st()
{
	// TODO:  在此添加控件通知处理程序代码
	// 加载当前要显示的岩样
	EachSample *pCurrenttSample = &sampleArray[currentSample];
	int curSelectNumber = this->m_firBreakPointComboControl.GetCurSel();
	pCurrenttSample->breakPointIndex[0] = curSelectNumber;
	this->SortBreakPoint(pCurrenttSample);
	this->fileIsModified = true;
	this->Invalidate();
}

void CGrainsizeAnalysisDlg::OnCbnSelchangeCombo2nd()
{
	// TODO:  在此添加控件通知处理程序代码
	// 加载当前要显示的岩样
	EachSample *pCurrenttSample = &sampleArray[currentSample];
	int curSelectNumber = this->m_secBreakPointComboControl.GetCurSel();
	pCurrenttSample->breakPointIndex[1] = curSelectNumber;
	this->SortBreakPoint(pCurrenttSample);
	this->fileIsModified = true;
	this->Invalidate();
}

void CGrainsizeAnalysisDlg::OnCbnSelchangeCombo3rd()
{
	// TODO:  在此添加控件通知处理程序代码
	// 加载当前要显示的岩样
	EachSample *pCurrenttSample = &sampleArray[currentSample];
	int curSelectNumber = this->m_thrBreakPointComboControl.GetCurSel();
	pCurrenttSample->breakPointIndex[2] = curSelectNumber;
	this->SortBreakPoint(pCurrenttSample);
	this->fileIsModified = true;
	this->Invalidate();
}

void CGrainsizeAnalysisDlg::OnCbnSelchangeCombo4th()
{
	// TODO:  在此添加控件通知处理程序代码
	// 加载当前要显示的岩样
	EachSample *pCurrenttSample = &sampleArray[currentSample];
	int curSelectNumber = this->m_forBreakPointComboControl.GetCurSel();
	pCurrenttSample->breakPointIndex[3] = curSelectNumber;
	this->SortBreakPoint(pCurrenttSample);
	this->fileIsModified = true;
	this->Invalidate();
}

void CGrainsizeAnalysisDlg::OnCbnSelchangeComboSample()
{
	// 先保存当前样品基础信息
	this->SaveSampleInfo();

	// 样品下拉框选择
	int curSelectNumber = this->m_SampleListControl.GetCurSel();
	currentSample = curSelectNumber;
	this->m_NumberSelectControl.SetCurSel(sampleArray[currentSample].breakPointCount - 1);
	this->OnCbnSelchangeComboNumber();
	this->SetDataStep(sampleArray[currentSample]); // 设置X坐标单元长
	this->SortBreakPoint(&sampleArray[currentSample]);
	this->AddData2Combo();
	this->ShowSampleInfo();
	this->ShowRawData();
	this->Invalidate(); // 刷新界面
}

void CGrainsizeAnalysisDlg::AddData2Combo()
{
	// 将原始数据添加到下拉框
	m_firBreakPointComboControl.ResetContent();
	m_secBreakPointComboControl.ResetContent();
	m_thrBreakPointComboControl.ResetContent();
	m_forBreakPointComboControl.ResetContent();
	EachSample sample = sampleArray[currentSample];
	CString ts;
	for (int i = 0; i <sample.dataCount; i++)
	{
		ts.Format(_T("%d: D=%.2f"), i + 1, sample.xDataArray[i]);
		m_firBreakPointComboControl.AddString(ts);
		m_secBreakPointComboControl.AddString(ts);
		m_thrBreakPointComboControl.AddString(ts);
		m_forBreakPointComboControl.AddString(ts);
	}
	m_firBreakPointComboControl.SetCurSel(sample.breakPointIndex[0]);
	m_secBreakPointComboControl.SetCurSel(sample.breakPointIndex[1]);
	m_thrBreakPointComboControl.SetCurSel(sample.breakPointIndex[2]);
	m_forBreakPointComboControl.SetCurSel(sample.breakPointIndex[3]);
}

void CGrainsizeAnalysisDlg::OnAboutAboutgrainsizeanalysis()
{
	MessageBox(_T("该软件用于各类沉积物粒度分析，尤其是粒度分析中概率累积曲线的自动绘制、截点智能识别显示与分段线性拟合等，使用过程中如有任何问题请联系我们。 \n作者： 陈忠，许安涛，舒婵，颜文等\nE-mail: xuantao193@163.com"), _T("关于粒度概率累积曲线分析软件"), MB_ICONINFORMATION);
}

void CGrainsizeAnalysisDlg::OnAboutHowtouse()
{
	MessageBox(_T("1. 导入CSV格式的原始数据； \n2. 点击 \"<\" 或者 \">\" 进行选择； \n3. 选择 \"截点数量\" 下拉按钮，对截点数据进行选择； \n4. 可将经纬度、水深等数据填入，并通过导出选项导出到CSV文件中。 "), _T("如何使用"), MB_ICONQUESTION);
}

void CGrainsizeAnalysisDlg::OnMenuQuit()
{
	if (fileIsModified)
	{
		if (MessageBox(_T("File has been modified, check \"Yes\" to save it or \"No\" to ignore it. "), _T("Notice"), MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		{
			this->OnMenuSave();
		}
		else
		{
			CDialog::OnOK();
		}
	}
	else
	{
		CDialog::OnOK();
	}
}

void CGrainsizeAnalysisDlg::OnToolPrevious()
{
	// TODO:  在此添加命令处理程序代码
	this->OnBnClickedBtnBefor();
}

void CGrainsizeAnalysisDlg::OnToolNext()
{
	// TODO:  在此添加命令处理程序代码
	this->OnBnClickedBtnNext();
}

void CGrainsizeAnalysisDlg::OnMenuNew()
{
	// TODO:  在此添加命令处理程序代码
	if (sampleArray.GetCount() != 1 && sampleArray.GetCount() != 0)
	{
		if (fileIsModified)
		{
			if (MessageBox(_T("The current data is not saved, Ignore it? "), _T("Notice"), MB_ICONEXCLAMATION | MB_YESNO) == IDNO)
				return;
		}
		else if (MessageBox(_T("The current data is not empty, Ignore it? "), _T("Notice"), MB_ICONEXCLAMATION | MB_YESNO) == IDNO)
			return;
	}
	CFileDialog fileDlg(true, _T("gsa"), _T("New GSA File"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("GSA File(*.gsa)|*.gsa|All Files(*.*)|*.*||"), this);
	fileDlg.m_ofn.lpstrTitle = _T("New");
	if (fileDlg.DoModal() == IDOK)
	{
		this->OnToolCleandata();
		this->filePath = fileDlg.GetFolderPath() + "\\" + fileDlg.GetFileName();
	}
}

void CGrainsizeAnalysisDlg::OnMenuOpen()
{
	// TODO: 打开
	CFileDialog fileDlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("GSA File(*.gsa)|*.gsa|All Files(*.*)|*.*||"), this);
	fileDlg.m_ofn.lpstrTitle = _T("Open");
	if (fileDlg.DoModal() == IDOK)
	{
		this->filePath = fileDlg.GetFolderPath() + "\\" + fileDlg.GetFileName();
		CFile file;
		if (file.Open(this->filePath, CFile::modeRead))
		{
			this->sampleArray.RemoveAll();
			m_SampleListControl.ResetContent();
			char buf[1024];
			CArchive ar(&file, CArchive::load, 1024, buf);
			ar >> this->sampleCount;
			for (int i = 0; i < this->sampleCount; i++)
			{
				EachSample e_sample;
				ar >> e_sample.averageSize;
				ar >> e_sample.breakPointCount;
				for (int j = 0; j < 4; j++)
					ar >> e_sample.breakPointIndex[j];
				ar >> e_sample.clay;
				ar >> e_sample.dataCount;
				ar >> e_sample.deep;
				ar >> e_sample.gravel;
				ar >> e_sample.kurtosisCoefficient;
				ar >> e_sample.latitude;
				ar >> e_sample.longitude;
				ar >> e_sample.mediumSize;
				for (int j = 0; j < 128 + 2 * SPAN; j++)
					ar >> e_sample.normalY[j];
				ar >> e_sample.positionNum;
				ar >> e_sample.sampleID;
				ar >> e_sample.sand;
				ar >> e_sample.silt;
				for (int j = 0; j < 128; j++)
					ar >> e_sample.sizeDistribution[j];
				ar >> e_sample.skewnessCoefficient;
				ar >> e_sample.sortingCoefficient;
				for (int j = 0; j < 128; j++)
					ar >> e_sample.suspectBreakpoint[j];
				ar >> e_sample.suspectBreakpointCount;
				for (int j = 0; j < 128; j++)
					ar >> e_sample.xDataArray[j];
				for (int j = 0; j < 128; j++)
					ar >> e_sample.yDataArray[j];
				this->sampleArray.Add(e_sample);
				this->m_SampleListControl.AddString(e_sample.sampleID);
			}
			file.Close();
			this->fileIsModified = false;
			this->sampleCount = sampleArray.GetCount();
			this->currentSample = 0;
			this->SetDataStep(sampleArray[currentSample]); // 设置X坐标单元长，默认显示第一个岩样
			this->AddData2Combo();
			this->ShowSampleInfo();
			this->ShowRawData();
			this->Invalidate(); // 刷新界面
		}
	}
}

void CGrainsizeAnalysisDlg::OnMenuSave()
{
	// TODO: 保存
	if (this->sampleCount <= 1)
	{
		MessageBox(_T("Only one sample data can not be saved. "), _T("Notice"), MB_ICONASTERISK);
		return;
	}
	if (this->filePath != "")
	{
		CFile file;
		file.Open(this->filePath, CFile::modeWrite | CFile::modeCreate);
		char buf[1024];
		CArchive ar(&file, CArchive::store, 1024, buf);
		ar << this->sampleCount;
		for (int i = 0; i < this->sampleCount; i++)
		{
			ar << sampleArray[i].averageSize;
			ar << sampleArray[i].breakPointCount;
			for (int j = 0; j < 4; j++)
				ar << sampleArray[i].breakPointIndex[j];
			ar << sampleArray[i].clay;
			ar << sampleArray[i].dataCount;
			ar << sampleArray[i].deep;
			ar << sampleArray[i].gravel;
			ar << sampleArray[i].kurtosisCoefficient;
			ar << sampleArray[i].latitude;
			ar << sampleArray[i].longitude;
			ar << sampleArray[i].mediumSize;
			for (int j = 0; j < 128 + 2 * SPAN; j++)
				ar << sampleArray[i].normalY[j];
			ar << sampleArray[i].positionNum;
			ar << sampleArray[i].sampleID;
			ar << sampleArray[i].sand;
			ar << sampleArray[i].silt;
			for (int j = 0; j < 128; j++)
				ar << sampleArray[i].sizeDistribution[j];
			ar << sampleArray[i].skewnessCoefficient;
			ar << sampleArray[i].sortingCoefficient;
			for (int j = 0; j < 128; j++)
				ar << sampleArray[i].suspectBreakpoint[j];
			ar << sampleArray[i].suspectBreakpointCount;
			for (int j = 0; j < 128; j++)
				ar << sampleArray[i].xDataArray[j];
			for (int j = 0; j < 128; j++)
				ar << sampleArray[i].yDataArray[j];
		}
		ar.Close();
		file.Close();
		this->fileIsModified = false;
		MessageBox(_T("File has been saved. "), _T("Done"), MB_ICONASTERISK);
	}
	else
	{
		this->OnMenuSaveto();
	}
	
}

void CGrainsizeAnalysisDlg::OnMenuSaveto()
{
	// TODO: 另存为
	CFileDialog fileDlg(false, _T("gsa"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("GSA File(*.gsa)|*.gsa|All Files(*.*)|*.*||"), this);
	fileDlg.m_ofn.lpstrTitle = _T("Save To");
	if (fileDlg.DoModal() == IDOK)
	{
		this->filePath = fileDlg.GetFolderPath() + "\\" + fileDlg.GetFileName();
		this->OnMenuSave();
	}
}

void CGrainsizeAnalysisDlg::OnToolExportascsv()
{
	// TODO: 输出到CSV
	if (this->sampleCount <= 1)
	{
		MessageBox(_T("Only one sample data can not exported to a CSV file. "), _T("Notice"), MB_ICONASTERISK);
		return;
	}
	CFileDialog fileDlg(false, _T("csv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("CSV File(*.csv)|*.csv|All Files(*.*)|*.*||"), this);
	fileDlg.m_ofn.lpstrTitle = _T("Export As CSV");
	if (fileDlg.DoModal() == IDOK)
	{
		CString csvPath = fileDlg.GetFolderPath() + "\\" + fileDlg.GetFileName();
		CStdioFile outFile(csvPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		outFile.SeekToBegin();
		CString t_Str;
		EachSample sample;
		// 输出表头
		t_Str = _T("Sample,Position,Longitude,Latitude,Deep,");
		char t_HeadChar[1024];
		for (int i = globleXCount - 1; i >= 0; i--)
		{
			sprintf_s(t_HeadChar, 1024, "%.2f,", globleXValue[i]);
			t_Str += t_HeadChar;
		}
		t_Str += _T("Gravel,Sand,Silt,Clay,Average Size,Medium Size,Skewness,Kurtosis,Sorting\n");
		outFile.WriteString(t_Str);
		// 输出数据
		for (int i = 0; i < this->sampleArray.GetCount(); i++)
		{
			sample = sampleArray[i];
			t_Str = sample.sampleID + _T(",");
			t_Str += sample.positionNum + _T(",");
			t_Str += sample.longitude + _T(",");
			t_Str += sample.latitude + _T(",");
			t_Str += sample.deep + _T(",");
			char t_Char[32];
			for (int n = globleXCount - 1; n >= 0; n--)
			{
				for (int m = 0; m < sample.dataCount; m++)
				{
					if (abs(sample.xDataArray[m] - globleXValue[n]) < 0.00000001) // 浮点型数据不能用逻辑相等==
					{
						sprintf_s(t_Char, 32, "%.6f,", sample.yDataArray[m]);
						t_Str += t_Char;
						break;
					}
					else
					{
						if (m == sample.dataCount - 1)
						{
							sprintf_s(t_Char, 32, "%.6f,", (double)0);
							t_Str += t_Char;
						}
					}
				}
			}
			t_Str += sample.gravel + _T(",");
			t_Str += sample.sand + _T(",");
			t_Str += sample.silt + _T(",");
			t_Str += sample.clay + _T(",");
			t_Str += sample.averageSize + _T(",");
			t_Str += sample.mediumSize + _T(",");
			t_Str += sample.skewnessCoefficient + _T(",");
			t_Str += sample.kurtosisCoefficient + _T(",");
			t_Str += sample.sortingCoefficient + _T("\n");
			outFile.WriteString(t_Str);
		}
		outFile.Close();
	}
}

void CGrainsizeAnalysisDlg::OnEnChangeStaticPosition()
{ 
	// TODO:  如果该控件是 RICHEDIT 控件，它将不qwe
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString tmpStr;
	GetDlgItemText(IDC_STATIC_POSITION, tmpStr);
	if (tmpStr != sampleArray[currentSample].positionNum)
	{
		sampleArray[currentSample].positionNum = tmpStr;
		this->fileIsModified = true;
	}
}

void CGrainsizeAnalysisDlg::OnEnChangeStaticLongitude()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString tmpStr;
	GetDlgItemText(IDC_STATIC_LONGITUDE, tmpStr);
	if (tmpStr != sampleArray[currentSample].longitude)
	{
		sampleArray[currentSample].longitude = tmpStr;
		this->fileIsModified = true;
	}
}

void CGrainsizeAnalysisDlg::OnEnChangeStaticLatitude()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString tmpStr;
	GetDlgItemText(IDC_STATIC_LATITUDE, tmpStr);
	if (tmpStr != sampleArray[currentSample].latitude)
	{
		sampleArray[currentSample].latitude = tmpStr;
		this->fileIsModified = true;
	}
}

void CGrainsizeAnalysisDlg::OnEnChangeStaticDeep()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString tmpStr;
	GetDlgItemText(IDC_STATIC_DEEP, tmpStr);
	if (tmpStr != sampleArray[currentSample].deep)
	{
		sampleArray[currentSample].deep = tmpStr;
		this->fileIsModified = true;
	}
}

void CGrainsizeAnalysisDlg::GetFocusPoint(double k1, double b1, double k2, double b2, double *xP, double *yP)
{
/*
	y1 = k1x1 + b1
	y2 = k2x2 + b2
	
	-k1x1 + y1 = b1
	-k2x2 + y2 = b2

	      | b1   1 |
	      | b2   1 |
	xP = ------------
	      | -k1  1 |
	      | -k2  1 |
	
	      | -k1 b1 |
	      | -k2 b2 |
	yP = ------------
	      | -k1  1 |
	      | -k2  1 |

*/
	*xP = (b1 - b2) / (k2 - k1);
	*yP = (k2*b1 - k1*b2) / (k2 - k1);
}

void CGrainsizeAnalysisDlg::OnBnClickedCheckShowbreakpoint()
{
	// TODO:  在此添加控件通知处理程序代码
	if (this->m_CheckToShowControl.GetCheck() == 1) 
		this->isShowBreakPoint = true;
	else 
		this->isShowBreakPoint = false;

	this->Invalidate();
}
