#pragma once
#include <Windows.h>
#include "paulslib.h"

typedef struct COLOR3UB_S {
	unsigned char r, g, b;
} COLOR3UB;

void OpenSelectColorDialog(HWND parent, COLOR3UB *color);

// ������ ������ ������ ����� ��� ��������
//	 _In_ HWND window - ������������ ����
//	 _Out_ char *path - ����� ��� ���������� ���������� ����
//	 _In_ const char *initialdir - ���� ������������ ��� ������ �������
//	 _In_ const char *filter - ���������� ���������� ��� ������ ������
BOOL OpenFileDialog(_In_ HWND window, _Out_ char *path, _In_ const char *initialdir, _In_ const char *filter);

// ������� ������ ���������� �����
//	 _In_ HWND window - ������������ ����
//	 _Out_ char *path - ����� ��� ���������� ���������� ����
//	 _In_ const char *initialdir - ���� ������������ ��� ������ �������
//	 _In_ const char *filter - ���������� ���������� ��� ���������� ������
BOOL SaveFileDialog(_In_ HWND window, _Out_ char *path, _In_ const char *initialdir, _In_ const char *filter);