
// myApplicationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "myApplication.h"
#include "myApplicationDlg.h"
#include "afxdialogex.h"
#include "ChartCtrl\ChartCtrl.h"
#include "ChartCtrl\ChartAxisLabel.h"
#include "ChartCtrl\ChartLineSerie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmyApplicationDlg 对话框



CmyApplicationDlg::CmyApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYAPPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmyApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_SHOW, m_ChartCtrl);
}
BEGIN_MESSAGE_MAP(CmyApplicationDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

//初始化显示的区域大小
#define DATA_SHOW_LENGHT	2000	//总共显示的点个数
#define DATA_UPDATE_LENGHT	10    //每次更新的点个数
#define DATA_SHOW_X_AXIS	2000   //X轴显示的点最大值
#define DATA_SHOW_Y_AXIS	1000   //Y轴显示的点最大值
//要显示点的缓冲数据
static double xBuff[DATA_SHOW_LENGHT] = { 0 };
static double yBuff[DATA_SHOW_LENGHT] = { 0 };
//显示点数据包初始化
void CmyApplicationDlg::DataBuffInit(void)
{
	for (int i = 0; i < DATA_SHOW_LENGHT; i++) {
		xBuff[i] = i;
		yBuff[i] = 50;// cos((i)) * 10 + 50;
	}
}
//初始化画图界面窗口
void CmyApplicationDlg::ChartCtrlInit(void) {
	//手动创建显示窗口
	//CRect rect, rectChart;
	//GetDlgItem(IDC_CUSTOM_SHOW)->GetWindowRect(&rect);
	//ScreenToClient(rect);
	//rectChart = rect;
	//rectChart.top = rect.bottom + 3;
	//rectChart.bottom = rectChart.top + rect.Height();
	//m_ChartCtrl2.Create(this, rectChart, IDC_CUSTOM_SHOW2);
	//m_ChartCtrl2.ShowWindow(SW_SHOWNORMAL);
	///////////////////////显示主题/////////////////////////////
	m_ChartCtrl.GetTitle()->AddString(_T("采样信号波形"));
	///////////////////////创建坐标xy标识/////////////////////////////
	//m_ChartCtrl.GetBottomAxis()->GetLabel()->SetText(_T("强度"));
	//m_ChartCtrl.GetLeftAxis()->GetLabel()->SetText(_T("采样点"));
	///////////////////////创建坐标显示范围/////////////////////////////
	CChartAxis *pAxis = NULL;
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetMinMax(0, DATA_SHOW_X_AXIS);
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetMinMax(0, DATA_SHOW_Y_AXIS);
}
// CmyApplicationDlg 消息处理程序
BOOL CmyApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//获取显示的对话框大小
	CRect rect;
	GetClientRect(&rect);
	oldPiont.x = rect.right - rect.left;
	oldPiont.y = rect.bottom - rect.top;
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	DataBuffInit();
	ChartCtrlInit();
	
	SetTimer(0, 100, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmyApplicationDlg::OnPaint()
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
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmyApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmyApplicationDlg::Resize(void) {
	float fsp[2];
	POINT newPoint;//获取当前对话框大小
	CRect newRect;//获取当前对话框的坐标
	GetClientRect(&newRect);
	newPoint.x = newRect.right - newRect.left;
	newPoint.y = newRect.bottom - newRect.top;
	fsp[0] = (float)newPoint.x / oldPiont.x;
	fsp[1] = (float)newPoint.y / oldPiont.y;

	int woc;
	CRect rect;
	CPoint oldTLPoint, newTLPoint;//左上角
	CPoint oldBRPoint, newBRPoint;//右下角
								  //列出所有的子空间
	HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD);
	while (hwndChild) {
		woc = ::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->GetWindowRect(rect);
		ScreenToClient(rect);

		oldTLPoint = rect.TopLeft();
		newTLPoint.x = long(oldTLPoint.x*fsp[0]);
		newTLPoint.y = long(oldTLPoint.y*fsp[1]);
		oldBRPoint = rect.BottomRight();
		newBRPoint.x = long(oldBRPoint.x*fsp[0]);
		newBRPoint.y = long(oldBRPoint.y*fsp[1]);

		rect.SetRect(newTLPoint, newBRPoint);
		GetDlgItem(woc)->MoveWindow(rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	oldPiont = newPoint;
	return;
}
void CmyApplicationDlg::OnSize(UINT nType, int cx, int cy) {
	//窗体大小发生变动。处理函数resize
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		Resize();
	}
}
void CmyApplicationDlg::DataShow(double *xb, double *yb, int len) {
	m_ChartCtrl.EnableRefresh(false);
	CChartLineSerie *pLineSerie;
	m_ChartCtrl.RemoveAllSeries();
	pLineSerie = m_ChartCtrl.CreateLineSerie();
	pLineSerie->SetSeriesOrdering(poNoOrdering);//设置为无序
	pLineSerie->AddPoints(xb, yb, len);
	UpdateWindow();
	m_ChartCtrl.EnableRefresh(true);
}
void CmyApplicationDlg::OnTimer(UINT nIDEvent) {
	static int offset = 0;
	for (int m = 0; m < DATA_SHOW_LENGHT - DATA_UPDATE_LENGHT; m++)
	{
		//xd[m] = xd[DATA_UPDATE_LENGHT + m];
		yBuff[m] = yBuff[DATA_UPDATE_LENGHT + m];
	}
	int index = DATA_SHOW_LENGHT - DATA_UPDATE_LENGHT;
	for (int i = 0; i<DATA_UPDATE_LENGHT; i++)
	{
		//yd[index + i] = cos((index + i + w)/5) * 50 + 100+rand() / 1000;
		yBuff[index + i] = cos((i+offset)/5) * DATA_SHOW_Y_AXIS/4 + rand()/1000+ DATA_SHOW_Y_AXIS / 2;
	}
	DataShow(xBuff, yBuff, DATA_SHOW_LENGHT);
	offset++;
	if (offset > 10000) {
		offset = 0;
	}
}