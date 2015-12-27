// comtestDlg.h : ͷ�ļ�
//

#pragma once

#include <map>
#include "commondefine.h"
#include "mscomm1.h"
using namespace std;
#define BUTID 60000
struct data
{
public:
	int id;
	int tbim;
	int tdcn;
	bool operator<(const data& other)  const 
    {
		return tbim*100+tdcn < other.tbim*100+other.tdcn;
    }
};
// CcomtestDlg �Ի���
class CcomtestDlg : public CDialog
{
// ����
public:
	CcomtestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_COMTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void myOnButtonClick(UINT uID);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeRcv();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	map<CObject*,data> m;
	map<data,CObject*>m_input;

	afx_msg void OnBnClickedButton5();
private:
	CString sensortype[20];
	int tbimnum;
	int statextnum;
	CStatic gbox[14];
	CStatic tbiminfo[14];
	CStatic statext[300];
	CButton butarr[100];
	unsigned int gboxid;//box id
	unsigned int staicid;//static id
	unsigned int bid;
	int button_num;
	char teds_data[33];
	int i;

	void writedata(int a,int b,unsigned char data[]);
	void addcode();
	void data_handle(char* data,int len);//������λ�����յ������������ﴦ��
	void gen_control_panle(char* data);
	void gen_sendor_post(int px,int py,int x,int y,int h,int w,int type);
	void gen_sendor_inorder(int px,int py,int x,int y,int h,int w,int tbim,int tdcn);
	void gen_sendor_preorder(int px,int py,int x,int y,int h,int w,int tdcn);
	void gen_transducer_inorder(int px,int py,int x,int y,int h,int w,int type);

	void gen_button(int px,int py,int x,int y,int h,int w,int tbim,int tdcn);

	//------------�ַ�������ʹ��------------------------
	char fin_data[33];
	CString fin_s;
	bool isend;
	bool isINat;
	void string_cal(CString str);
	void edit_print(CString str);
	//-----------------------------------------------------------------
public:
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	CMscomm1 m_ctrlComm;
	afx_msg void OnBnClickedButton6();
};
