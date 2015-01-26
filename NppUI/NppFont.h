#ifndef NPPFONT_H
#define NPPFONT_H

class CNppFont
{
public:
	CNppFont();
	~CNppFont();
	void destroy();
	HFONT getFont(HWND hctrl)const;
	// @brief: dialog ²»Ö§³Ö
	void setFont(HWND hctrl, HFONT hfont, BOOL redraw = TRUE)const;
	void setFont(HWND hTo, HWND hFrom);
	void setFont(HWND hctrl, LPLOGFONT lpLogFont, BOOL redraw = TRUE);
	LOGFONT& getLogFont(HWND hctrl)const;
private:
	LPLOGFONT m_lpLogFont;
	HFONT     m_hFont;
};
#endif
