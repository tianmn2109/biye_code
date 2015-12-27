#include "stdafx.h"
#include "commondefine.h"
CString codeToinfo(int a)
{
	switch(a)
	{
	case 1:return "TBC init done!\r\n";
	case 2:return "TBC reconfig TBIM\r\n";
	case 3:return "TBC reconfig TBIM done!\r\n";
	case 4:return "TBC working\r\n";

	default:break;
	}
	return "error";
}