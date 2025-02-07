#include <QPainter>
#include <QApplication>
#include <QDebug>
#include "CutMask.h"
#include "MainWindow.h"
#include "Util.h"
#include "PixelInfo.h"

MainWindow::MainWindow(QObject* parent) : QObject(parent)
{
	initWinPosSize();
	initImgs();
    initWindow();
}

MainWindow::~MainWindow()
{
	
}
void MainWindow::initWindow()
{
    WNDCLASSEX wcx{};
    auto hinstance = GetModuleHandle(NULL);
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcx.lpfnWndProc = &MainWindow::routeWinMsg;
    wcx.cbWndExtra = sizeof(MainWindow*);
    wcx.hInstance = hinstance;
    wcx.hIcon = LoadIcon(hinstance, IDI_APPLICATION);
    wcx.hCursor = LoadCursor(NULL, IDC_CROSS);
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcx.lpszClassName = L"ScreenCapture";
    RegisterClassEx(&wcx);
    auto style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP;
    hwnd = CreateWindowEx(NULL ,L"ScreenCapture", L"ScreenCapture", style, x, y, w, h, NULL, NULL, hinstance, static_cast<LPVOID>(this));
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}
void MainWindow::show()
{
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    SetCursor(LoadCursor(nullptr, IDC_ARROW));
    pixelInfo = new PixelInfo(this);
    cutMask = new CutMask(this);

}
void MainWindow::initWinPosSize()
{
    x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
}

void MainWindow::initImgs()
{
    imgScreen = Util::printScreen(x, y, w, h);
    imgScreen.setDevicePixelRatio(1.0);
    imgBoard = QImage(w, h, QImage::Format_ARGB32_Premultiplied);
    imgBoard.fill(Qt::transparent);
    imgBoard.setDevicePixelRatio(1.0);
    imgCanvas = QImage(w, h, QImage::Format_ARGB32_Premultiplied);
    imgCanvas.fill(Qt::transparent);
    imgCanvas.setDevicePixelRatio(1.0);
}

void MainWindow::paint()
{
    QPainter painter(&imgBoard);
    painter.drawImage(0, 0, imgScreen);
    painter.drawImage(0, 0, imgCanvas);
    if (state >= State::mask) {
		cutMask->paint(&painter);
    }
}
LRESULT MainWindow::routeWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto obj = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (!obj) {
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    switch (msg)
    {
        case WM_NCCALCSIZE:
        {
            if (wParam == TRUE) {
                NCCALCSIZE_PARAMS* pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
                pncsp->rgrc[0] = pncsp->rgrc[1]; //窗口客户区覆盖整个窗口
                return 0; //确认改变窗口客户区
            }
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        case WM_CLOSE:
        {
            DestroyWindow(hWnd);
            return 0;
        }
        case WM_DESTROY:
        {
            SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
            UnregisterClass(L"ScreenCapture", nullptr);
            return 0;
        }
        default:
        {
            return obj->processWinMsg(msg, wParam, lParam);
        }
    }
}
LRESULT MainWindow::processWinMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_MOVE: {
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            return 0;
        }
        case WM_PAINT: {
            paint();
            PAINTSTRUCT ps;
            auto dc = BeginPaint(hwnd, &ps);
            BITMAPINFO bmi = { sizeof(BITMAPINFOHEADER), w, 0 - h, 1, 32, BI_RGB, h * 4 * w, 0, 0, 0, 0 };
            SetDIBitsToDevice(dc, 0, 0, w, h, 0, 0, 0, h, imgBoard.bits(), &bmi, DIB_RGB_COLORS);
            ReleaseDC(hwnd, dc);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            auto e = Util::createMouseEvent(lParam, QEvent::MouseButtonPress);
            mousePress(&e);
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            auto e = Util::createMouseEvent(lParam, QEvent::MouseMove);
            if (wParam & MK_LBUTTON) {
                mouseDrag(&e);
            }
            else {
                mouseMove(&e);
            }
            return 0;
        }
        case WM_LBUTTONDBLCLK:
        {
            auto e = Util::createMouseEvent(lParam, QEvent::MouseButtonDblClick);
            mouseDBClick(&e);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            auto e = Util::createMouseEvent(lParam, QEvent::MouseButtonRelease);
            mouseRelease(&e);
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            auto e = Util::createMouseEvent(lParam, QEvent::MouseButtonPress, Qt::MouseButton::RightButton);
            mousePressRight(&e);
            return 0;
        }
        case WM_KEYDOWN:
        {
            if (wParam == VK_BACK || wParam == VK_DELETE) {
                //removeShape();
                return true;
            }
            else if (wParam == VK_ESCAPE) {
                //escPress();
                return true;
            }
            else if (wParam == VK_LEFT) {
                //moveByKey(0);
                return true;
            }
            else if (wParam == VK_UP) {
                //moveByKey(1);
                return true;
            }
            else if (wParam == VK_RIGHT) {
                //moveByKey(2);
                return true;
            }
            else if (wParam == VK_DOWN) {
                //moveByKey(3);
                return true;
            }
            else if ((GetKeyState(VK_CONTROL) & 0x8000) != 0) {
                if (wParam == 'Z') {
                    //undo(); 撤销
                    return true;
                }
                else if (wParam == 'Y') {
                    //redo(); 重做
                    return true;
                }
                else if (wParam == 'H') {
                    //copyColor(0); Hex
                    return true;
                }
                else if (wParam == 'R') {
                    //copyColor(1); RGB
                    return true;
                }
            }
            break;
        }
        default: {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
    return 0;
}
void MainWindow::mouseMove(QMouseEvent* event)
{
    if (state < State::mask) {
        pixelInfo->mouseMove(event->pos());
    }
}
void MainWindow::mouseDrag(QMouseEvent* event) {
	if (state == State::mask) {
		cutMask->mouseDrag(event);
	}
}
void MainWindow::mousePress(QMouseEvent* event)
{
	if (state == State::start) {
        pixelInfo->close();
        cutMask->mousePress(event);
	}
}