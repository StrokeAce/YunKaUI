// Copyright (c) 2011 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_
#pragma once

#include <list>
#include <map>
#include <set>
#include <string>

#include <include/base/cef_lock.h>
#include <include/cef_client.h>
#include <include/wrapper/cef_helpers.h>
#include <include/wrapper/cef_message_router.h>

using namespace std;

#if defined(OS_LINUX)
// The Linux client uses GTK instead of the underlying platform type (X11).
#include <gtk/gtk.h>
#define ClientWindowHandle GtkWidget*
#else
#define ClientWindowHandle CefWindowHandle
#endif

#define ON_AFTER_CREATED				(WM_USER + 666) // 浏览器创建成功后的消息
#define ON_AFTER_LOAD					(WM_USER + 667) // 浏览器成功reload后的消息
#define ON_JS_CALL_MFC					(WM_USER + 668) // js调用C++的消息
#define JS_CALL_RESTART_SESSION			(WM_USER + 669) // 二次会话重启
#define JS_CALL_START_RECORD			(WM_USER + 670) // 开始录音
#define JS_CALL_SEND_AUDIO				(WM_USER + 671) // 发送录音
#define JS_CALL_CANCEL_RECORD			(WM_USER + 672) // 取消录音
#define JS_CALL_VIEW_DETAILS			(WM_USER + 673) // 查看详情，看大图，看坐标等
#define JS_CALL_RESEND_FILE				(WM_USER + 674) // 重新发送媒体文件
#define JS_CALL_RERECV_FILE				(WM_USER + 675) // 重新接收媒体文件
#define JS_CALL_CHANGE_CHAT_OBJECT		(WM_USER + 676) // 切换聊天对象
#define JS_CALL_RESEND_MSG				(WM_USER + 677) // 重新发送消息

class CRect;
class CUserObject;
class CWebUserObject;
class ClientHandler;

class HandlerInfo
{
public:
	string handleName;
	CefRefPtr<ClientHandler> handler;
	bool isLoaded;	// 是否已加载
	bool isCreated; // 是否已创建页面
};

// Define this value to redirect all popup URLs to the main application browser
// window.
// #define TEST_REDIRECT_POPUP_URLS

// ClientHandler implementation.
class ClientHandler : public CefClient,
                      public CefContextMenuHandler,
                      public CefDialogHandler,
                      public CefDisplayHandler,
                      public CefDownloadHandler,
                      public CefDragHandler,
                      public CefGeolocationHandler,
                      public CefJSDialogHandler,
                      public CefKeyboardHandler,
                      public CefLifeSpanHandler,
                      public CefLoadHandler,
                      public CefRenderHandler,
                      public CefRequestHandler {
 public:
  // Interface implemented to handle off-screen rendering.
  class RenderHandler : public CefRenderHandler {
   public:
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) =0;
  };

  typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

  ClientHandler();
  virtual ~ClientHandler();

  // CefClient methods
  virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefDialogHandler> GetDialogHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE {
    return this;
  }
  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message)
                                        OVERRIDE;

  // CefContextMenuHandler methods
  virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefContextMenuParams> params,
                                   CefRefPtr<CefMenuModel> model) OVERRIDE;
  virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefContextMenuParams> params,
                                    int command_id,
                                    EventFlags event_flags) OVERRIDE;

  // CefDialogHandler methods
  virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
                            FileDialogMode mode,
                            const CefString& title,
                            const CefString& default_file_name,
                            const std::vector<CefString>& accept_types,
                            CefRefPtr<CefFileDialogCallback> callback) OVERRIDE;

  // CefDisplayHandler methods
  virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               const CefString& url) OVERRIDE;
  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
                             const CefString& title) OVERRIDE;
  virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                const CefString& message,
                                const CefString& source,
                                int line) OVERRIDE;

  // CefDownloadHandler methods
  virtual void OnBeforeDownload(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefDownloadItem> download_item,
      const CefString& suggested_name,
      CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;
  virtual void OnDownloadUpdated(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefDownloadItem> download_item,
      CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

  // CefDragHandler methods
  virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefDragData> dragData,
                           CefDragHandler::DragOperationsMask mask) OVERRIDE;

  // CefGeolocationHandler methods
  virtual bool OnRequestGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      const CefString& requesting_url,
      int request_id,
      CefRefPtr<CefGeolocationCallback> callback) OVERRIDE;

  // CefJSDialogHandler methods
  virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                          const CefString& origin_url,
                          const CefString& accept_lang,
                          JSDialogType dialog_type,
                          const CefString& message_text,
                          const CefString& default_prompt_text,
                          CefRefPtr<CefJSDialogCallback> callback,
                          bool& suppress_message) OVERRIDE;
  virtual bool OnBeforeUnloadDialog(
      CefRefPtr<CefBrowser> browser,
      const CefString& message_text,
      bool is_reload,
      CefRefPtr<CefJSDialogCallback> callback) OVERRIDE;
  virtual void OnResetDialogState(CefRefPtr<CefBrowser> browser) OVERRIDE;

  // CefKeyboardHandler methods
  virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                             const CefKeyEvent& event,
                             CefEventHandle os_event,
                             bool* is_keyboard_shortcut) OVERRIDE;

  // CefLifeSpanHandler methods
  virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             const CefString& target_url,
                             const CefString& target_frame_name,
                             const CefPopupFeatures& popupFeatures,
                             CefWindowInfo& windowInfo,
                             CefRefPtr<CefClient>& client,
                             CefBrowserSettings& settings,
                             bool* no_javascript_access) OVERRIDE;
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

  // CefLoadHandler methods
  virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                    bool isLoading,
                                    bool canGoBack,
                                    bool canGoForward) OVERRIDE;
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) OVERRIDE;
  virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
						 CefRefPtr<CefFrame> frame,
						 int httpStatusCode) OVERRIDE;

  // CefRequestHandler methods
  virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              CefRefPtr<CefRequest> request,
                              bool is_redirect) OVERRIDE;
  virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request) OVERRIDE;
  virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                              const CefString& origin_url,
                              int64 new_size,
                              CefRefPtr<CefQuotaCallback> callback) OVERRIDE;
  virtual void OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                                   const CefString& url,
                                   bool& allow_os_execution) OVERRIDE;
  virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                         TerminationStatus status) OVERRIDE;

  // CefRenderHandler methods
  virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
                                 CefRect& rect) OVERRIDE;
  virtual bool GetViewRect(CefRefPtr<CefBrowser> browser,
                           CefRect& rect) OVERRIDE;
  virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
                              int viewX,
                              int viewY,
                              int& screenX,
                              int& screenY) OVERRIDE;
  virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
                             CefScreenInfo& screen_info) OVERRIDE;
  virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE;
  virtual void OnPopupSize(CefRefPtr<CefBrowser> browser,
                           const CefRect& rect) OVERRIDE;
  virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                       PaintElementType type,
                       const RectList& dirtyRects,
                       const void* buffer,
                       int width,
                       int height) OVERRIDE;
  virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                              CefCursorHandle cursor,
                              CursorType type,
                              const CefCursorInfo& custom_cursor_info) OVERRIDE;
  virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefDragData> drag_data,
                             CefRenderHandler::DragOperationsMask allowed_ops,
                             int x, int y) OVERRIDE;
  virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser,
                                CefRenderHandler::DragOperation operation)
                                OVERRIDE;

  void SetMainWindowHandle(ClientWindowHandle handle);
  ClientWindowHandle GetMainWindowHandle() const;
  void SetEditWindowHandle(ClientWindowHandle handle);
  void SetButtonWindowHandles(ClientWindowHandle backHandle,
                              ClientWindowHandle forwardHandle,
                              ClientWindowHandle reloadHandle,
                              ClientWindowHandle stopHandle);

  void SetOSRHandler(CefRefPtr<RenderHandler> handler);
  CefRefPtr<RenderHandler> GetOSRHandler() const;

  CefRefPtr<CefBrowser> GetBrowser() const;
  int GetBrowserId() const;

  // Request that all existing browser windows close.
  void CloseAllBrowsers(bool force_close);

  // Returns true if the main browser window is currently closing. Used in
  // combination with DoClose() and the OS close notification to properly handle
  // 'onbeforeunload' JavaScript events during window close.
  bool IsClosing() const;

  std::string GetLogFile() const;

  void SetLastDownloadFile(const std::string& fileName);
  std::string GetLastDownloadFile() const;

  // Send a notification to the application. Notifications should not block the
  // caller.
  enum NotificationType {
    NOTIFY_CONSOLE_MESSAGE,
    NOTIFY_DOWNLOAD_COMPLETE,
    NOTIFY_DOWNLOAD_ERROR,
  };
  void SendNotification(NotificationType type);

  void ShowDevTools(CefRefPtr<CefBrowser> browser,
                    const CefPoint& inspect_element_at);
  void CloseDevTools(CefRefPtr<CefBrowser> browser);

  // Returns the startup URL.
  std::string GetStartupURL() const;

  void BeginTracing();
  void EndTracing();

  bool Save(const std::string& path, const std::string& data);

  /////////////////////////gaowb/////////////////////////////
  // 创建浏览器
  bool CreateBrowser(HWND hwnd, RECT rect, string url, string handlerName);


  void MoveBrowser(RECT rect);

  // 显示或隐藏窗口
  void ShowBrowser(int nCmdShow);

  // 直接加载html内容
  void LoadString(string html);

 private:
  void SetLoading(bool isLoading);
  void SetNavState(bool canGoBack, bool canGoForward);

  // Create all CefMessageRouterBrowserSide::Handler objects. They will be
  // deleted when the ClientHandler is destroyed.
  static void CreateMessageHandlers(MessageHandlerSet& handlers);

  bool ExecuteTestMenu(int command_id);
  struct TestMenuState {
    TestMenuState() : check_item(true), radio_item(0) {}
    bool check_item;
    int radio_item;
  } test_menu_state_;

  // Returns the full download path for the specified file, or an empty path to
  // use the default temp directory.
  std::string GetDownloadPath(const std::string& file_name);

  // START THREAD SAFE MEMBERS
  // The following members are thread-safe because they're initialized during
  // object construction and not changed thereafter.

  // The startup URL.
  std::string startup_url_;

  // True if mouse cursor change is disabled.
  bool mouse_cursor_change_disabled_;
  // END THREAD SAFE MEMBERS

  // Lock used to protect members accessed on multiple threads. Make it mutable
  // so that it can be used from const methods.
  mutable base::Lock lock_;

  // START LOCK PROTECTED MEMBERS
  // The following members are accessed on multiple threads and must be
  // protected by |lock_|.

  // The child browser window.
  CefRefPtr<CefBrowser> browser_;

  // The child browser id.
  int browser_id_;

  // True if the main browser window is currently closing.
  bool is_closing_;
  // END LOCK PROTECTED MEMBERS

  // START UI THREAD ACCESS ONLY MEMBERS
  // The following members will only be accessed on the CEF UI thread.

  // List of any popup browser windows.
  typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
  BrowserList popup_browsers_;

  // The main frame window handle.
  ClientWindowHandle main_handle_;

  //// The edit window handle.
  //ClientWindowHandle edit_handle_;

  //// The button window handles.
  //ClientWindowHandle back_handle_;
  //ClientWindowHandle forward_handle_;
  //ClientWindowHandle stop_handle_;
  //ClientWindowHandle reload_handle_;

  // The handler for off-screen rendering, if any.
  CefRefPtr<RenderHandler> osr_handler_;

  // Support for logging.
  std::string log_file_;

  // Support for downloading files.
  std::string last_download_file_;

  // True if an editable field currently has focus.
  bool focus_on_editable_field_;

  // Handles the browser side of query routing. The renderer side is handled
  // in client_renderer.cpp.
  CefRefPtr<CefMessageRouterBrowserSide> message_router_;

  // Set of Handlers registered with the message router.
  MessageHandlerSet message_handler_set_;

  // Number of currently existing browser windows. The application will exit
  // when the number of windows reaches 0.
  static int browser_count_;

  // 嵌入的主窗口句柄
  HWND m_hWnd;
  // 页面的名字
  string m_handlerName;
public:
  // 右键菜单是否展示刷新项
  bool m_isDisplayCopy;
  bool m_isDisplayRefresh;

#if defined(OS_LINUX)
  // Linux-only implementation of GTK-based dialog boxes.
  static void OnDialogResponse(GtkDialog *dialog,
                               gint response_id,
                               ClientHandler* handler);
  GtkWidget* gtk_dialog_;
  CefRefPtr<CefJSDialogCallback> js_dialog_callback_;
#endif
  // END UI THREAD ACCESS ONLY MEMBERS

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ClientHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_
