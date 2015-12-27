// comtestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "comtest.h"
#include "comtestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	
END_MESSAGE_MAP()


// CcomtestDlg �Ի���




CcomtestDlg::CcomtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CcomtestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcomtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlComm);
}

BEGIN_MESSAGE_MAP(CcomtestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	//ON_EN_CHANGE(IDC_RCV, &CcomtestDlg::OnEnChangeRcv)
	ON_BN_CLICKED(IDC_BUTTON1, &CcomtestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CcomtestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CcomtestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CcomtestDlg::OnBnClickedButton4)
	ON_COMMAND_RANGE(60000,60115,myOnButtonClick) 
//ON_BN_CLICKED(60000, &CcomtestDlg::myOnButtonClick)
ON_BN_CLICKED(IDC_BUTTON5, &CcomtestDlg::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON6, &CcomtestDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CcomtestDlg ��Ϣ�������

BOOL CcomtestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CStatic* pStatic;
	pStatic=(CStatic*)GetDlgItem(IDC_STATIC);
	pStatic->ShowWindow(false);
	pStatic=(CStatic*)GetDlgItem(IDC_STATIC2);
	pStatic->ShowWindow(false);
	pStatic=(CStatic*)GetDlgItem(IDC_STATIC3);
	pStatic->ShowWindow(false);
	pStatic=(CStatic*)GetDlgItem(IDC_STATIC4);
	pStatic->ShowWindow(false);

	sensortype[0]="ѹǿ";
	sensortype[1]="����";
	sensortype[2]="ѹ��";
	sensortype[3]="����";
	sensortype[4]="���ٶ�";
	sensortype[5]="Һѹ";
	sensortype[6]="�ٶ�";
	sensortype[7]="��ѹ";
	sensortype[8]="���ٶ�";
	sensortype[9]="����";
	sensortype[10]="����";
	sensortype[12]="�߶�";
	sensortype[13]="�¶�";
	sensortype[14]="ѹ��";
	sensortype[15]="����";
	sensortype[16]="���ٶ�";
	sensortype[17]="Һѹ";
	sensortype[18]="�ٶ�";
	sensortype[19]="��ѹ";

	tbimnum=0;
	statextnum=0;
	gboxid=12300;
	staicid=12400;
	bid=60000;
	button_num=0;

	fin_s="";
	isend=false;
	isINat=false;

	i=0;


	CStatic* pc=(CStatic*)GetDlgItem(IDC_STATIC12);
	CFont font;
	font.CreatePointFont(120,"����");
	pc->SetFont(&font);

	OnBnClickedButton6();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CcomtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CcomtestDlg::OnPaint()
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
		CDialog::OnPaint();
	}

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CcomtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CcomtestDlg::OnEnChangeRcv()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CcomtestDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CStatic* pStatic=(CStatic*)GetDlgItem(IDC_STATIC);
	//pStatic->ShowWindow(true);

	//CStatic* mystatic1=new CStatic[16];
	//CStatic* mystatic2=new CStatic[16];
	//CButton* pbutton=new CButton[16];
	//
	//for(int i=0;i<16;i++)
	//{
	//	CString s;
	//	s.Format(_T("%d:"),i+1);
	//	mystatic1[i].Create(s,WS_CHILD | WS_VISIBLE,CRect(10,(10+20)*i+20,10+20,(10+20)*i+40),pStatic,5000+i);
	//	mystatic1[i].SetFont(this->GetFont());


	//	mystatic2[i].Create(_T("����"),WS_CHILD | WS_VISIBLE,CRect(40,(10+20)*i+20,10+40+30,(10+20)*i+40),pStatic,6000+i);
	//	mystatic2[i].SetFont(this->GetFont());		
	//	pbutton[i].Create(_T("off"),WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,CRect(80,(10+20)*i+20,10+40+40+30,(10+20)*i+40), pStatic, 60000+i); 
	//	pbutton[i].SetFont(this->GetFont());
	//	data a;
	//	a.tbim=1;
	//	a.tdcn=i;
	//	m[&pbutton[i]]=a;
	//	
	//}
	data a;
		a.tbim=1;
	a.tdcn=10;

		CString s;
		s.Format(_T("%d"),100);
		((CStatic*)m_input[a])->SetWindowText(s);
}

void CcomtestDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CStatic* pStatic=(CStatic*)GetDlgItem(IDC_STATIC2);
	pStatic->ShowWindow(true);

	CStatic* mystatic1=new CStatic[16];
	CStatic* mystatic2=new CStatic[16];
	CStatic* mystatic3=new CStatic[16];
	
	for(int i=0;i<16;i++)
	{
		CString s;
		s.Format(_T("%d:"),i+1);
		mystatic1[i].Create(s,WS_CHILD | WS_VISIBLE,CRect(10,(10+20)*i+20,10+20,(10+20)*i+40),pStatic,7000+i);
		mystatic1[i].SetFont(this->GetFont());

		mystatic2[i].Create(_T("100"),WS_CHILD | WS_VISIBLE,CRect(40,(10+20)*i+20,10+20+30,(10+20)*i+40),pStatic,8000+i);
		mystatic2[i].SetFont(this->GetFont());
		data a;
		a.tbim=2;
		a.tdcn=i+1;
		m_input[a]=&mystatic2[i];

		mystatic3[i].Create(_T("��"),WS_CHILD | WS_VISIBLE,CRect(80,(10+20)*i+20,10+20+40+30,(10+20)*i+40),pStatic,9000+i);
		mystatic3[i].SetFont(this->GetFont());
	}
}

void CcomtestDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CStatic* pStatic=(CStatic*)GetDlgItem(IDC_STATIC3);
	pStatic->ShowWindow(true);

	CStatic* mystatic1=new CStatic[16];
	CStatic* mystatic2=new CStatic[16];
	CStatic* mystatic3=new CStatic[16];
	
	for(int i=0;i<16;i++)
	{
		CString s;
		s.Format(_T("%d:"),i+1);
		mystatic1[i].Create(s,WS_CHILD | WS_VISIBLE,CRect(10,(10+20)*i+20,10+20,(10+20)*i+40),pStatic,10000+i);
		mystatic1[i].SetFont(this->GetFont());
		CString ss;
		ss.Format(_T("%d"),i+100);
		mystatic2[i].Create(ss,WS_CHILD | WS_VISIBLE,CRect(40,(10+20)*i+20,10+20+30,(10+20)*i+40),pStatic,11000+i);
		mystatic2[i].SetFont(this->GetFont());
			
		data a;
		a.tbim=3;
		a.tdcn=i+1;
		m_input[a]=&mystatic2[i];


		mystatic3[i].Create(_T("��"),WS_CHILD | WS_VISIBLE,CRect(80,(10+20)*i+20,10+20+40+30,(10+20)*i+40),pStatic,12000+i);
		mystatic3[i].SetFont(this->GetFont());
	}
}

void CcomtestDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CStatic* pStatic=(CStatic*)GetDlgItem(IDC_STATIC4);
	pStatic->ShowWindow(true);

	CStatic* mystatic1=new CStatic[16];
	CStatic* mystatic2=new CStatic[16];
	CStatic* mystatic3=new CStatic[16];
	
	for(int i=0;i<16;i++)
	{
		CString s;
		s.Format(_T("%d:"),i+1);
		mystatic1[i].Create(s,WS_CHILD | WS_VISIBLE,CRect(10,(10+20)*i+20,10+20,(10+20)*i+40),pStatic,200+i);
		mystatic1[i].SetFont(this->GetFont());

		mystatic2[i].Create(_T("100"),WS_CHILD | WS_VISIBLE,CRect(40,(10+20)*i+20,10+20+30,(10+20)*i+40),pStatic,300+i);
		mystatic2[i].SetFont(this->GetFont());

		mystatic3[i].Create(_T("��"),WS_CHILD | WS_VISIBLE,CRect(80,(10+20)*i+20,10+20+40+30,(10+20)*i+40),pStatic,300+i);
		mystatic3[i].SetFont(this->GetFont());
	}
}
void CcomtestDlg::myOnButtonClick(UINT uID)
{
	CButton* button=(CButton*)GetDlgItem(uID);
	CEdit* editext=(CEdit*)GetDlgItem(IDC_EDIT1);
	int a,b;
	a=m[button].tbim;
	b=m[button].tdcn;
	CString s;
	s.Format(_T("TBIM%d:"),a);
	CString s1;
	s1.Format(_T("tdcn%d:"),b);
	editext->SetWindowText(_T(s+s1));

	CString buttonname;
	button->GetWindowText(buttonname);
	if(buttonname=="on")
		button->SetWindowText("off");
	else
		button->SetWindowText("on");

 
	char upcmd[2];
	upcmd[0]=a;
	upcmd[1]=b;
	char d=(a<<4&0xf0)|(b&0x0f);
	//m_ctrlComm.put_Output(COleVariant(&d));



	CByteArray c;
	c.SetSize(1);
	c.SetAt(0,d);
m_ctrlComm.put_Output(COleVariant(c));
	


}
void CcomtestDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*for(;;)
	{
		data a;
		a.tbim=3;
		a.tdcn=(rand() % (16-1+1))+ 1;
		CString s;
		s.Format(_T("%d"),(rand() % (100-1+1))+ 1);
		((CStatic*)m_input[a])->SetWindowText(s);
	}*/
	char d[33];
	for(int i=0;i<33;i++)
		d[i]=1;

	d[1]=0;
	d[3]=0;
d[5]=0;
	d[7]=0;

	for(int i=0;i<4;i++)
	{
		d[0]=i+1;
		gen_control_panle(d);
	}
}
void CcomtestDlg::writedata(int a,int b,unsigned char sensordata[])
{
	union inter
	{
		int num;
		unsigned char data[4];
	};
	inter i;
	i.data[0]=sensordata[0];
	i.data[1]=sensordata[1];
	i.data[2]=sensordata[2];
	i.data[3]=sensordata[3];

	data index;
	index.tbim=a;
	index.tdcn=b;

	CString s;
	s.Format(_T("%d"),i.num);
	if(m_input[index]!=NULL)
	{
		((CStatic*)m_input[index])->SetWindowText(s);
	}
}
void CcomtestDlg::addcode()
{
// TODO: �ڴ˴������Ϣ����������


}
void CcomtestDlg::data_handle(char* data,int len)
{
	char* type=data;
	char t=*type;
	switch(t)
	{
	case '2':
		{
			//TEDS��Ϣ,����ÿһ��tdcn_teds��������һ���ռ�
	//CEdit* edit=(CEdit*)GetDlgItem(IDC_EDIT1);
	//CString s;
	//for(int i=1;i<33;i++)
	//{
	//	CString t;
	//	t.Format(_T("%d"),data[i]);
	//	s+=t;

	//}
	teds_data[1]=data[1];
	teds_data[2*i+2]=data[2];
	teds_data[2*i+3]=data[3];
	printf("%d, %d\n",data[2],data[3]);
	++i;
	if(i==16)
	{
			gen_control_panle(&teds_data[1]);
			i=0;
	}
			break;
		}
	case '3':
		{
			//������������Ϣ
			struct tdcn_value* v=(struct tdcn_value*)data;
			writedata(v->tbim,v->num,v->data);
			break;
		}
	default:
				{
			//TBC״̬��Ϣ��ֱ�Ӵ�ӡ����
			CString s;
			CString strtemp;
			 BYTE *rxdata=(BYTE*)data;
			 
			for(int k=0;k<len;k++)                       
			{   
				BYTE bt=(*(unsigned char*)(rxdata+k)); 
				strtemp.Format("%c",bt); 
				s+=strtemp;   
			 }
			//CEdit* edit=(CEdit*)GetDlgItem(IDC_EDIT2);
			//CString str;
			//edit->GetWindowText(str);
			//edit->SetWindowText(str+s);
			//edit->LineScroll(edit->GetLineCount(),0);
			string_cal(s);

			break;
		}
	}
}BEGIN_EVENTSINK_MAP(CcomtestDlg, CDialog)
ON_EVENT(CcomtestDlg, IDC_MSCOMM1, 1, CcomtestDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()

void CcomtestDlg::OnCommMscomm1()
{
	// TODO: �ڴ˴������Ϣ����������
	// TODO: �ڴ˴������Ϣ����������
	VARIANT variant_inp;     
	COleSafeArray safearray_inp;     
	LONG len,k;    
	BYTE rxdata[2048]; //����BYTE���� An 8-bit integerthat is not signed.     
	CString strtemp;     
	if(m_ctrlComm.get_CommEvent()==2) //�¼�ֵΪ2��ʾ���ջ����������ַ�     
	{            
		////////��������Ը����Լ���ͨ��Э����봦����� 
        variant_inp=m_ctrlComm.get_Input(); //��������  
        safearray_inp=variant_inp; //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
        len=safearray_inp.GetOneDimSize(); //�õ���Ч���ݳ���        
		for(k=0;k<len;k++)             
			safearray_inp.GetElement(&k,rxdata+k);//ת��ΪBYTE������   
		data_handle((char*)rxdata,len);
		
		
   }

}

void CcomtestDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctrlComm.put_CommPort(1);
//ѡ�񴮿ں�5(�ʼǱ�û�д��ڣ��õ����⴮�ڣ�����ʹ�����vspd����)
	m_ctrlComm.put_PortOpen(TRUE);
//�򿪴���
	m_ctrlComm.put_RThreshold(2);
//�յ������ֽ�����OnComm�¼�
	m_ctrlComm.put_InputMode(1);
//����ģʽѡΪ������
	m_ctrlComm.put_Settings(_T("115200,n,8,1"));
//���ô��ڲ����������ʣ�����żУ�飬λֹͣλ��λ����λ
	m_ctrlComm.put_InputMode(1);  // �Զ����Ʒ�ʽ��ȡ���� 
	m_ctrlComm.put_RThreshold(1); //����1��ʾÿ�����ڽ� �ջ��������ж��ڻ����1���ַ�ʱ������һ���������ݵ�OnComm�¼� 
	m_ctrlComm.put_InputLen(0); //���õ�ǰ���������ݳ���Ϊ0 
	m_ctrlComm.get_Input();//��Ԥ���������������������  
}

void CcomtestDlg::gen_control_panle(char* data)
{
	int tbim=data[0];
	++data;
	//����box
	CString s;
	s.Format(_T("%d"),tbim);
	s="TBIM"+s;
	int px=380+(180+20)*tbimnum;
	int py=100;
	tbiminfo[tbim-1].Create(_T(s),WS_CHILD|WS_VISIBLE|SS_CENTER,CRect(px+60,py-30,px+60+40,py-30+20),  this, gboxid++);
	tbiminfo[tbim-1].SetFont(this->GetFont());
	gbox[tbim-1].Create(_T(s),BS_TEXT|BS_GROUPBOX|WS_CHILD/*|SS_BLACKRECT*/|WS_VISIBLE, CRect(px,py,px+180,py+500),  this, gboxid++);
	gbox[tbim-1].SetFont(this->GetFont());
	for(int i=0;i<16;i++)
	{
		if(data[i*2]==0)//������
		{
			gen_sendor_preorder(px,py,20,20+(20+10)*i,40,20,i+1);
			gen_transducer_inorder(px,py,20+30+20,20+(20+10)*i,40,20,data[2*i+1]);
			gen_button(px,py,20+30+20+30+20,20+(20+10)*i,40,20,tbim,i+1);

		}
		else//sensor
		{
			gen_sendor_preorder(px,py,20,20+(20+10)*i,40,20,i+1);
			gen_sendor_inorder(px,py,20+30+20,20+(20+10)*i,40,20,tbim,i+1);
			gen_sendor_post(px,py,20+30+20+30+20,20+(20+10)*i,40,20,data[2*i+1]);
		}
	}
	++tbimnum;
}
void CcomtestDlg::gen_sendor_preorder(int px,int py,int x,int y,int w,int h,int tdcn)
{
	CString s;
	s.Format(_T("%d"),tdcn);
	statext[statextnum].Create(_T(s+":"),WS_CHILD | WS_VISIBLE,CRect(px+x,py+y,px+x+w,py+y+h),this,staicid++);
	statext[statextnum].SetFont(this->GetFont());
	statextnum++;
}
void CcomtestDlg::gen_sendor_inorder(int px,int py,int x,int y,int w,int h,int tbim,int tdcn)
{
	statext[statextnum].Create(_T("0"),WS_CHILD | WS_VISIBLE,CRect(px+x,py+y,px+x+w,py+y+h),this,staicid++);
	statext[statextnum].SetFont(this->GetFont());
	data a;
	a.tbim=tbim;
	a.tdcn=tdcn;
	m_input[a]=&statext[statextnum];
	statextnum++;
}
void CcomtestDlg::gen_sendor_post(int px,int py,int x,int y,int w,int h,int type)
{
	statext[statextnum].Create(_T(sensortype[type]),WS_CHILD | WS_VISIBLE,CRect(px+x,py+y,px+x+w,py+y+h),this,staicid++);
	statext[statextnum].SetFont(this->GetFont());
	statextnum++;
}
void CcomtestDlg::gen_button(int px,int py,int x,int y,int w,int h,int tbim,int tdcn)
{
	butarr[button_num].	Create(_T("off"),WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,CRect(px+x,py+y,px+x+w,py+y+h), this, BUTID+button_num); 
	butarr[button_num].	SetFont(this->GetFont());
	data a;
	a.tbim=tbim;
	a.tdcn=tdcn;
	m[&butarr[button_num]]=a;
	button_num++;
}
void CcomtestDlg::gen_transducer_inorder(int px,int py,int x,int y,int w,int h,int type)
{
	if(type==1)
		statext[statextnum].Create(_T("ָʾ��"),WS_CHILD | WS_VISIBLE,CRect(px+x,py+y,px+x+w,py+y+h),this,staicid++);
	else
		statext[statextnum].Create(_T("������"),WS_CHILD | WS_VISIBLE,CRect(px+x,py+y,px+x+w,py+y+h),this,staicid++);

	statext[statextnum].SetFont(this->GetFont());
	statextnum++;
}

void CcomtestDlg::string_cal(CString str)
{
	int idx=str.Find(_T("@"),0);

	if(idx==-1)//����ַ�����û��@
	{
		if(isINat)
		{
			fin_s+=str;//����@@�м�
		}
		else
		{
			edit_print(str);//ֱ�Ӵ�ӡ
		}
	}
	else//����ַ�������@
	{
		if(isend)
		{
			fin_s+=str.Left(idx);
			
			for(int i=0;i<33;i++)//�����������ݲ����н�������
				fin_data[i]=fin_s[i]-'0';
			gen_control_panle(fin_data);

			fin_s="";
			edit_print(str.Right(str.GetLength()-idx-1));//�����ֱ�Ӵ�ӡ
			isINat=false;
			isend=false;
		}
		else
		{
			edit_print(str.Left(idx));//ֱ�Ӵ�ӡstr��ߵ�����
			isend=true;
			isINat=true;
			fin_s+=str.Right(str.GetLength()-idx-1);
		}
	}
}
void CcomtestDlg::edit_print(CString str)
{
	CEdit* edit=(CEdit*)GetDlgItem(IDC_EDIT2);
	CString t;
	edit->GetWindowText(t);
	edit->SetWindowText(t+str);
	edit->LineScroll(edit->GetLineCount(),0);
}