#ifndef _MAIN_ONE_
#define _MAIN_ONE_

int handle_file(LPCTSTR lpFilePath);
int create_menu();
LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
