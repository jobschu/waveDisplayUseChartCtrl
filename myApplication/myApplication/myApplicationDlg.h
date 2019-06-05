
// myApplicationDlg.h : 头文件
//

#pragma once
#include "E:\study\vc++\myproject\myApplication\myApplication\ChartCtrl\ChartCtrl.h"


// CmyApplicationDlg 对话框
class CmyApplicationDlg : public CDialogEx
{
// 构造
public:
	CmyApplicationDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYAPPLICATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	POINT oldPiont;
	void Resize(void);
	CChartCtrl m_ChartCtrl;
	void ChartCtrlInit(void);
	void DataBuffInit(void);
	void DataShow(double *xb, double *yb, int len);
};
