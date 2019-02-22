
// GrainsizeAnalysisDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#define MARGIN 60
#define PI 3.14159265
#define E 2.71828182

// 保留两位小数
#define FIX001(x) ((double)((int)((x)*100+0.5))/100.0)

// 定义显示Y轴长度（像素）
#define YLEN 410

// 拐弯地方识别的精度，推荐2~4，整数
#define SPAN 3

// 数据长度
#define XMAX 8000

// 填充颜色
#define BS_GRAVEL	RGB(0, 64, 128)
#define BS_SAND		RGB(64, 0, 128)
#define BS_SILT		RGB(255, 128, 128)
#define BS_CLAY		RGB(106, 57, 6)

// 粒径分布宽高
#define GS_WIDTH	220
#define GS_HEIGHT	155
#define GS_MARGIN	30

// CGrainsizeAnalysisDlg 对话框
class CGrainsizeAnalysisDlg : public CDialogEx
{
	// 构造
public:
	CGrainsizeAnalysisDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_GRAINSIZEANALYSIS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	// 样品结构体
	struct EachSample
	{
		CString sampleID; // 样品编号
		CString positionNum; // 站位号
		CString longitude; // 经度
		CString latitude; // 纬度
		CString deep; // 深度
		CString gravel; // 砾石
		CString sand; // 砂
		CString silt; // 粉砂
		CString clay; // 泥
		CString averageSize; // 平均粒径
		CString mediumSize; // 中值粒径
		CString skewnessCoefficient; // 偏态系数
		CString kurtosisCoefficient; // 峰态系数
		CString sortingCoefficient; // 分选系数
		int dataCount; // 样品有效数据个数
		int suspectBreakpoint[128]; // 疑似拐弯点下标
		int suspectBreakpointCount; // 疑似拐弯点下标个数
		int breakPointCount; // 转折点个数
		int breakPointIndex[4]; // 记录拐弯点下标
		double sizeDistribution[128]; // 各粒径大小分布（频率）
		double xDataArray[128]; // 粒度，已取负对数，-3~16
		double yDataArray[128]; // 累计频率，0~100，进行查值前，转换为显示坐标需要除以100转换到 0~1！
		int normalY[128 + 2 * SPAN]; // 正太Y: 0~2048;
	};

	double xStep; // PHI值，0.25 0.5 1
	double dataStep; // 用于横轴分度值
	double gs_dataStep; // 粒径分布图分度值
	double distributionMap[2048]; // 正太查询表 0~1
	int base; // 基准点 0.01时，正太查询表下标
	int sampleCount; // 记录样品数据条数
	CArray<EachSample, EachSample> sampleArray; // 样品队列
	int currentSample; // 当前样品序列号
	double globleXValue[128]; // 整个X值分布（表头X值）
	int globleXCount; // 数据源中X值分布范围
	char breakPointTitle[4][5]; // 拐弯点序号
	bool isShowBreakPoint; // 是否显示黑色断点用于调整
	CString filePath;  // 当前文件打开路径
	bool fileIsModified; // 当前打开文件已被改动过

	void ShowTheMap(CDC *, CRect, EachSample); // 绘制坐标图
	void SetDataStep(EachSample); // 设置横轴步进
	void GetNormalDistributionMap(); // 建立正太分布查询表
	int GetShowPointX(double, EachSample); // 横轴源数据点转成显示点
	int GetShowPointY(double); // 纵轴源数据点转成显示点，通过查表的方式进行
	int GetGSShowPointX(double, EachSample); // 横轴源数据点转成粒径分布图显示点
	int GetGSShowPointY(double); // 纵轴源数据点转成显示点
	void ShowPointToValue(int, int, double*, double*, EachSample); // 显示的点返回去查值
	int GetNormalY(double); // 纵轴源数据点转成正太点，通过查表的方式进行
	void GetBreakPointIndex(EachSample *); // 获取拐弯点下标，并将点添加到下拉框，供手动选择
	void SortBreakPoint(EachSample *); // 断点下标从小到大排序
	void AddData2Combo(); // 添加拐弯点候选数据到下拉框
	void ShowThePoint(CDC *, EachSample); // 显示数据点
	int GetDataFromCSVLine(CString); // 从CSV文件的每一行添加表头，样品数据，完成数据载入初始化
	void ShowSampleInfo(); // 显示样品基础数据
	void SaveSampleInfo(); // 保存样品基础数据
	void ShowRawData(); // 显示样品程序从数据源所得到的原始数据，用于查错
	void InitNoneSample(); // 载入一个空的样品数据用于显示
	void OldDrawTheAnalysisLine1(CDC *, EachSample); // 显示1个断点时解释的直线
	void OldDrawTheAnalysisLine2(CDC *, EachSample); // 显示2个断点时解释的直线
	void OldDrawTheAnalysisLine3(CDC *, EachSample); // 显示3个断点时解释的直线
	void OldDrawTheAnalysisLine4(CDC *, EachSample); // 显示4个断点时解释的直线
	void DrawTheAnalysisLine1(CDC *, EachSample); // 显示1个断点时解释的直线
	void DrawTheAnalysisLine2(CDC *, EachSample); // 显示2个断点时解释的直线
	void DrawTheAnalysisLine3(CDC *, EachSample); // 显示3个断点时解释的直线
	void DrawTheAnalysisLine4(CDC *, EachSample); // 显示4个断点时解释的直线
	void DrawSizeDitribution(CDC *, CRect, EachSample); // 绘制粒度大小分布
	void LineFitting(double *, double *, int, int, double *, double *); // 线性回归
	void GetFocusPoint(double, double, double, double, double *, double *); // 计算焦点

public:
	afx_msg void OnMenuImportdata();
	// 画板
	CStatic m_DrawPanelControl;
	afx_msg void OnBnClickedBtnBefor();
	afx_msg void OnBnClickedBtnNext();
	// 原始数据显示表
	CListBox m_RawDataListControl;
	afx_msg void OnToolCleandata();
	// 解释点数
	CComboBox m_NumberSelectControl;
	afx_msg void OnCbnSelchangeComboNumber();
	// 第一个转折点下拉框控件
	CComboBox m_firBreakPointComboControl;
	// 第二个转折点下拉框控件
	CComboBox m_secBreakPointComboControl;
	// 第三个转折点下拉框控件
	CComboBox m_thrBreakPointComboControl;
	// 第四个转折点下拉框控件
	CComboBox m_forBreakPointComboControl;
	afx_msg void OnCbnSelchangeCombo1st();
	afx_msg void OnCbnSelchangeCombo2nd();
	afx_msg void OnCbnSelchangeCombo3rd();
	afx_msg void OnCbnSelchangeCombo4th();
	afx_msg void OnAboutAboutgrainsizeanalysis();
	afx_msg void OnAboutHowtouse();
	afx_msg void OnMenuQuit();
	// 样品下拉框
	CComboBox m_SampleListControl;
	afx_msg void OnCbnSelchangeComboSample();
	afx_msg void OnMenuOpen();
	afx_msg void OnMenuSave();
	afx_msg void OnMenuSaveto();
	afx_msg void OnToolExportascsv();
	afx_msg void OnToolPrevious();
	afx_msg void OnToolNext();
	afx_msg void OnEnChangeStaticPosition();
	afx_msg void OnEnChangeStaticLongitude();
	afx_msg void OnEnChangeStaticLatitude();
	afx_msg void OnEnChangeStaticDeep();
	afx_msg void OnMenuNew();
	CButton m_CheckToShowControl;
	afx_msg void OnBnClickedCheckShowbreakpoint();
};
