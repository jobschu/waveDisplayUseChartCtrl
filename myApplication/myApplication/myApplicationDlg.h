
// myApplicationDlg.h : ͷ�ļ�
//

#pragma once
#include "E:\study\vc++\myproject\myApplication\myApplication\ChartCtrl\ChartCtrl.h"


// CmyApplicationDlg �Ի���
class CmyApplicationDlg : public CDialogEx
{
// ����
public:
	CmyApplicationDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYAPPLICATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
