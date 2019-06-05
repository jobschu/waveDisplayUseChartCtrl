
// myApplicationDlg.cpp : ʵ���ļ�
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


// CmyApplicationDlg �Ի���



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

//��ʼ����ʾ�������С
#define DATA_SHOW_LENGHT	2000	//�ܹ���ʾ�ĵ����
#define DATA_UPDATE_LENGHT	10    //ÿ�θ��µĵ����
#define DATA_SHOW_X_AXIS	2000   //X����ʾ�ĵ����ֵ
#define DATA_SHOW_Y_AXIS	1000   //Y����ʾ�ĵ����ֵ
//Ҫ��ʾ��Ļ�������
static double xBuff[DATA_SHOW_LENGHT] = { 0 };
static double yBuff[DATA_SHOW_LENGHT] = { 0 };
//��ʾ�����ݰ���ʼ��
void CmyApplicationDlg::DataBuffInit(void)
{
	for (int i = 0; i < DATA_SHOW_LENGHT; i++) {
		xBuff[i] = i;
		yBuff[i] = 50;// cos((i)) * 10 + 50;
	}
}
//��ʼ����ͼ���洰��
void CmyApplicationDlg::ChartCtrlInit(void) {
	//�ֶ�������ʾ����
	//CRect rect, rectChart;
	//GetDlgItem(IDC_CUSTOM_SHOW)->GetWindowRect(&rect);
	//ScreenToClient(rect);
	//rectChart = rect;
	//rectChart.top = rect.bottom + 3;
	//rectChart.bottom = rectChart.top + rect.Height();
	//m_ChartCtrl2.Create(this, rectChart, IDC_CUSTOM_SHOW2);
	//m_ChartCtrl2.ShowWindow(SW_SHOWNORMAL);
	///////////////////////��ʾ����/////////////////////////////
	m_ChartCtrl.GetTitle()->AddString(_T("�����źŲ���"));
	///////////////////////��������xy��ʶ/////////////////////////////
	//m_ChartCtrl.GetBottomAxis()->GetLabel()->SetText(_T("ǿ��"));
	//m_ChartCtrl.GetLeftAxis()->GetLabel()->SetText(_T("������"));
	///////////////////////����������ʾ��Χ/////////////////////////////
	CChartAxis *pAxis = NULL;
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetMinMax(0, DATA_SHOW_X_AXIS);
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetMinMax(0, DATA_SHOW_Y_AXIS);
}
// CmyApplicationDlg ��Ϣ�������
BOOL CmyApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//��ȡ��ʾ�ĶԻ����С
	CRect rect;
	GetClientRect(&rect);
	oldPiont.x = rect.right - rect.left;
	oldPiont.y = rect.bottom - rect.top;
	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	DataBuffInit();
	ChartCtrlInit();
	
	SetTimer(0, 100, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CmyApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CmyApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmyApplicationDlg::Resize(void) {
	float fsp[2];
	POINT newPoint;//��ȡ��ǰ�Ի����С
	CRect newRect;//��ȡ��ǰ�Ի��������
	GetClientRect(&newRect);
	newPoint.x = newRect.right - newRect.left;
	newPoint.y = newRect.bottom - newRect.top;
	fsp[0] = (float)newPoint.x / oldPiont.x;
	fsp[1] = (float)newPoint.y / oldPiont.y;

	int woc;
	CRect rect;
	CPoint oldTLPoint, newTLPoint;//���Ͻ�
	CPoint oldBRPoint, newBRPoint;//���½�
								  //�г����е��ӿռ�
	HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD);
	while (hwndChild) {
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID
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
	//�����С�����䶯��������resize
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
	pLineSerie->SetSeriesOrdering(poNoOrdering);//����Ϊ����
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