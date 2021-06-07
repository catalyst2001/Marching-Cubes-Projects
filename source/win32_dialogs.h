#pragma once
#include <Windows.h>
#include "paulslib.h"

typedef struct COLOR3UB_S {
	unsigned char r, g, b;
} COLOR3UB;

void OpenSelectColorDialog(HWND parent, COLOR3UB *color);

// Создаёт диалог выбора файла для открытия
//	 _In_ HWND window - родительское окно
//	 _Out_ char *path - буфер для сохранения выбранного пути
//	 _In_ const char *initialdir - путь отображаемый при старте диалога
//	 _In_ const char *filter - расширения подходящих для выбора файлов
BOOL OpenFileDialog(_In_ HWND window, _Out_ char *path, _In_ const char *initialdir, _In_ const char *filter);

// Открыть диалог сохранения файла
//	 _In_ HWND window - родительское окно
//	 _Out_ char *path - буфер для сохранения выбранного пути
//	 _In_ const char *initialdir - путь отображаемый при старте диалога
//	 _In_ const char *filter - расширения подходящих для сохранения файлов
BOOL SaveFileDialog(_In_ HWND window, _Out_ char *path, _In_ const char *initialdir, _In_ const char *filter);