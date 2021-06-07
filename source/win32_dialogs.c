#include "win32_dialogs.h"

void OpenSelectColorDialog(HWND parent, COLOR3UB *color)
{
	CHOOSECOLOR cc;
	static COLORREF custColors[16];
	BYTE rr = 255;
	BYTE gg = 255;
	BYTE bb = 255;
	memset(&cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = parent;
	cc.rgbResult = RGB(rr, gg, bb);
	cc.lpCustColors = custColors;
	cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
	if (ChooseColorA(&cc)) {
		color->r = (int)GetRValue(cc.rgbResult);
		color->g = (int)GetGValue(cc.rgbResult);
		color->b = (int)GetBValue(cc.rgbResult);
	}
}

// Создаёт диалог выбора файла для открытия
//	 _In_ HWND window - родительское окно
//	 _Out_ char *path - буфер для сохранения выбранного пути
//	 _In_ const char *initialdir - путь отображаемый при старте диалога
//	 _In_ const char *filter - расширения подходящих для выбора файлов
BOOL OpenFileDialog(_In_ HWND window, _Out_ char *path, _In_ const char *initialdir, _In_ const char *filter)
{
	OPENFILENAMEA ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = window;
	ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = path;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrInitialDir = initialdir;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	return GetOpenFileNameA(&ofn);
}

// Открыть диалог сохранения файла
//	 _In_ HWND window - родительское окно
//	 _Out_ char *path - буфер для сохранения выбранного пути
//	 _In_ const char *initialdir - путь отображаемый при старте диалога
//	 _In_ const char *filter - расширения подходящих для сохранения файлов
BOOL SaveFileDialog(_In_ HWND window, _Out_ char *path, _In_ const char *initialdir, _In_ const char *filter)
{
	OPENFILENAMEA ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = window;
	ofn.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = path;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrInitialDir = initialdir;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	return GetSaveFileNameA(&ofn);
}
