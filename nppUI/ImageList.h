// This file is part of Notepad++ project
// Copyright (C)2003 Don HO <don.h@free.fr>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// Note that the GPL places important restrictions on "derived works", yet
// it does not provide a detailed definition of that term.  To avoid      
// misunderstandings, we consider an application to constitute a          
// "derivative work" for the purpose of this license if it does any of the
// following:                                                             
// 1. Integrates source code from Notepad++.
// 2. Integrates/includes/aggregates Notepad++ into a proprietary executable
//    installer, such as those produced by InstallShield.
// 3. Links to a library or executes a program that does any of the above.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#ifndef IMAGE_LIST_H
#define IMAGE_LIST_H



class CNppImageList
{
public :
	CNppImageList() : _hImglst(NULL), _iconCount(0), _iconSize(0){};
	virtual ~CNppImageList() 
	{
		destroy();
	}
	virtual bool create(int cx,int cy,UINT flags = (ILC_COLOR32 | ILC_MASK), int cInitial = 3, int cGrow = 6);
	virtual bool destroy();
	HIMAGELIST getSysImgLst(UINT uFlags = SHGFI_SMALLICON);
	int addIcon(HINSTANCE hInst, int iconID);
	bool delIcon(int iconID);
	int addImage(HBITMAP hbmImage,HBITMAP hbmMask=NULL);
	bool changeIcon(int index, const TCHAR *iconLocation) const;
	void setIconSize(int size) const;
	HIMAGELIST getImglst()const;
	int getCount()const;
	void clear();
	HICON getHIcon(int indx,UINT uFlags);
private :
	HIMAGELIST _hImglst;
	int _iconCount;
	int _iconSize;
};

#endif //IMAGE_LIST_H
