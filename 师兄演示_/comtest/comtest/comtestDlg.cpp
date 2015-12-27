// comtestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "comtest.h"
#include "comtestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CcomtestDlg 对话框




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


// CcomtestDlg 消息处理程序

BOOL CcomtestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CStatic* pStatic;
	pStatic=(CStatic*)GetDlgItem(IDC_STATIC);
	pStatic->ShowWindow(false);
	pStatic=(CStatic*)GetDlgItem(IDC_STATIC2);
	pStatic->ShowWindow(false);
	pStatic=(CStatic*)GetDlgItem(IDC_STATIC3);
	pStatic->ShowWindow(false);
	pStatic=(CStatic*)GetDlgItem(IDC_STATIC4);
	pStatic->ShowWindow(false);

	sensortype[0]="压强";
	sensortype[1]="开关";
	sensortype[2]="压力";
	sensortype[3]="流量";
	sensortype[4]="角速度";
	sensortype[5]="液压";
	sensortype[6]="速度";
	sensortype[7]="气压";
	sensortype[8]="加速度";
	sensortype[9]="功耗";
	sensortype[10]="油量";
	sensortype[12]="高度";
	sensortype[13]="温度";
	sensortype[14]="压力";
	sensortype[15]="流量";
	sensortype[16]="角速度";
	sensortype[17]="液压";
	sensortype[18]="速度";
	sensortype[19]="气压";

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
	font.CreatePointFont(120,"宋体");
	pc->SetFont(&font);

	OnBnClickedButton6();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcomtestDlg::OnPaint()
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
		CDialog::OnPaint();
	}

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcomtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CcomtestDlg::OnEnChangeRcv()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CcomtestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
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


	//	mystatic2[i].Create(_T("开关"),WS_CHILD | WS_VISIBLE,CRect(40,(10+20)*i+20,10+40+30,(10+20)*i+40),pStatic,6000+i);
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
	// TODO: 在此添加控件通知处理程序代码
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

		mystatic3[i].Create(_T("度"),WS_CHILD | WS_VISIBLE,CRect(80,(10+20)*i+20,10+20+40+30,(10+20)*i+40),pStatic,9000+i);
		mystatic3[i].SetFont(this->GetFont());
	}
}

void CcomtestDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
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


		mystatic3[i].Create(_T("度"),WS_CHILD | WS_VISIBLE,CRect(80,(10+20)*i+20,10+20+40+30,(10+20)*i+40),pStatic,12000+i);
		mystatic3[i].SetFont(this->GetFont());
	}
}

void CcomtestDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
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

		mystatic3[i].Create(_T("度"),WS_CHILD | WS_VISIBLE,CRect(80,(10+20)*i+20,10+20+40+30,(10+20)*i+40),pStatic,300+i);
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
	// TODO: 在此添加控件通知处理程序代码
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
// TODO: 在此处添加消息处理程序代码


}
void CcomtestDlg::data_handle(char* data,int len)
{
	char* type=data;
	char t=*type;
	switch(t)
	{
	case '2':
		{
			//TEDS信息,根据每一个tdcn_teds数据生成一个空间
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
			//传感器数据信息
			struct tdcn_value* v=(struct tdcn_value*)data;
			writedata(v->tbim,v->num,v->data);
			break;
		}
	default:
				{
			//TBC状态信息，直接打印处理
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
	// TODO: 在此处添加消息处理程序代码
	// TODO: 在此处添加消息处理程序代码
	VARIANT variant_inp;     
	COleSafeArray safearray_inp;     
	LONG len,k;    
	BYTE rxdata[2048]; //设置BYTE数组 An 8-bit integerthat is not signed.     
	CString strtemp;     
	if(m_ctrlComm.get_CommEvent()==2) //事件值为2表示接收缓冲区内有字符     
	{            
		////////以下你可以根据自己的通信协议加入处理代码 
        variant_inp=m_ctrlComm.get_Input(); //读缓冲区  
        safearray_inp=variant_inp; //VARIANT型变量转换为ColeSafeArray型变量
        len=safearray_inp.GetOneDimSize(); //得到有效数据长度        
		for(k=0;k<len;k++)             
			safearray_inp.GetElement(&k,rxdata+k);//转换为BYTE型数组   
		data_handle((char*)rxdata,len);
		
		
   }

}

void CcomtestDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	
	// TODO: 在此添加控件通知处理程序代码
	m_ctrlComm.put_CommPort(1);
//选择串口号5(笔记本没有串口，用的虚拟串口，可以使用软件vspd创建)
	m_ctrlComm.put_PortOpen(TRUE);
//打开串口
	m_ctrlComm.put_RThreshold(2);
//收到两个字节引发OnComm事件
	m_ctrlComm.put_InputMode(1);
//输入模式选为二进制
	m_ctrlComm.put_Settings(_T("115200,n,8,1"));
//设置串口参数，波特率，无奇偶校验，位停止位，位数据位
	m_ctrlComm.put_InputMode(1);  // 以二进制方式检取数据 
	m_ctrlComm.put_RThreshold(1); //参数1表示每当串口接 收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件 
	m_ctrlComm.put_InputLen(0); //设置当前接收区数据长度为0 
	m_ctrlComm.get_Input();//先预读缓冲区以清除残留数据  
}

void CcomtestDlg::gen_control_panle(char* data)
{
	int tbim=data[0];
	++data;
	//生成box
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
		if(data[i*2]==0)//作动器
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
		statext[statextnum].Create(_T("指示灯"),WS_CHILD | WS_VISIBLE,CRect(px+x,py+y,px+x+w,py+y+h),this,staicid++);
	else
		statext[statextnum].Create(_T("警报器"),WS_CHILD | WS_VISIBLE,CRect(px+x,py+y,px+x+w,py+y+h),this,staicid++);

	statext[statextnum].SetFont(this->GetFont());
	statextnum++;
}

void CcomtestDlg::string_cal(CString str)
{
	int idx=str.Find(_T("@"),0);

	if(idx==-1)//如果字符串中没有@
	{
		if(isINat)
		{
			fin_s+=str;//处于@@中间
		}
		else
		{
			edit_print(str);//直接打印
		}
	}
	else//如果字符串中有@
	{
		if(isend)
		{
			fin_s+=str.Left(idx);
			
			for(int i=0;i<33;i++)//处理生成数据并进行界面生成
				fin_data[i]=fin_s[i]-'0';
			gen_control_panle(fin_data);

			fin_s="";
			edit_print(str.Right(str.GetLength()-idx-1));//右面的直接打印
			isINat=false;
			isend=false;
		}
		else
		{
			edit_print(str.Left(idx));//直接打印str左边的数据
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