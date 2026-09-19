// Build: x86_64-w64-mingw32-gcc -O2 -Wall -Wextra -municode -mwindows syscolor-demo.c -o syscolor-demo.exe -lcomctl32 -lgdi32 -luser32
#include <windows.h>
#include <commctrl.h>

#define IDC_CONTROLS 100
#define IDC_SWATCHES 101
#define IDC_SCROLLBOX 102
#define IDC_STATUS 103

static HWND g_controls;
static HWND g_swatches;
static HWND g_scrollbox;
static HWND g_status;
static HWND g_tooltip;

static const struct {
    int idx;
    const wchar_t *name;
    const wchar_t *regname;
} k_colors[] = {
    { COLOR_SCROLLBAR,              L"COLOR_SCROLLBAR",              L"Scrollbar" },
    { COLOR_BACKGROUND,             L"COLOR_BACKGROUND",             L"Background" },
    { COLOR_ACTIVECAPTION,          L"COLOR_ACTIVECAPTION",          L"ActiveTitle" },
    { COLOR_INACTIVECAPTION,        L"COLOR_INACTIVECAPTION",        L"InactiveTitle" },
    { COLOR_MENU,                   L"COLOR_MENU",                   L"Menu" },
    { COLOR_WINDOW,                 L"COLOR_WINDOW",                 L"Window" },
    { COLOR_WINDOWFRAME,            L"COLOR_WINDOWFRAME",            L"WindowFrame" },
    { COLOR_MENUTEXT,               L"COLOR_MENUTEXT",               L"MenuText" },
    { COLOR_WINDOWTEXT,             L"COLOR_WINDOWTEXT",             L"WindowText" },
    { COLOR_CAPTIONTEXT,            L"COLOR_CAPTIONTEXT",            L"TitleText" },
    { COLOR_ACTIVEBORDER,           L"COLOR_ACTIVEBORDER",           L"ActiveBorder" },
    { COLOR_INACTIVEBORDER,         L"COLOR_INACTIVEBORDER",         L"InactiveBorder" },
    { COLOR_APPWORKSPACE,           L"COLOR_APPWORKSPACE",           L"AppWorkSpace" },
    { COLOR_HIGHLIGHT,              L"COLOR_HIGHLIGHT",              L"Hilight" },
    { COLOR_HIGHLIGHTTEXT,          L"COLOR_HIGHLIGHTTEXT",          L"HilightText" },
    { COLOR_BTNFACE,                L"COLOR_BTNFACE",                L"ButtonFace" },
    { COLOR_BTNSHADOW,              L"COLOR_BTNSHADOW",              L"ButtonShadow" },
    { COLOR_GRAYTEXT,               L"COLOR_GRAYTEXT",               L"GrayText" },
    { COLOR_BTNTEXT,                L"COLOR_BTNTEXT",                L"ButtonText" },
    { COLOR_INACTIVECAPTIONTEXT,    L"COLOR_INACTIVECAPTIONTEXT",    L"InactiveTitleText" },
    { COLOR_BTNHIGHLIGHT,           L"COLOR_BTNHIGHLIGHT",           L"ButtonHilight" },
    { COLOR_3DDKSHADOW,             L"COLOR_3DDKSHADOW",             L"ButtonDkShadow" },
    { COLOR_3DLIGHT,                L"COLOR_3DLIGHT",                L"ButtonLight" },
    { COLOR_INFOTEXT,               L"COLOR_INFOTEXT",               L"InfoText" },
    { COLOR_INFOBK,                 L"COLOR_INFOBK",                 L"InfoWindow" },
    { COLOR_HOTLIGHT,               L"COLOR_HOTLIGHT",               L"HotTrackingColor" },
    { COLOR_GRADIENTACTIVECAPTION,  L"COLOR_GRADIENTACTIVECAPTION",  L"GradientActiveTitle" },
    { COLOR_GRADIENTINACTIVECAPTION,L"COLOR_GRADIENTINACTIVECAPTION",L"GradientInactiveTitle" },
    { COLOR_MENUHILIGHT,            L"COLOR_MENUHILIGHT",            L"MenuHilight" },
    { COLOR_MENUBAR,                L"COLOR_MENUBAR",                L"MenuBar" },
};

static void add_tip(HWND parent, HWND target, const wchar_t *text)
{
    if (!g_tooltip) {
        g_tooltip = CreateWindowExW(WS_EX_TOPMOST, TOOLTIPS_CLASSW, NULL,
            WS_POPUP | TTS_ALWAYSTIP, 0, 0, 0, 0, parent, NULL, GetModuleHandleW(NULL), NULL);
        SetWindowPos(g_tooltip, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    TOOLINFOW ti;
    ZeroMemory(&ti, sizeof(ti));
    ti.cbSize = sizeof(ti);
    ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    ti.hwnd = parent;
    ti.uId = (UINT_PTR)target;
    ti.lpszText = (LPWSTR)text;
    SendMessageW(g_tooltip, TTM_ADDTOOLW, 0, (LPARAM)&ti);
}

static void add_combo_items(HWND combo)
{
    SendMessageW(combo, CB_ADDSTRING, 0, (LPARAM)L"Combo item 1");
    SendMessageW(combo, CB_ADDSTRING, 0, (LPARAM)L"Combo item 2");
    SendMessageW(combo, CB_ADDSTRING, 0, (LPARAM)L"Combo item 3");
    SendMessageW(combo, CB_SETCURSEL, 0, 0);
}

static void add_list_items(HWND list)
{
    SendMessageW(list, LB_ADDSTRING, 0, (LPARAM)L"Selected list row");
    SendMessageW(list, LB_ADDSTRING, 0, (LPARAM)L"Normal list row");
    SendMessageW(list, LB_ADDSTRING, 0, (LPARAM)L"Another row");
    SendMessageW(list, LB_SETCURSEL, 0, 0);
}

static void build_controls(HWND parent)
{
    HWND h;

    CreateWindowW(L"BUTTON", L"Buttons", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        10, 8, 210, 108, parent, NULL, NULL, NULL);
    CreateWindowW(L"BUTTON", L"Push button", WS_CHILD | WS_VISIBLE,
        22, 30, 92, 26, parent, NULL, NULL, NULL);
    HWND disabled_button = CreateWindowW(L"BUTTON", L"Disabled", WS_CHILD | WS_VISIBLE | WS_DISABLED,
        122, 30, 82, 26, parent, NULL, NULL, NULL);
    h = CreateWindowW(L"BUTTON", L"Check box", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        22, 64, 94, 22, parent, NULL, NULL, NULL);
    SendMessageW(h, BM_SETCHECK, BST_CHECKED, 0);
    CreateWindowW(L"BUTTON", L"Radio", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        122, 64, 74, 22, parent, NULL, NULL, NULL);
    EnableWindow(disabled_button, FALSE);

    CreateWindowW(L"BUTTON", L"Text and edit", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        230, 8, 255, 108, parent, NULL, NULL, NULL);
    CreateWindowW(L"STATIC", L"Static WindowText", WS_CHILD | WS_VISIBLE,
        242, 32, 130, 20, parent, NULL, NULL, NULL);
    CreateWindowW(L"STATIC", L"Disabled GrayText", WS_CHILD | WS_VISIBLE | WS_DISABLED,
        242, 58, 130, 20, parent, NULL, NULL, NULL);
    h = CreateWindowW(L"EDIT", L"Editable text", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        372, 30, 100, 24, parent, NULL, NULL, NULL);
    add_tip(parent, h, L"Tooltip uses InfoWindow and InfoText");
    CreateWindowW(L"EDIT", L"Read-only", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
        372, 62, 100, 24, parent, NULL, NULL, NULL);

    HWND tab = CreateWindowW(WC_TABCONTROLW, NULL,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
        10, 126, 475, 48, parent, NULL, NULL, NULL);
    TCITEMW tci;
    ZeroMemory(&tci, sizeof(tci));
    tci.mask = TCIF_TEXT;
    tci.pszText = L"Tab A";
    TabCtrl_InsertItem(tab, 0, &tci);
    tci.pszText = L"Tab B";
    TabCtrl_InsertItem(tab, 1, &tci);
    tci.pszText = L"Tab C";
    TabCtrl_InsertItem(tab, 2, &tci);

    CreateWindowW(L"BUTTON", L"Lists and selection", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        10, 184, 475, 172, parent, NULL, NULL, NULL);
    h = CreateWindowW(WC_COMBOBOXW, NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
        22, 208, 150, 120, parent, NULL, NULL, NULL);
    add_combo_items(h);
    h = CreateWindowW(WC_LISTBOXW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
        22, 242, 150, 86, parent, NULL, NULL, NULL);
    add_list_items(h);

    HWND lv = CreateWindowW(WC_LISTVIEWW, NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS,
        190, 208, 280, 120, parent, NULL, NULL, NULL);
    LVCOLUMNW col;
    ZeroMemory(&col, sizeof(col));
    col.mask = LVCF_TEXT | LVCF_WIDTH;
    col.cx = 128;
    col.pszText = L"Column A";
    ListView_InsertColumn(lv, 0, &col);
    col.pszText = L"Column B";
    ListView_InsertColumn(lv, 1, &col);
    LVITEMW item;
    ZeroMemory(&item, sizeof(item));
    item.mask = LVIF_TEXT | LVIF_STATE;
    item.iItem = 0;
    item.pszText = L"Selected";
    item.state = LVIS_SELECTED | LVIS_FOCUSED;
    item.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
    ListView_InsertItem(lv, &item);
    ListView_SetItemText(lv, 0, 1, L"Hilight");
    item.iItem = 1;
    item.pszText = L"Normal";
    item.state = 0;
    item.stateMask = 0;
    ListView_InsertItem(lv, &item);
    ListView_SetItemText(lv, 1, 1, L"Window");

    CreateWindowW(L"BUTTON", L"Common controls", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        10, 366, 475, 176, parent, NULL, NULL, NULL);
    HWND tree = CreateWindowW(WC_TREEVIEWW, NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
        22, 390, 160, 130, parent, NULL, NULL, NULL);
    TVINSERTSTRUCTW ti;
    ZeroMemory(&ti, sizeof(ti));
    ti.hInsertAfter = TVI_LAST;
    ti.hParent = TVI_ROOT;
    ti.item.mask = TVIF_TEXT;
    ti.item.pszText = L"Tree root";
    HTREEITEM root = TreeView_InsertItem(tree, &ti);
    ti.hParent = root;
    ti.item.pszText = L"Selected child";
    HTREEITEM child = TreeView_InsertItem(tree, &ti);
    TreeView_Expand(tree, root, TVE_EXPAND);
    TreeView_SelectItem(tree, child);

    HWND track = CreateWindowW(TRACKBAR_CLASSW, NULL, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
        200, 390, 260, 32, parent, NULL, NULL, NULL);
    SendMessageW(track, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));
    SendMessageW(track, TBM_SETPOS, TRUE, 45);
    HWND progress = CreateWindowW(PROGRESS_CLASSW, NULL, WS_CHILD | WS_VISIBLE,
        200, 432, 260, 20, parent, NULL, NULL, NULL);
    SendMessageW(progress, PBM_SETPOS, 65, 0);
    CreateWindowW(DATETIMEPICK_CLASSW, NULL, WS_CHILD | WS_VISIBLE,
        200, 464, 150, 24, parent, NULL, NULL, NULL);
    CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT,
        438, 462, 22, 28, parent, NULL, NULL, NULL);
    CreateWindowW(WC_LINK, L"<a href=\"https://example.invalid/\">SysLink hot tracking</a>",
        WS_CHILD | WS_VISIBLE, 200, 500, 220, 22, parent, NULL, NULL, NULL);
}

static void draw_swatches(HWND hwnd, HDC hdc)
{
    RECT rc;
    GetClientRect(hwnd, &rc);
    FillRect(hdc, &rc, GetSysColorBrush(COLOR_WINDOW));
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));

    int x = 12;
    int y = 12;
    const int box_w = 48;
    const int box_h = 22;
    const int row_h = 38;
    const int col_w = 350;

    for (unsigned i = 0; i < sizeof(k_colors) / sizeof(k_colors[0]); ++i) {
        COLORREF c = GetSysColor(k_colors[i].idx);
        HBRUSH brush = CreateSolidBrush(c);
        RECT box = { x, y, x + box_w, y + box_h };
        FillRect(hdc, &box, brush);
        FrameRect(hdc, &box, GetSysColorBrush(COLOR_WINDOWTEXT));
        DeleteObject(brush);

        wchar_t text[256];
        wsprintfW(text, L"%s / %s\r\nRGB(%u,%u,%u) index %d",
            k_colors[i].name, k_colors[i].regname,
            GetRValue(c), GetGValue(c), GetBValue(c), k_colors[i].idx);

        RECT tr = { x + box_w + 8, y - 1, x + col_w - 8, y + row_h };
        DrawTextW(hdc, text, -1, &tr, DT_LEFT | DT_TOP | DT_NOPREFIX);

        y += row_h;
        if (y + row_h > rc.bottom - 8) {
            y = 12;
            x += col_w;
        }
    }
}

static LRESULT CALLBACK SwatchesProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        draw_swatches(hwnd, hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_SYSCOLORCHANGE:
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

static LRESULT CALLBACK ScrollBoxProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static int pos_x;
    static int pos_y;

    switch (msg) {
    case WM_CREATE:
        SetScrollRange(hwnd, SB_HORZ, 0, 100, FALSE);
        SetScrollRange(hwnd, SB_VERT, 0, 100, FALSE);
        return 0;
    case WM_HSCROLL:
        switch (LOWORD(wparam)) {
        case SB_LINELEFT: pos_x -= 1; break;
        case SB_LINERIGHT: pos_x += 1; break;
        case SB_PAGELEFT: pos_x -= 10; break;
        case SB_PAGERIGHT: pos_x += 10; break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION: pos_x = HIWORD(wparam); break;
        }
        if (pos_x < 0) pos_x = 0;
        if (pos_x > 100) pos_x = 100;
        SetScrollPos(hwnd, SB_HORZ, pos_x, TRUE);
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    case WM_VSCROLL:
        switch (LOWORD(wparam)) {
        case SB_LINEUP: pos_y -= 1; break;
        case SB_LINEDOWN: pos_y += 1; break;
        case SB_PAGEUP: pos_y -= 10; break;
        case SB_PAGEDOWN: pos_y += 10; break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION: pos_y = HIWORD(wparam); break;
        }
        if (pos_y < 0) pos_y = 0;
        if (pos_y > 100) pos_y = 100;
        SetScrollPos(hwnd, SB_VERT, pos_y, TRUE);
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, GetSysColorBrush(COLOR_APPWORKSPACE));
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
        DrawTextW(hdc, L"Native WS_HSCROLL / WS_VSCROLL area", -1, &rc,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

static HMENU make_menu(void)
{
    HMENU bar = CreateMenu();
    HMENU file = CreatePopupMenu();
    HMENU view = CreatePopupMenu();

    AppendMenuW(file, MF_STRING, 1, L"&New");
    AppendMenuW(file, MF_STRING, 2, L"&Open");
    AppendMenuW(file, MF_SEPARATOR, 0, NULL);
    AppendMenuW(file, MF_STRING, 3, L"E&xit");
    AppendMenuW(bar, MF_POPUP, (UINT_PTR)file, L"&File");

    AppendMenuW(view, MF_STRING | MF_CHECKED, 10, L"Checked item");
    AppendMenuW(view, MF_STRING | MF_GRAYED, 11, L"Disabled item");
    AppendMenuW(bar, MF_POPUP, (UINT_PTR)view, L"&View");

    return bar;
}

static void layout(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    int status_h = 24;
    SendMessageW(g_status, WM_SIZE, 0, 0);

    int gap = 10;
    int left_w = 500;
    int bottom_h = 72;
    int top = 10;
    int bottom = rc.bottom - status_h - gap;
    int main_h = bottom - top - bottom_h - gap;
    int swatch_x = left_w + gap * 2;
    int swatch_w = rc.right - swatch_x - gap;

    MoveWindow(g_controls, gap, top, left_w, main_h, TRUE);
    MoveWindow(g_scrollbox, gap, top + main_h + gap, left_w, bottom_h, TRUE);
    MoveWindow(g_swatches, swatch_x, top, swatch_w, bottom - top, TRUE);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg) {
    case WM_CREATE: {
        INITCOMMONCONTROLSEX icc;
        icc.dwSize = sizeof(icc);
        icc.dwICC = ICC_WIN95_CLASSES | ICC_BAR_CLASSES | ICC_TAB_CLASSES |
            ICC_PROGRESS_CLASS | ICC_TREEVIEW_CLASSES | ICC_LISTVIEW_CLASSES |
            ICC_UPDOWN_CLASS | ICC_STANDARD_CLASSES | ICC_LINK_CLASS |
            ICC_DATE_CLASSES;
        InitCommonControlsEx(&icc);

        SetMenu(hwnd, make_menu());

        WNDCLASSW wc;
        ZeroMemory(&wc, sizeof(wc));
        wc.lpfnWndProc = SwatchesProc;
        wc.hInstance = GetModuleHandleW(NULL);
        wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = L"SysColorDemoSwatches";
        RegisterClassW(&wc);

        ZeroMemory(&wc, sizeof(wc));
        wc.lpfnWndProc = ScrollBoxProc;
        wc.hInstance = GetModuleHandleW(NULL);
        wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
        wc.lpszClassName = L"SysColorDemoScrollBox";
        RegisterClassW(&wc);

        g_controls = CreateWindowW(L"STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            0, 0, 0, 0, hwnd, (HMENU)IDC_CONTROLS, NULL, NULL);
        build_controls(g_controls);

        g_scrollbox = CreateWindowW(L"SysColorDemoScrollBox", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL,
            0, 0, 0, 0, hwnd, (HMENU)IDC_SCROLLBOX, NULL, NULL);

        g_swatches = CreateWindowW(L"SysColorDemoSwatches", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            0, 0, 0, 0, hwnd, (HMENU)IDC_SWATCHES, NULL, NULL);

        g_status = CreateWindowW(STATUSCLASSNAMEW,
            L"Menu, tooltip, status bar, native scrollbars, common controls, and GetSysColor swatches",
            WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)IDC_STATUS, NULL, NULL);

        layout(hwnd);
        return 0;
    }
    case WM_SIZE:
        if (g_controls && g_swatches && g_scrollbox && g_status)
            layout(hwnd);
        return 0;
    case WM_GETMINMAXINFO: {
        MINMAXINFO *mmi = (MINMAXINFO *)lparam;
        mmi->ptMinTrackSize.x = 1040;
        mmi->ptMinTrackSize.y = 760;
        return 0;
    }
    case WM_COMMAND:
        if (LOWORD(wparam) == 3)
            PostMessageW(hwnd, WM_CLOSE, 0, 0);
        return 0;
    case WM_SYSCOLORCHANGE:
        InvalidateRect(hwnd, NULL, TRUE);
        InvalidateRect(g_controls, NULL, TRUE);
        InvalidateRect(g_swatches, NULL, TRUE);
        InvalidateRect(g_scrollbox, NULL, TRUE);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

int APIENTRY wWinMain(HINSTANCE hinst, HINSTANCE prev, LPWSTR cmd, int show)
{
    (void)prev;
    (void)cmd;

    WNDCLASSW wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hinst;
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"SysColorDemoMain";
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(L"SysColorDemoMain",
        L"Wine System Color Demo",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 1180, 760,
        NULL, NULL, hinst, NULL);
    if (!hwnd)
        return 1;

    ShowWindow(hwnd, show);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
