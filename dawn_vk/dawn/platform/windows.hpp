# include <memory>
# include <functional>
# include "dawn/common.hpp"

# include "windows.h"


template<typename Module_handle_type__> 
struct Module_base : public std::enable_shared_from_this<Module_base<Module_handle_type__>> {
public:
    using Sptr = std::shared_ptr<Module_base>;

    Module_handle_type__ handle;

    static Sptr get_new(
        const char* const name_,
        std::function<void()> fail_execute_ = []() {}
    ) {
        dbg_show << "����ģ��:" << name_;
        Sptr sptr;
        Module_handle_type__ moudle_handle = LoadLibraryA(name_);
        if (moudle_handle == NULL) {
            err_show << "���� " << name_ << " ʧ��";
            fail_execute_();
            return sptr;
        }
        else {
            dbg_show << "���سɹ�:" << std::to_string(size_t(moudle_handle));
        }
        sptr.reset(new Module_base);
        sptr->handle = moudle_handle;
        return std::move(sptr);
    }
    template<typename Function_ptr_type__>
    Function_ptr_type__ load(
        const char* const function_name_,
        std::function<void()> fail_execute_ = []() {}
    ) {
        //dbg_show("��ģ��:" << std::to_string((size_t)handle) << " ���غ���:" << function_name_);
        auto fp = (Function_ptr_type__)GetProcAddress(handle, (LPCSTR)function_name_);
        if (fp != NULL) {
            //dbg_show("�ɹ���ú�����ַ:" << std::to_string((size_t)fp));
        }
        else {
            //err_show("��ȡ������ַʧ��");
            fail_execute_();
        }
        return fp;
    }
    ~Module_base() {
        FreeLibrary(handle);
    }
private:
    Module_base() {};
};

namespace dawn{

struct F_wnd_class_style {
    UINT flag;
    /* ����ƶ����С�������Ŀͻ����ĸ߶ȣ����ػ��������ڡ� */
    constexpr static UINT v_redraw = CS_VREDRAW;
    /* ����ƶ����С�������Ŀͻ����Ŀ�ȣ����ػ��������ڡ� */
    constexpr static UINT h_redraw = CS_HREDRAW;
    /* ���û������ڸ���Ĵ�����˫�����ʱ���򴰿ڹ��̷���˫����Ϣ�� */
    constexpr static UINT dbl_clks = CS_DBLCLKS;
    /* Ϊ���е�ÿ�����ڷ���Ψһ���豸�����ġ� */
    constexpr static UINT one_window_one_dc = CS_OWNDC;
    /*
        ���Ӵ��ڵļ��о�������Ϊ�����ڵļ��о��Σ��Ա��Ӵ��ڿ����ڸ������ϻ��ơ�
        ����CS_PARENTDC��ʽλ�Ĵ��ڴ�ϵͳ���豸�����Ļ����н��ճ����豸�����ġ�
        �������������ṩ������豸�����Ļ��豸���������á�
        ָ��CS_PARENTDC����ǿӦ�ó�������ܡ�
    */
    constexpr static UINT parent_dc = CS_PARENTDC;
    /* ���ô��ڲ˵��ϵĹرա� */
    constexpr static UINT no_close = CS_NOCLOSE;
    /*
    ��λͼ����ʽ�����ɴ��ര���ڵ�����Ļͼ�񲿷֡�
    ɾ�����ں�ϵͳ��ʹ�ñ����λͼ�ָ���Ļͼ�񣬰����������ڵ��Ĵ��ڡ�
    ��ˣ����λͼʹ�õ��ڴ���δ������������������Ļ����δʹ�洢��ͼ����Ч����ϵͳ�������ڵ��Ĵ��ڷ���WM_PAINT��Ϣ��
    ����ʽ���ڶ�����ʾȻ����������Ļ�����֮ǰ��ɾ����С���ڣ����磬�˵���Ի��򣩷ǳ����á�
    ������ʽ��������ʾ���������ʱ�䣬��Ϊϵͳ�������ȷ����ڴ����洢λͼ��
    */
    constexpr static UINT save_bits = CS_SAVEBITS;
    /* �ڴ��ڱ߽磨x�����϶��봰�ڵĿͻ����򡣴���ʽ��Ӱ�촰�ڵĿ�ȼ�������ʾ���ϵ�ˮƽλ�á� */
    constexpr static UINT byte_align_client = CS_BYTEALIGNCLIENT;
    /* �ڴ��ڱ߽磨x�����϶��봰�ڡ�����ʽ��Ӱ�촰�ڵĿ�ȼ�������ʾ���ϵ�ˮƽλ�á� */
    constexpr static UINT byte_align_window = CS_BYTEALIGNWINDOW;
    /* ָʾ��������Ӧ�ó���ȫ���ࡣ */
    constexpr static UINT global_class = CS_GLOBALCLASS;
    /*
        ����һ���豸�������Թ����е����д��ڹ���
        ���ڴ��������ض��ڽ��̵ģ����Ӧ�ó���Ķ���߳̿��Դ���ͬһ��Ĵ��ڡ�
        �߳�Ҳ���Գ���ͬʱʹ���豸�����ġ������������ʱ��ϵͳֻ����һ���̳߳ɹ�������ͼ������
    */
    constexpr static UINT class_dc = CS_CLASSDC;
    constexpr static UINT ime = CS_IME;
#if(_WIN32_WINNT >= 0x0501)
    /*
        �ڴ�����������ӰЧ����
        ͨ��SPI_SETDROPSHADOW�򿪺͹ر�Ч����
        ͨ�������������˵�֮���С�Ķ�������������Ч�ģ���ǿ���������������ڵ�Z˳���ϵ��
        �Ӿ��д���ʽ���ഴ����Windows�����Ƕ�������;���ǿ��ܲ��Ƕ�ͯ���ڡ�
    */
    constexpr static UINT drop_shadow = CS_DROPSHADOW;
#endif /* _WIN32_WINNT >= 0x0501 */
    F_wnd_class_style(int i_) :flag(static_cast<UINT>(i_)) {}
    operator UINT& () { return flag; }
};

struct F_wnd_style {
    DWORD flag;
    /* �������ص��Ĵ��ڡ� �ص����ھ��б������ͱ߿� ��b_tiled��ʽ��ͬ�� */
    constexpr static DWORD overlapped = WS_OVERLAPPED;
    /* ���������һ���ص��Ĵ��ڡ��ص����ھ��б������ͱ߿���b_overlapped��ʽ��ͬ�� */
    constexpr static DWORD tiled = WS_TILED;
    /* ���������һ���ص��Ĵ��ڡ���b_overlapped_window�����ͬ�� */
    constexpr static DWORD tiled_window = WS_TILEDWINDOW;
    /* ���������һ���ص��Ĵ��ڡ���b_tiled_window�����ͬ��*/
    constexpr static DWORD overlapped_window = WS_OVERLAPPEDWINDOW;
    /* ���������һ���Ӵ��ڡ����д���ʽ�Ĵ��ڲ����в˵������ؼ�����ʹ�ô���ʽ��WS������� */
    constexpr static DWORD child = WS_CHILD;
    /* ��һ���������ڡ� ����ʽ������b_child��ʽһ��ʹ�� */
    constexpr static DWORD popup = WS_POPUP;
    /* ��һ���������ڡ� �������b_caption��b_popup_window��ʽ����ʹ���ڲ˵��ɼ��� */
    constexpr static DWORD popup_window = WS_POPUPWINDOW;
    /*
        �ô�����һ��ؼ��ĵ�һ���ؼ���
        �����ɵ�һ���ؼ��������������пؼ���ɣ�ֱ������b_group��ʽ����һ���ؼ���
        ÿ�����еĵ�һ���ؼ�ͨ������b_tab_stop��ʽ���Ա��û����Դ�һ�����ƶ�����һ���顣
        �û�������ͨ��ʹ�÷���������̽�������е�һ���ؼ�����Ϊ���е���һ���ؼ���
        �����Դ򿪺͹رմ���ʽ�Ը��ĶԻ��򵼺���
        Ҫ�ڴ������ں���Ĵ���ʽ����ʹ��SetWindowLong������
    */
    constexpr static DWORD group = WS_GROUP;
    /*
        �ô�����һ���ؼ������û�����TAB��ʱ���ÿؼ����Խ��ռ��̽��㡣
        ��TAB���ɽ����̽������Ϊb_tab_stop��ʽ����һ���ؼ���
        �����Դ򿪺͹رմ���ʽ�Ը��ĶԻ��򵼺���
        Ҫ�ڴ������ں���Ĵ���ʽ����ʹ��SetWindowLong������
        �����û������Ĵ��ں���ģʽ�Ի�����ʹ���Ʊ�λ���������Ϣѭ���Ե���IsDialogMessage������
    */
    constexpr static DWORD tab_stop = WS_TABSTOP;

    /* ����ͼ�����ڸ�������ʱ���������Ӵ���ռ�õ����򡣴���������ʱʹ�ô���ʽ�� */
    constexpr static DWORD clipchildren = WS_CLIPCHILDREN;
    /*
        �����Ӵ�������ڱ˴�;
        Ҳ����˵�����ض��Ӵ����յ�WM_PAINT��Ϣʱ��
        b_clip_siblings��ʽ�����Ҫ���µ��Ӵ�������֮������������ص��Ӵ��ڡ�
        ���δָ��b_clip_siblings�����Ӵ����ص���
        �����Ӵ��ڵĿͻ������ڻ���ʱ�������������Ӵ��ڵĿͻ������ڻ��ơ�
    */
    constexpr static DWORD clip_siblings = WS_CLIPSIBLINGS;

    /* ���������һ��ϸ�߿�. */
    constexpr static DWORD border = WS_BORDER;
    /* ������һ��������(����WS�߽���)��*/
    constexpr static DWORD caption = WS_CAPTION;
    /* ������һ��������(����WS�߽���)��*/
    constexpr static DWORD child_window = WS_CHILDWINDOW;
    /* ����������һ�����ڲ˵��� ������ָ��b_caption��ʽ�� */
    constexpr static DWORD system_menu = WS_SYSMENU;
    /* ������һ��ˮƽ�������� */
    constexpr static DWORD h_scroll = WS_HSCROLL;
    /* ������һ����ֱ������ */
    constexpr static DWORD v_scroll = WS_VSCROLL;
    /* ������һ����С����ť���޷���WS_EX_CONTEXTHELP��ʽ��ϡ�������ָ��b_system_menu��ʽ��*/
    constexpr static DWORD minimize_box = WS_MINIMIZEBOX;
    /* ������һ���ߴ�߿� ��b_thick_frame��ʽ��ͬ�� */
    constexpr static DWORD size_box = WS_SIZEBOX;
    /* ������һ���ߴ�߿� ��b_size_box��ʽ��ͬ�� */
    constexpr static DWORD thick_frame = WS_THICKFRAME;
    /* ������һ����󻯰�ť���޷���WS_EX_CONTEXTHELP��ʽ��ϡ�������ָ��b_system_menu��ʽ��*/
    constexpr static DWORD maximize_box = WS_MAXIMIZEBOX;
    /* ���ھ���ͨ����Ի���һ��ʹ�õ���ʽ�ı߿� ���д���ʽ�Ĵ��ڲ����б������� */
    constexpr static DWORD dialog_frame = WS_DLGFRAME;

    /*
        �ô�����������á�
        ���õĴ����޷������û������롣
        Ҫ�ڴ������ں���Ĵ����ã���ʹ��EnableWindow���ܡ�
    */
    constexpr static DWORD disabled = WS_DISABLED;
    /* �����������С���� ��b_minimize��ʽ��ͬ�� */
    constexpr static DWORD iconic = WS_ICONIC;
    /* �����������С���� ��b_iconic��ʽ��ͬ�� */
    constexpr static DWORD minimize = WS_MINIMIZE;
    /* �����������󻯵� */
    constexpr static DWORD maximize = WS_MAXIMIZE;
    /* ��������ǿɼ��ġ�ͨ��ʹ��ShowWindow��SetWindowPos���������Դ򿪻�رմ���ʽ�� */
    constexpr static DWORD visible = WS_VISIBLE;
    F_wnd_style(int i_) :flag(static_cast<DWORD>(i_)) {}
    operator DWORD& () { return flag; }
};

struct F_wnd_ex_style {
    DWORD flag;
    /*
        ������һ���ֲ㴰�ڡ�
        ������ھ���CS_OWNDC��CS_CLASSDC������ʽ������ʹ�ô���ʽ��
        Windows 8���������ں��Ӵ���֧��WS_EX_LAYERED��ʽ��
        ��ǰ��Windows�汾��֧�ֶ������ڵ�WS_EX_LAYERED��
    */
    constexpr static DWORD layered = WS_EX_LAYERED;

    /* ���������һ���ص��Ĵ��ڡ� */
    constexpr static DWORD overlapped_window = WS_EX_OVERLAPPEDWINDOW;

    /* �ô�����һ��MDI�Ӵ��ڡ� */
    constexpr static DWORD mdi_child = WS_EX_MDICHILD;

    /* �����ǵ�ɫ�崰�ڣ�����һ����ģʽ�Ի�����ʾһϵ����� */
    constexpr static DWORD palette_window = WS_EX_PALETTEWINDOW;

    /*
        �ô���ּ������������������
        ���ߴ��ڵı���������ͨ�������̣����ڱ���ʹ�ý�С��������ơ�
        ���û�����ALT + TABʱ�����ߴ��ڲ����������������Ի����С�
        ������ߴ��ھ���ϵͳ�˵�������ͼ�겻����ʾ�ڱ������ϡ�
        ���ǣ�������ͨ���Ҽ����������ALT + SPACE����ʾϵͳ�˵���
    */
    constexpr static DWORD tool_window = WS_EX_TOOLWINDOW;

    /*
        ����Ӧ�������з����Ĵ����Ϸ�������Ӧ�ñ���������֮�ϣ���ʹ���ڱ�ͣ��Ҳ����ˡ�
        Ҫ��ӻ�ɾ������ʽ����ʹ��SetWindowPos������
    */
    constexpr static DWORD topmost = WS_EX_TOPMOST;

    /*
        ʹ�ô���ʽ�����Ķ����������û�����ʱ�����Ϊǰ̨���ڡ�
        ���û���С����ر�ǰ������ʱ��ϵͳ���Ὣ�˴�������ǰ̨��
        ��Ӧͨ��������ʻ�ͨ���ɷ��ʼ��������署���ˣ�ͨ�����̵���������ڡ�
        Ҫ����ڣ���ʹ��SetActiveWindow��SetForegroundWindow������
        Ĭ������£����ڲ���������������ϡ�
        Ҫǿ�ƴ�����ʾ���������ϣ���ʹ��WS_EX_APPWINDOW��ʽ��
    */
    constexpr static DWORD no_activete = WS_EX_NOACTIVATE;

    /* ���ڲ��Ὣ�䴰�ڲ��ִ��ݸ����Ӵ��� */
    constexpr static DWORD no_inherit_layout = WS_EX_NOINHERITLAYOUT;

    /* ʹ�ô���ʽ�������Ӵ����ڴ���������ʱ���ὫWM_PARENTNOTIFY��Ϣ���͵��丸���ڡ� */
    constexpr static DWORD no_parent_notify = WS_EX_NOPARENTNOTIFY;

    /* ���ڲ�����Ⱦ���ض�����档 ��������û�пɼ����ݻ�ʹ�ñ�������Ļ������ṩ�Ӿ�Ч���Ĵ��ڡ� */
    constexpr static DWORD no_rediction_bitmap = WS_EX_NOREDIRECTIONBITMAP;

    /* �����ڿɼ�ʱ�������㴰��ǿ�Ƶ���������. */
    constexpr static DWORD app_window = WS_EX_APPWINDOW;

    /* �ô��ڽ����Ϸ��ļ��� */
    constexpr static DWORD accept_files = WS_EX_ACCEPTFILES;

    /* �ô�����һ��˫�߿򣻿���ѡ��ʹ�ñ�������dwStyle������ָ��WS_caption��ʽ�������ô��ڡ� */
    constexpr static DWORD dialog_modal_frame = WS_EX_DLGMODALFRAME;

    /* �ô��ھ���ͨ�õ���������ԡ�����Ĭ�ϵġ� */
    constexpr static DWORD left = WS_EX_LEFT;

    /* �����ı�ʹ�ô����ҵĶ�ȡ˳��������ʾ������Ĭ�ϵġ� */
    constexpr static DWORD ltr_reading = WS_EX_LTRREADING;

    /*
        ʹ��˫�����Եײ�����������˳����ƴ��ڵ����к����
        ������ھ���CS_OWNDC��CS_CLASSDC������ʽ������ʹ�ô˷�����
        Windows 2000����֧�ִ���ʽ��
    */
    constexpr static DWORD composited = WS_EX_COMPOSITED;

    /*
        �ڴ����·����ֵܽ��ã���ͬһ���̴߳�������Ϳ��֮�󣬲�Ӧ�öԴ��ڽ��л��ơ�
        ������ʾΪ͸������Ϊ�ײ��ֵܴ��ڵ�λ�Ѿ������ơ�
        Ҫ��û����Щ���Ƶ������ʵ��͸���ȣ���ʹ��SetWindowRgn������
    */
    constexpr static DWORD transparent = WS_EX_TRANSPARENT;

    /* �ô��ھ�����ά�߿���ʽ��ּ�����ڲ������û��������Ŀ�� */
    constexpr static DWORD static_edge = WS_EX_STATICEDGE;

    /* ��ֱ��������������ڣ�λ�ڿͻ����Ҳࡣ ����Ĭ��ֵ�� */
    constexpr static DWORD right_scroll_bar = WS_EX_RIGHTSCROLLBAR;

    /*
        ���ڱ������Ӧ�ò���Ի��򵼺����Ӵ��ڡ�
        ���ָ���˴���ʽ����Ի���������ִ�е�������
        �����紦��TAB������ͷ����������Ƿ���ʱ��ݹ鵽�˴��ڵ������С�
    */
    constexpr static DWORD control_parent = WS_EX_CONTROLPARENT;

    /* �����ı߿��а��ݵı�Ե�� */
    constexpr static DWORD client_edge = WS_EX_CLIENTEDGE;

    /* ������һ����͹���Ե�ı߿� */
    constexpr static DWORD window_edge = WS_EX_WINDOWEDGE;

    /*
        ���ڵı����������ʺš�
        ���û������ʺ�ʱ����꽫��Ϊ����ָ����ʺš�
        ����û�Ȼ�󵥻��Ӵ��ڣ�������յ�WM_HELP��Ϣ��
        �Ӵ���Ӧ�ý���Ϣ���ݸ������ڹ��̣��ù���Ӧ��ʹ��HELP_WM_HELP�������WinHelp������
        ��������Ӧ�ó�����ʾһ���������ڣ��ô���ͨ�������Ӵ��ڵİ�����
        WS_EX_CONTEXTHELP������WS_MAXIMIZEBOX��WS_MINIMIZEBOX��ʽһ��ʹ�á�
    */
    constexpr static DWORD context_hlep = WS_EX_CONTEXTHELP;

    /*
        �ô��ھ���ͨ�õġ��Ҷ��롱���ԡ� ��ȡ���ڴ����ࡣ
        ����shell������ϣ��������������֧���Ķ�˳��������������ʱ������ʽ����Ч; ���򣬷�񱻺��ԡ�
        �Ծ�̬��༭�ؼ�ʹ��WS_EX_RIGHT��ʽ�ֱ���ʹ��SS_RIGHT��ES_RIGHT��ʽ������ͬ��Ч����
        ʹ�ô��а�ť�ؼ��Ĵ���ʽ��ʹ��BS_RIGHT��BS_RIGHTBUTTON��ʽ������ͬ��Ч����
    */
    constexpr static DWORD right = WS_EX_RIGHT;
    /*
        ���shell������ϣ��������������֧���Ķ�˳�������������ԣ�
        ��ֱ��������������ڣ�λ�ڿͻ�����ࡣ �����������ԣ������Ը���ʽ��
    */
    constexpr static DWORD left_scroll_bar = WS_EX_LEFTSCROLLBAR;
    /*
        ���shell������ϣ��������������֧���Ķ�˳�������������ԣ�
        ��ʹ�ô��ҵ�����Ķ�˳��������ʾ�����ı��� �����������ԣ������Ը���ʽ��
    */
    constexpr static DWORD rtl_reading = WS_EX_RTLREADING;
    /*
        ���shell������ϣ��������������֧���Ķ�˳�������������ԣ�
        �򴰿ڵ�ˮƽԭ��λ���ұ�Ե�� ����ˮƽֵ����ǰ����
    */
    constexpr static DWORD layout_rtl = WS_EX_LAYOUTRTL;
    F_wnd_ex_style(int i_) :flag(static_cast<DWORD>(i_)) {}
    operator DWORD& () { return flag; }
};


struct S_wnd_class;

struct S_wnd : std::enable_shared_from_this<S_wnd> {
public:
    using Sptr = Sp<S_wnd>;
public:
    HWND handle;
    Sp<S_wnd_class> father_sptr;
public:
    inL ~S_wnd() {
        if (DestroyWindow(handle) == FALSE) {
            wrn_show<<"���� ����ʧ��";
        }
    }
public:

    inL bool set_prop(LPCTSTR lpString_, HANDLE hData_) {
        return SetProp(handle, lpString_, hData_) == TRUE;
    }

    inL HANDLE get_prop(LPCTSTR lpString_) {
        return GetProp(handle, lpString_);
    }
    inL void show(uint32_t flag = SW_SHOWNORMAL) {
        ShowWindow(handle, flag);
    }

    inL void update() {
        UpdateWindow(handle);
    }

    inL std::array<uint32_t, 2> get_size() {
        RECT rect;
        GetWindowRect(handle, &rect);
        return {
            static_cast<uint32_t>(rect.right - rect.left),
            static_cast<uint32_t>(rect.bottom - rect.top)
        };
    }
    inL std::array<uint32_t, 2> get_draw_size() {
        WINDOWPLACEMENT rect;
        GetWindowPlacement(handle, &rect);
        return {
            static_cast<uint32_t>(rect.rcNormalPosition.right - rect.rcNormalPosition.left),
            static_cast<uint32_t>(rect.rcNormalPosition.bottom - rect.rcNormalPosition.top)
        };
    }
};

struct S_wnd_class :public WNDCLASSEX, std::enable_shared_from_this<S_wnd_class> {
public:
    using Sptr = Sp<S_wnd_class>;
protected:
    using WNDCLASSEX::WNDCLASSEX;
public:
    static inL Sptr create(
        HINSTANCE           hInstance_,
        LPCWSTR             lpszClassName_,
        WNDPROC             wnd_process_,
        F_wnd_class_style   style_ = F_wnd_class_style::h_redraw | F_wnd_class_style::v_redraw,
        HBRUSH              hbrBackground_ = (HBRUSH)GetStockObject(BLACK_BRUSH),
        HICON               hIcon_ = LoadIcon(NULL, IDI_APPLICATION),
        HCURSOR             hCursor_ = LoadCursor(NULL, IDC_ARROW),
        HICON               hIconSm_ = LoadIcon(NULL, IDI_WINLOGO),
        LPCWSTR             lpszMenuName_ = NULL,
        int                 cls_extra_ = 0,
        int                 wnd_extra_ = 0
    ) {
        auto result = std::make_shared<S_wnd_class>();
        result->cbSize = sizeof(WNDCLASSEX);
        result->hInstance = hInstance_;
        result->lpszClassName = lpszClassName_;
        result->lpfnWndProc = wnd_process_;
        result->style = style_;
        result->hbrBackground = hbrBackground_;
        result->hIcon = hIcon_;
        result->hCursor = hCursor_;
        result->hIconSm = hIconSm_;
        result->lpszMenuName = lpszMenuName_;
        result->cbClsExtra = cls_extra_;
        result->cbWndExtra = wnd_extra_;

        auto ret = RegisterClassEx(result.get());
        if (!ret) {
            err_show << "������ ע��ʧ��";
            result.reset();
        }
        return std::move(result);
    }
    inL ~S_wnd_class() {
        auto ret = UnregisterClass(lpszClassName, hInstance);
        if (!ret) {
            wrn_show<<"������ ע��ʧ��";
        }
    }
public:
    inL S_wnd::Sptr create_window(
        LPCWSTR         window_name_,
        int             x_ = 0,
        int             y_ = 0,
        int             width_ = 1280,
        int             height_ = 768,
        F_wnd_style     style_ = F_wnd_style::overlapped,
        F_wnd_ex_style  ex_style_ = 0,
        HWND            hWndParent_ = NULL,
        HMENU           hMenu_ = NULL,
        LPVOID          lpParam_ = NULL
    ) {
        RECT wr{ 0,0,width_,height_ };
        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
        auto result = std::make_shared<S_wnd>();
        auto wnd = CreateWindowEx(
            ex_style_,
            lpszClassName,
            window_name_,
            style_,
            x_,
            y_,
            wr.right - wr.left,
            wr.bottom - wr.top,
            hWndParent_,
            hMenu_,
            hInstance,
            lpParam_
        );
        if (wnd == NULL) {
            err_show << "���� ����ʧ��!";
            result.reset();
        }
        result->handle = wnd;
        result->father_sptr = shared_from_this();
        return std::move(result);
    }
    inL HWND create_a_window(
        LPCWSTR         window_name_,
        int             x_ = 0,
        int             y_ = 0,
        int             width_ = 1280,
        int             height_ = 768,
        F_wnd_style     style_ = F_wnd_style::clip_siblings |
        F_wnd_style::clipchildren |
        F_wnd_style::visible,
        F_wnd_ex_style  ex_style_ = 0,
        HWND            hWndParent_ = NULL,
        HMENU           hMenu_ = NULL,
        LPVOID          lpParam_ = NULL
    ) {
        return CreateWindowEx(
            ex_style_,
            lpszClassName,
            window_name_,
            style_,
            x_,
            y_,
            width_,
            height_,
            hWndParent_,
            hMenu_,
            hInstance,
            lpParam_
        );
    }
};

}