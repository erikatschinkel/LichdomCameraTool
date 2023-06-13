#pragma once
#include <d3d11.h>
#include <Windows.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
#include <dinput.h>
#include <math.h>
#include <vector>

extern HINSTANCE g_gameHandle;

//This file is a mess, I know, but I kept it as information. It could be useful to someone else.

namespace CryLich {

    class CTimeValue {
    public:
        __int64 m_lValue;

    };



    struct ISystem {
        
    };

    struct IEntity {

    };

    struct IRenderer {

    };

    struct ILog {

    };

    //This vtable is extremely useful--you can manipulate any CVar this way, once you grab the IConsole object
    struct ICVar
    {
        virtual ~ICVar() {}
        virtual void Release();
        virtual int GetIVal() = 0;
        virtual float GetFVal() = 0;
        virtual const char* GetString() = 0;
        virtual void Set(const int i) = 0;
        //virtual void Set(const float f) = 0;
        //virtual void Set(char const* c) = 0;
        //virtual void ForceSet(char const* c) = 0;
        virtual void funcSet00();
        virtual void funcSet01();
        virtual void funcSet02();
        virtual void ClearFlags(int flags);
        virtual int GetFlags() const;
        virtual int SetFlags(int flags);
        virtual int GetType();
        virtual const char* GetName() = 0;
        virtual const char* GetHelp();
        virtual bool IsConstCVar() const;
        virtual void func1();
        virtual void func2();
        virtual void func3();
        virtual void func4();
        virtual bool func5();

    };

    struct IConsole {
        virtual ~IConsole() {};
        virtual void Release();
        virtual void func00();
        virtual void func01();
        virtual void func02();
        virtual void func03();
        virtual void func04();
        virtual void func05();
        virtual void func06();
        virtual void func07();
        virtual void func08();
        virtual void func09();
        virtual void func10();
        virtual void func11();
        virtual void func12();
        virtual void func13();
        virtual void func14();
        virtual void func15();
        virtual void func16();
        virtual void func17();
        virtual void func18();
        virtual ICVar* GetCVar(const char *sName) = 0;

    };

    struct ITimer {
        virtual void func00();
        virtual void func01();
        virtual void func02();
        virtual void func03();
        virtual void func04();
        virtual void func05();
        virtual void func06();
        virtual void func07();
        virtual void func08();
        virtual void func09();
        virtual void func10();
        virtual void func11();
        virtual void func12();
        virtual void func13();
        virtual void func14();
        virtual void func15();
        virtual void func16();
        virtual void func17();
        virtual void func18();
        virtual bool PauseTimer(enum ETimer which, bool pause) = 0; //This does a full game pause, like in the menu, not helpful

    };

    

    enum ETimer : int {
        ETIMER_GAME = 0x0,
        ETIMER_UI = 0x1,
        ETIMER_LAST = 0x2
    };



    struct I3DEngine {

    };


    struct SRenderThread {

    };

    struct SViewport {

        int nX;
        int nY;
        int nWidth;
        int nHeight;
        float fMinZ;
        float fMaxZ;

    };

    struct MTRenderInfo {

    };

    //There are of course more of these, but these seemed useful to me
    struct SCVars {
        char Pad000[0x18];
        float cl_fov;
        float cl_defaultfov;
        int cl_fovmaint;
        float cl_fov_aspect_ratio;
        char Pad028[0x14];
        float cl_nearPlane;
        char Pad040[0x2c0];
        int g_godMode;
        char Pad304[0x80];
        int sv_pacifist;
        char Pad388[0x428];
        int xav_showDebugOptions;
        int xav_UseMapLoader;
        int xav_skipTutorialMovies;
        int xav_skipMotionGraphics;
        char Pad7c0[0xb4];
        int g_UseTressFX;
        char Pad878[0x1f8]; //size 0xa70


    };

    enum DXUTDeviceVersion {
        DXUT_D3D9_DEVICE = 0x0,
        DXUT_D3D11_DEVICE = 0x1
        
    };
    
    enum _D3DDEVTYPE {

    };
    
    enum _D3DFORMAT {

    };

    enum _D3DMULTISAMPLE_TYPE {

    };

    enum _D3DSWAPEFFECT {

    };

    enum D3D_DRIVER_TYPE {

    };


    enum DXGI_FORMAT {

    };

    struct _D3DPRESENT_PARAMETERS_ {
        unsigned int BackBufferWidth;
        unsigned int BackBufferHeight;
        _D3DFORMAT BackBufferFormat;
        unsigned int BackBufferCount;
        _D3DMULTISAMPLE_TYPE MultiSampleType;
        unsigned int MultiSampleQuality;
        _D3DSWAPEFFECT SwapEffect;
        HWND* hDeviceWindow;
        int Windowed;
        int EnableAutoDepthStencil;
        _D3DFORMAT AutoDepthStencilFormat;
        unsigned int Flags;
        unsigned int FullScreen_RefreshRateInHz;
        unsigned int PresentationInterval;

    };

    struct DXUTD3D9DeviceSettings {
        unsigned int AdapterOrdinal;
        _D3DDEVTYPE DeviceType;
        _D3DFORMAT AdapterFormat;
        unsigned int BehaviorFlags;
        _D3DPRESENT_PARAMETERS_ pp;

    };

    struct DXUTD3D11DeviceSettings {
        unsigned int AdapterOrdinal;
        D3D_DRIVER_TYPE DriverType;
        unsigned int Output;
        char Pad00c[0x4];
        DXGI_SWAP_CHAIN_DESC sd;
        unsigned int CreateFlags;
        unsigned int SyncInterval;
        unsigned int PresentFlags;
        unsigned int AutoCreateDepthStencil;
        DXGI_FORMAT AutoDepthStencilFormat;
        D3D_FEATURE_LEVEL DeviceFeatureLevel;

    };

    struct DXUTDeviceSettings {
        enum DXUTDeviceVersion ver;
        D3D_FEATURE_LEVEL MinimumFeatureLevel;
        DXUTD3D9DeviceSettings d3d9;
        DXUTD3D11DeviceSettings d3d11;
    };

    struct SD3DSurface {
        int nWidth;
        int nHeight;
        unsigned int bBusy;
        int nFrameAccess;
        void* pSurf;
        void* pTex;
    };

    //My initial hotsampling attempts resulted in me writing down this vtable
    //Ultimately not necessary
    class CD3D9Renderer {
    public:
        virtual void func001();
        virtual void func002();
        virtual void func003();
        virtual void func004();
        virtual void func005();
        virtual void func006();
        virtual void func007();
        virtual void func008();
        virtual void func009();
        virtual void func010();
        virtual void func011();
        virtual void func012();
        virtual void func013();
        virtual void func014();
        virtual void func015();
        virtual void func016();
        virtual void func017();
        virtual void func018();
        virtual void func019();
        virtual void func020();
        virtual bool DeleteContext(HWND hWnd) = 0;
        virtual bool CreateContext(HWND hWnd, bool allowMSAA) = 0;
        virtual void func021();
        virtual void func022();
        virtual void func023();
        virtual void func024();
        virtual void func025();
        virtual void func026();
        virtual void func027();
        virtual void func028();
        virtual void func029();
        virtual void func030();
        virtual void func031();
        virtual void func032();
        virtual void func033();
        virtual void func034();
        virtual void func035();
        virtual void func036();
        virtual void func037();
        virtual void func038();
        virtual void func039();
        virtual void func040();
        virtual void func041();
        virtual void func042();
        virtual void func043();
        virtual void func044();
        virtual void func045();
        virtual void func046();
        virtual void func047();
        virtual void func048();
        virtual void func049();
        virtual void RestoreGamma() = 0;
        virtual void func050();
        virtual void func051();
        virtual void func052();
        virtual void func053();
        virtual void func054();
        virtual void func055();
        virtual void func056();
        virtual void func057();
        virtual void func058();
        virtual void func059();
        virtual void func060();
        virtual void func061();
        virtual void func062();
        virtual void func063();
        virtual void func064();
        virtual void func065();
        virtual void func066();
        virtual void func067();
        virtual void func068();
        virtual void func069();
        virtual void func070();
        virtual void func071();
        virtual void func072();
        virtual void func073();
        virtual void func074();
        virtual void func075();
        virtual void func076();
        virtual void func077();
        virtual void func078();
        virtual void func079();
        virtual void func080();
        virtual void func081();
        virtual void func082();
        virtual void func083();
        virtual void func084();
        virtual void func085();
        virtual void func086();
        virtual void func087();
        virtual void func088();
        virtual void func089();
        virtual void func090();
        virtual void func091();
        virtual void func092();
        virtual void func093();
        virtual void func094();
        virtual void func095();
        virtual void func096();
        virtual void func097();
        virtual void func098();
        virtual void func099();
        virtual void func100();
        virtual void func101();
        virtual void func102();
        virtual void func103();
        virtual void func104();
        virtual void func105();
        virtual void func106();
        virtual void func107();
        virtual void func108();
        virtual void func109();
        virtual void func110();
        virtual void func111();
        virtual void func112();
        virtual void func113();
        virtual void func114();
        virtual void func115();
        virtual void func116();
        virtual void func117();
        virtual void func118();
        virtual void func119();
        virtual void func120();
        virtual void func121();
        virtual void func122();
        virtual void func123();
        virtual void func124();
        virtual void func125();
        virtual void func126();
        virtual void func127();
        virtual void func128();
        virtual void func129();
        virtual void func130();
        virtual void func131();
        virtual void func132();
        virtual void func133();
        virtual void func134();
        virtual void func135();
        virtual void func136();
        virtual void func137();
        virtual void func138();
        virtual void func139();
        virtual void func140();
        virtual void func141();
        virtual void func142();
        virtual void func143();
        virtual void func144();
        virtual void func145();
        virtual void func146();
        virtual void func147();
        virtual void func148();
        virtual void func149();
        virtual void func150();
        virtual void func151();
        virtual void func152();
        virtual void func153();
        virtual void func154();
        virtual void func155();
        virtual void func156();
        virtual void func157();
        virtual void func158();
        virtual void func159();
        virtual void func160();
        virtual void func161();
        virtual void func162();
        virtual void func163();
        virtual void func164();
        virtual void func165();
        virtual void func166();
        virtual void func167();
        virtual void func168();
        virtual void func169();
        virtual void func170();
        virtual void func171();
        virtual void func172();
        virtual void func173();
        virtual void func174();
        virtual void func175();
        virtual void func176();
        virtual void func177();
        virtual void func178();
        virtual void func179();
        virtual void func180();
        virtual void func181();
        virtual void func182();
        virtual void func183();
        virtual void func184();
        virtual void func185();
        virtual void func186();
        virtual void func187();
        virtual void func188();
        virtual void func189();
        virtual void func190();
        virtual void func191();
        virtual void func192();
        virtual void func193();
        virtual void func194();
        virtual void func195();
        virtual void func196();
        virtual void func197();
        virtual void func198();
        virtual void func199();
        virtual void func200();
        virtual void func201();
        virtual void func202();
        virtual void func203();
        virtual void func204();
        virtual void func205();
        virtual void func206();
        virtual void func207();
        virtual void func208();
        virtual void func209();
        virtual void func210();
        virtual void func211();
        virtual void func212();
        virtual void func213();
        virtual void func214();
        virtual void func215();
        virtual void func216();
        virtual void func217();
        virtual void func218();
        virtual void func219();
        virtual void func220();
        virtual void func221();
        virtual void func222();
        virtual void func223();
        virtual void func224();
        virtual void func225();
        virtual void func226();
        virtual void func227();
        virtual void func228();
        virtual void func229();
        virtual void func230();
        virtual void func231();
        virtual void func232();
        virtual void func233();
        virtual void func234();
        virtual void func235();
        virtual void func236();
        virtual void func237();
        virtual void func238();
        virtual void func239();
        virtual void func240();
        virtual void func241();
        virtual void func242();
        virtual void func243();
        virtual void func244();
        virtual void func245();
        virtual void func246();
        virtual void func247();
        virtual void func248();
        virtual void func249();
        virtual void func250();
        virtual void func251();
        virtual void func252();
        virtual void func253();
        virtual void func254();
        virtual void func255();
        virtual void func256();
        virtual void func257();
        virtual void func258();
        virtual void func259();
        virtual void func260();
        virtual void func261();
        virtual void func262();
        virtual void func263();
        virtual void func264();
        virtual void func265();
        virtual void func266();
        virtual void func267();
        virtual void func268();
        virtual void func269();
        virtual void func270();
        virtual void func271();
        virtual void func272();
        virtual void func273();
        virtual void func274();
        virtual void func275();
        virtual void func276();
        virtual void func277();
        virtual void func278();
        virtual void func279();
        virtual void func280();
        virtual void func281();
        virtual void func282();
        virtual void func283();
        virtual void func284();
        virtual void func285();
        virtual void func286();
        virtual void func287();
        virtual void func288();
        virtual void func289();
        virtual void func290();
        virtual void func291();
        virtual void func292();
        virtual void func293();
        virtual void func294();
        virtual void func295();
        virtual void func296();
        virtual void func297();
        virtual void func298();
        virtual void func299();
        virtual void func300();
        virtual void func301();
        virtual void func302();
        virtual void func303();
        virtual void func304();
        virtual void func305();
        virtual void func306();
        virtual void func307();
        virtual void func308();
        virtual void func309();
        virtual void func310();
        virtual void func311();
        virtual void func312();
        virtual void func313();
        virtual void func314();
        virtual void func315();
        virtual void func316();
        virtual void func317();
        virtual void func318();
        virtual void func319();
        virtual void func320();
        virtual void func321();
        virtual void func322();
        virtual void func323();
        virtual void func324();
        virtual void func325();
        virtual void func326();
        virtual void func327();
        virtual void func328();
        virtual void func329();
        virtual void func330();
        virtual void func331();
        virtual void func332();
        virtual void func333();
        virtual void func334();
        virtual void func335();
        virtual void func336();
        virtual void func337();
        virtual void func338();
        virtual void func339();
        virtual void func340();
        virtual void func341();
        virtual void func342();
        virtual void func343();
        virtual void func344();
        virtual void func345();
        virtual void func346();
        virtual void func347();
        virtual bool ForceFlushRTCommands() = 0;



    public:
        char Pad000[0xbe0];
        bool m_bInitialized;
        SRenderThread* m_pRT;
        char Padbf8[0x5574];
        int m_deskwidth; //This and the one after can be a combo of multiple monitors if you have them
        int m_deskheight;
        int m_deskbpp;
        int m_nHDRType;
        int m_prefMonX;
        int m_prefMonY;
        int m_prefMonWidth; //This and the one after indicate the maximum game res
        int m_prefMonHeight;
        char Pad618c[0x1c];
        ICVar* m_CVWidth; //This corresponds to "r_width"
        ICVar* m_CVHeight; //This corresponds ot "r_height"
        ICVar* m_CVFullScreen;
        ICVar* m_CVColorBits;
        ICVar* m_CVDisplayInfo;
        ICVar* m_CVMultiMonitor;
        char Pad61d8[0x1fc];
        float m_fogCullDistance;
        char Pad63d8[0x80];
        SViewport m_MainRTViewport;
        SViewport m_MainViewport;
        SViewport m_CurViewport;
        SViewport m_NewViewport;
        int m_bViewportDirty;
        int m_nCurVPStackLevel;
        SViewport m_VPStack[0x8];
        int m_maxFrameLatency;
        int m_width;
        int m_height;
        int m_cbpp;
        int m_zbpp;
        int m_sbpp;
        int m_wireframe_mode;
        int m_wireframe_mode_prev;
        float m_drawNearFov;
        float m_pixelAspectRatio;
        char Pad65a8[0x330];
        unsigned int m_bPauseTimer;
        char Pad68dc[0xcb0];
        bool m_bCopyScreenToBackBuffer;
        bool m_bSwapBuffers;
        char Pad7590[0x10];
        char m_WinTitle[0x50];
        HINSTANCE__* m_hInst;
        HWND__* m_hWnd;
        HWND__* m_hWndDesktop;
        HWND__* m_hWnd2;
        char Pad7610[0x218];
        ID3D11RenderTargetView* m_pBackBuffer;
        ID3D11RenderTargetView* m_pSecondBackBuffer;
        ID3D11DepthStencilView* m_pZBuffer;
        IDXGISwapChain* m_pSwapChain;
        DWORD m_dwPresentStatus;
        DWORD m_dwCreateFlags;
        DWORD m_dwWindowStyle;
        char m_strDeviceStats[0x5a];
        char Pad78b0[0x688];
        ID3D11Device* m_pd3dDevice;
        ID3D11Device* m_pActuald3dDevice;
        ID3D11DeviceContext* m_pd3dDeviceContext;
        ID3D11DeviceContext* m_pActuald3dDeviceContext;
        ID3D11InputLayout* m_pLastVDeclaration;
        DXGI_SURFACE_DESC m_d3dsdBackBuffer;
        char Pad7f74[0x5c];
        SD3DSurface m_DepthBufferOrig; 
        SD3DSurface m_DepthBufferOrigMSAA; 
        char Pad8010[0x1c60]; //Size = 9c70

    public:
        //DX11 hooks come from these three functions
        static ID3D11Device* GetDevice()
        {
            typedef ID3D11Device* (__fastcall* tGetDevice)();

            tGetDevice oGetDevice = (tGetDevice)((__int64)g_gameHandle + 0x159F520);
            return oGetDevice();
        }

        static ID3D11DeviceContext* GetContext()
        {
            typedef ID3D11DeviceContext* (__fastcall* tGetContext)();

            tGetContext oGetContext = (tGetContext)((__int64)g_gameHandle + 0x159F5A0);
            return oGetContext();
        }

        static IDXGISwapChain* GetSwapChain()
        {
            typedef IDXGISwapChain* (__fastcall* tGetSwapChain)();

            tGetSwapChain oGetSwapChain = (tGetSwapChain)((__int64)g_gameHandle + 0x159F620);
            return oGetSwapChain();
        }

        //I thought the following functions might be helpful when I was trying to get hotsampling working, but they weren't necessary
        static void DXUTResizeDXGIBuffers(unsigned int Width, unsigned int Height, int bFullScreen) {
            typedef void(__fastcall* tDXUTResizeDXGIBuffers)(unsigned int Width, unsigned int Height, int bFullScreen);

            tDXUTResizeDXGIBuffers oDXUTResizeDXGIBuffers = (tDXUTResizeDXGIBuffers)((__int64)g_gameHandle + 0x15A8080);

            return oDXUTResizeDXGIBuffers(Width, Height, bFullScreen);
        }

        static void RC_SetViewport(SRenderThread* pThis, int x, int y, int width, int height) {
            typedef void(__fastcall* tRC_SetViewport)(SRenderThread* pThis, int x, int y, int width, int height);

            tRC_SetViewport oRC_SetViewport = (tRC_SetViewport)((__int64)g_gameHandle + 0x14CB840);

            return oRC_SetViewport(pThis, x, y, width, height);
        }

        static DXUTDeviceSettings* DXUTGetCurrentDeviceSettings() {
            typedef DXUTDeviceSettings*(__fastcall* tDXUTGetCurrentDeviceSettings)();

            tDXUTGetCurrentDeviceSettings oDXUTGetCurrentDeviceSettings = (tDXUTGetCurrentDeviceSettings)((__int64)g_gameHandle + 0x15A9D10);

            return oDXUTGetCurrentDeviceSettings();
        }

        static __int64 DXUTSnapDeviceSettingsToEnumDevice(DXUTDeviceSettings* pDeviceSettings, bool forceEnum, D3D_FEATURE_LEVEL forceFL) {
            typedef __int64(__fastcall* tDXUTSnapDeviceSettingsToEnumDevice)(DXUTDeviceSettings* pDeviceSettings, bool forceEnum, D3D_FEATURE_LEVEL forceFL);

            tDXUTSnapDeviceSettingsToEnumDevice oDXUTSnapDeviceSettingsToEnumDevice = (tDXUTSnapDeviceSettingsToEnumDevice)((__int64)g_gameHandle + 0x15AA360);

            return oDXUTSnapDeviceSettingsToEnumDevice(pDeviceSettings, forceEnum, forceFL);

        }

        static __int64 AdjustWindowForChange(CD3D9Renderer* pThis) {
            typedef __int64(__fastcall* tAdjustWindowForChange)(CD3D9Renderer* pThis);

            tAdjustWindowForChange oAdjustWindowForChange = (tAdjustWindowForChange)((__int64)g_gameHandle + 0x1589A90);

            return oAdjustWindowForChange(pThis);
        }

        static bool CreateMSAADepthBuffer(CD3D9Renderer* pThis) {
            typedef bool(__fastcall* tCreateMSAADepthBuffer)(CD3D9Renderer* pThis);

            tCreateMSAADepthBuffer oCreateMSAADepthBuffer = (tCreateMSAADepthBuffer)((__int64)g_gameHandle + 0x1588F00);

            return oCreateMSAADepthBuffer(pThis);
        }

        static void PostDeviceReset(CD3D9Renderer* pThis) {
            typedef void(__fastcall* tPostDeviceReset)(CD3D9Renderer* pThis);

            tPostDeviceReset oPostDeviceReset = (tPostDeviceReset)((__int64)g_gameHandle + 0x1589A10);

            return oPostDeviceReset(pThis);
        }

        static ID3D11RenderTargetView* DXUTGetD3D11RenderTargetView() {
            typedef ID3D11RenderTargetView* (__fastcall* tDXUTGetD3D11RenderTargetView)();

            tDXUTGetD3D11RenderTargetView oDXUTGetD3D11RenderTargetView = (tDXUTGetD3D11RenderTargetView)((__int64)g_gameHandle + 0x159F6A0);

            return oDXUTGetD3D11RenderTargetView();
        }

        static HRESULT OnD3D10PostCreateDevice(ID3D11Device* pd3dDevice) {
            typedef HRESULT(__fastcall* tOnD3D10PostCreateDevice)(ID3D11Device* pd3dDevice);

            tOnD3D10PostCreateDevice oOnD3D10PostCreateDevice = (tOnD3D10PostCreateDevice)((__int64)g_gameHandle + 0x158F290);

            return oOnD3D10PostCreateDevice(pd3dDevice);
        }

    };

    class CEffectParam {
    public:
        virtual void func000();
        virtual void Release() = 0;
        virtual void SetParam(float, unsigned int) = 0;
        virtual void SetParamBool(unsigned int) = 0;
        virtual void SetParamInt(int) = 0;
        virtual void SetParamVec4(DirectX::XMVECTOR, unsigned int) = 0;
        virtual void SetParamString(char const*) = 0;
        virtual void ResetParam(float) = 0;
        virtual void ResetParamVec4(DirectX::XMVECTOR) = 0;
        virtual float GetParam() = 0;
        virtual bool GetParamBool() = 0;
        virtual int GetParamInt() = 0;
        virtual DirectX::XMVECTOR GetParamVec4() = 0;
        virtual char const* GetParamString() = 0;
        virtual void SyncMainWithRender() = 0;
    };

    class CRendElementBase {

    };

    struct SShaderItem {

    };

    class CRenderObject {

    };

    //I started to try to get the Depth of Field, despite the fact that the game creates this object and then
    //never uses it. I haven't gotten around to figuring it out yet. But I think the CEffectParam vtable could be useful.
    class CDepthOfField : public CEffectParam {
    public:
        virtual void func000();
        virtual int Initialize() = 0;
        virtual int CreateResources() = 0;
        virtual void Release() = 0;
        virtual bool Preprocess() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void Reset(bool) = 0;
        virtual void OnLostDevice() = 0;
        virtual void AddRE(CRendElementBase*, SShaderItem*, CRenderObject*) = 0;
        virtual void OnBeginFrame() = 0;
        virtual char const* GetName() = 0;
        virtual bool IsActive() = 0;
        virtual void func001();

    public:
        bool m_nRenderFlags;
        bool m_nID;
        CEffectParam* m_pActive;
        CEffectParam* m_pUseMask;
        CEffectParam* m_pBokehMaskTex;
        CEffectParam* m_pFocusDistance;
        CEffectParam* m_pFocusRange;
        CEffectParam* m_pMaxCoC;
        CEffectParam* m_pCenterWeight;
        CEffectParam* m_pBlurAmount;
        CEffectParam* m_pFocusMin;
        CEffectParam* m_pFocusMax;
        CEffectParam* m_pFocusLimit;
        CEffectParam* m_pMaskTex;
        CEffectParam* m_pDebug;
        CEffectParam* m_pUserActive;
        CEffectParam* m_pUserFocusDistance;
        CEffectParam* m_pUserFocusRange;
        CEffectParam* m_pUserBlurAmount;
        CEffectParam* m_pTimeOfDayFocusRange;
        CEffectParam* m_pTimeOfDayBlurAmount;
        CEffectParam* m_pFocusMinZ;
        CEffectParam* m_pFocusMinZScale;
        CEffectParam* m_pMaskedBlurAmount;
        CEffectParam* m_pMaskedBlurMaskTex;
        float m_fUserFocusRangeCurr;
        float m_fUserFocusDistanceCurr;
        float m_fUserBlurAmountCurr;
        char Pad000[0x28];

    };

    class CRenderer {
    public:
        char Pad000[0x10];
        DirectX::XMMATRIX m_IdentityMatrix;
        DirectX::XMMATRIX m_ViewMatrix;
        DirectX::XMMATRIX m_CameraMatrix;
        DirectX::XMMATRIX m_CameraZeroMatrix[0x2];
        DirectX::XMMATRIX m_ProjMatrix;
        DirectX::XMMATRIX m_TranspOrigCameraProjMatrix;
        DirectX::XMMATRIX m_CameraProjMatrix;
        DirectX::XMMATRIX m_CameraProjZeroMatrix;
        DirectX::XMMATRIX m_InvCameraProjMatrix;
        DirectX::XMMATRIX m_TempMatrices[0x4][0x8];
        DirectX::XMMATRIX m_CameraMatrixPrev;
        DirectX::XMMATRIX m_CameraProjMatrixPrev;
        DirectX::XMMATRIX m_CameraProjMatrixPrevAvg;
        DirectX::XMMATRIX m_CameraMatrixNearest;
        DirectX::XMMATRIX m_CameraMatrixNearestPrev;
        char PadBD0[0x18];
        bool m_bInitialized;
        char PadBF0[0x59b0];
        float m_drawNearFov;
        char Pad65a4[0x334];
        bool m_bPauseTimer;
        float m_fPrevTime;
        char Pad68e0[0xcac];
        bool m_bCopyScreenToBackBuffer;
        bool m_bSwapBuffers;
        char Pad7590[0x10]; //Size = 75a0

    };

    class CCamera {
    public:
        DirectX::XMFLOAT3X4 m_Matrix;
        float m_fov;
        int m_Width;
        int m_Height;
        float m_ProjectionRatio;
        float m_PixelAspectRatio;
        DirectX::XMFLOAT3 m_edge_nlt;
        DirectX::XMFLOAT3 m_edge_plt;
        DirectX::XMFLOAT3 m_edge_flt;
        float m_asymL;
        float m_asymR;
        float m_asymB;
        float m_asymT;
        DirectX::XMFLOAT3 m_cltp;
        DirectX::XMFLOAT3 m_crtp;
        DirectX::XMFLOAT3 m_clbp;
        DirectX::XMFLOAT3 m_crbp;
        DirectX::XMFLOAT3 m_cltn;
        DirectX::XMFLOAT3 m_crtn;
        DirectX::XMFLOAT3 m_clbn;
        DirectX::XMFLOAT3 m_crbn;
        DirectX::XMFLOAT3 m_cltf;
        DirectX::XMFLOAT3 m_crtf;
        DirectX::XMFLOAT3 m_clbf;
        DirectX::XMFLOAT3 m_crbf;
        DirectX::SimpleMath::Plane m_fp[0x6]; //A lot of these things are "translations" from CryEngine types that are just typedefs of various C++ stuff
        unsigned int m_idx1[0x6];
        unsigned int m_idy1[0x6];
        unsigned int m_idz1[0x6];
        unsigned int m_idx2[0x6];
        unsigned int m_idy2[0x6];
        unsigned int m_idz2[0x6];
        float m_zrangeMin;
        float m_zrangeMax;
        int m_nPosX;
        int m_nPosY;
        int m_nSizeX;
        int m_nSizeY;
        char Pad210[0x28]; //Size = 0x238

    public:
        static void UpdateFrustum(CCamera* pThis) {

            typedef void(__fastcall* tUpdateFrustum) (CryLich::CCamera* pThis);
            tUpdateFrustum oUpdateFrustum = (tUpdateFrustum)((__int64)g_gameHandle + 0xFD540);


            oUpdateFrustum(pThis);

        }

        static void SetFrustumSmartFOV(CryLich::CCamera* pThis, int fovmaintain, int nWidth, int nHeight, float FOV, float fFovAspectRatio, float nearplane, float farpane, float fPixelAspectRatio) {
            typedef void(__fastcall* tSetFrustumSmartFOV)(CryLich::CCamera* pThis, int fovmaintain, int nWidth, int nHeight, float FOV, float fFovAspectRatio, float nearplane, float farpane, float fPixelAspectRatio);
            tSetFrustumSmartFOV oSetFrustumSmartFOV = (tSetFrustumSmartFOV)((__int64)g_gameHandle + 0x2DE7D0);

            oSetFrustumSmartFOV(pThis, fovmaintain, nWidth, nHeight, FOV, fFovAspectRatio, nearplane, farpane, fPixelAspectRatio);
        }


    };

    class CTimer {
    public:
        char Pad000[0x8];
        CTimeValue m_CurrTime[0x3];
        __int64 m_lTicksPerSec;
        double m_fSecsPerTick;
        __int64 m_lRealFrameStartTime;
        __int64 m_lBaseTime;
        __int64 m_lLastTime;
        __int64 m_lOffsetTime;
        __int64 m_lCurrentTime;
        float m_fFrameTime;
        float m_fRealFrameTime;
        __int64 m_lForcedGameTime;
        unsigned int m_bEnabled;
        float m_arrFrameTimes[0x64];
        float m_fAverageFrameTime;
        int m_timecount;
        float m_fProfileBlend;
        float m_fAvgFrameTime;
        float m_fixed_time_step;
        float m_max_time_step;
        float m_time_scale;
        int m_TimeSmoothing;
        int m_TimeDebug;
        float m_fCurrentTimeSmoothingFactor;
        float m_profile_smooth_time;
        int m_profile_weighting;
        ISystem* m_pSystem;
        unsigned int m_nFrameCounter;
        unsigned int m_bGameTimerPaused;
        int m_lGameTimerPausedTime; //size 0x248

    };



    class CSystem {
    public:
        char Pad000[0x10];
        CTimer* m_Time;
        CCamera m_ViewCamera;
        char Pad250[0x4e0];
        ICVar* m_rWidth;
        ICVar* m_rHeight;
        char Pad740[0x18];
        ICVar* m_rFullscreen;
        ICVar* m_rFullscreenWindow;
        char Pad768[0x1ebc];
        bool m_bPaused;
        char Pad2625[0x103]; //size 0x2728

    };


    class CXConsole {
    public:
        char Pad000[0x68];
        bool m_bStaticBackground;
        int m_nLoadingBackTexID;
        int m_nWhiteTexID;
        int m_nProgress;
        int m_nProgressRange;
        char Pad07c[0x20];
        int m_nTabCount;
        char Pad09c[0x108];
        bool m_bDrawCursor;
        char Pad1a8[0x4];
        bool m_bConsoleActive;
        bool m_bActivationKeyEnable;
        unsigned __int64 m_nCheatHashRangeFirst;
        unsigned __int64 m_nCheatHashRangeLast;
        bool m_bCheatHashDirty;
        int m_nCheatHash;
        CSystem* m_pSystem;
        char Pad1d0[0x8];
        IRenderer* m_pRenderer;
        char Pad1e0[0x18];
        ICVar* m_pSysDeactivateConsole;
        bool m_bUsingRemoteConsole;
        bool m_bConsoleIsEmerging;
        char Pad202[0x4]; // size 0x208

        //This was my first attempt with the CVars, cannot remember why I discarded it
            static ICVar* GetCVar(CXConsole* pThis, const char* sName) {

                typedef ICVar* (__fastcall* tGetCVar)(CryLich::CXConsole* pThis, const char* sName);
                tGetCVar oGetCVar = (tGetCVar)((__int64)g_gameHandle + 0xECBA60);

                return oGetCVar(pThis, sName);
        }

    };




    struct IFunctionHandler {

    };

    class CScriptBind_System {
    public:
        char Pad000[0x8];
        char m_sGlobalName[0x40];
        char Pad048[0x10];
        int m_nParamIdOffset;
        ISystem* m_pSystem;
        ILog* m_pLog;
        IRenderer* m_pRenderer;
        IConsole* m_pConsole;
        ITimer* m_pTimer;
        I3DEngine* m_p3DEngine;
        float m_SkyFadeStart;
        float m_SkyFadeEnd;
        char Pad098[0x10]; // size 0xa8;

        //Another attempt at the CVars, before I really figured out how to use the vtables
        static int SetCVar(CScriptBind_System* pThis, IFunctionHandler* pH) {

            typedef int(__fastcall* tSetCVar)(CryLich::CScriptBind_System* pThis, IFunctionHandler* pH);
            tSetCVar oSetCVar = (tSetCVar)((__int64)g_gameHandle + 0xDDD100);

            return oSetCVar(pThis, pH);
        }
    };


    enum EShadowMode {

    };

    class CTimeOfDay {

    };

    class CGlobalIlluminationManager {

    };

    struct CLightEntity {

    };

    class C3DEngine {
    public:
        char Pad000[0x8];
        int m_nFlags;
        bool m_bAreaActivationInUse;
        float m_fSkyBoxAngle;
        float m_fSkyBoxStretching;
        float m_fMaxViewDistScale;
        float m_fMaxViewDistHighSpec;
        float m_fMaxViewDistLowSpec;
        float m_fTerrainDetailMaterialsViewDistRatio;
        float m_volFogGlobalDensity;
        float m_volFogGlobalDensityMultiplierLDR;
        float m_volFogFinalDensityClamp;
        float m_fCloudShadingSunLightMultiplier;
        float m_fCloudShadingSkyLightMultiplier;
        float m_vCloudShadingCustomSunColor[3];
        float m_vCloudShadingCustomSkyColor[3];
        float m_vFogColor[3];
        float m_vDefFogColor[3];
        float m_vSunDir[3];
        float m_vSunDirNormalized[3];
        float m_fSunDirUpdateTime;
        float m_vSunDirRealtime[3];
        float m_vWindSpeed[3];
        float m_volFogRamp[3];
        float m_volFogShadowRange[3];
        float m_volFogShadowDarkening[3];
        float m_volFogShadowEnable[3];
        float m_nightSkyHorizonCol[3];
        float m_nightSkyZenithCol[3];
        float m_nightSkyZenithColShift;
        float m_nightSkyStarIntensity;
        float m_nightMoonCol[3];
        float m_nightMoonSize;
        float m_nightMoonInnerCoronaCol[3];
        float m_nightMoonInnerCoronaScale;
        float m_nightMoonOuterCoronaCol[3];
        float m_nightMoonOuterCoronaScale;
        float m_sunRotationZ;
        float m_sunRotationLongitude;
        float m_moonRotationLatitude;
        float m_moonRotationLongitude;
        float m_moonDirection[3];
        float m_fSkyDepth;
        int m_nWaterBottomTexId;
        int m_nNightMoonTexId;
        bool m_bShowTerrainSurface;
        bool m_bSunShadows;
        bool m_bGsmExtendLastLod;
        int m_unknown;
        int m_unkonwn2; //Something went weird here with the sizees, I have fudged it a little to make some things work
        int m_nCloudShadowTexId;
        float m_vCloudShadowAnimParams[3];
        float m_vCloudShadowTileParams[3];
        float m_fGsmRange;
        float m_fGsmRangeStep;
        float m_fShadowsConstBias;
        float m_fShadowsSlopeBias;
        int m_nGsmLodsNum;
        int m_nGsmCache;
        float m_oceanFogColor[3];
        float m_oceanFogColorShallow[3];
        float m_oceanFogDensity;
        float m_oceanFogColorMultiplier;
        float m_skyboxMultiplier;
        float m_dayNightIndicator;
        float m_fogColor2[3];
        float m_fogColorRadial[3];
        float m_volFogHeightDensity[3];
        float m_volFogHeightDensity2[3];
        float m_volFogGradientCtrl[3];
        int m_bHeightBasedAmbientAffectsGlobalCube;
        float m_oceanCausticsDistanceAtten;
        float m_oceanCausticsMultiplier;
        float m_oceanCausticsDarkeningMultiplier;
        float m_oceanCausticsTilling;
        float m_oceanCausticHeight;
        float m_oceanCausticDepth;
        float m_oceanCausticIntensity;
        char m_skyMatName[0x2];
        char m_skyLowSpecMatName[0x8];
        float m_oceanWindDirection;
        float m_oceanWindSpeed;
        float m_oceanWavesSpeed;
        float m_oceanWavesAmount;
        float m_oceanWavesSize;
        float m_dawnStart;
        float m_dawnEnd;
        float m_duskStart;
        float m_duskEnd;
        float m_hdrBlueShift[3];
        float m_hdrBlueShiftThreshold;
        float m_hdrBloomColor[3];
        float m_hdrBloomMul;
        float m_hdrFlaresColor[3];
        float m_hdrFlaresMul;
        float m_hdrStreaksColor[3];
        float m_hdrStreaksMul;
        float m_hdrStreakScale;
        float m_fHDRShoulderScale;
        float m_fHDRMidtonesScale;
        float m_fHDRToeScale;
        float m_fHDRWhitePoint;
        float m_vColorBalance[3];
        float m_fHDRContrast;
        float m_fHDRSaturation;
        float m_pShadowCascadeConstBias[0x14];
        float m_pShadowCascadeSlopeBias[0x14];
        float m_fSaturation;
        float m_pPhotoFilterColor[3];
        float m_fPhotoFilterColorDensity;
        float m_fGrainAmount;
        float m_fSunSpecMult;
        char Pad364[0xc0];
        float m_fHDRDynamicMultiplier;
        char Pad424[0x308];
        bool m_bOcean;
        char Pad72d;
        float m_vSkyHightlightPos[3];
        float m_vSkyHightlightCol[3];
        float m_fSkyHighlightSize;
        float m_vAmbGroundCol[3];
        float m_fAmbMaxHeight;
        float m_fAmbMinHeight;
        char Pad760[0x28];
        EShadowMode m_eShadowMode;
        bool m_bLayersActivated;
        bool m_bContentPrecacheRequested;
        bool m_bTerrainTextureStreamingInProgress;
        bool m_bHasDirtyDynLights;
        bool m_bSegmentedWorldActive;
        char Pad798[0x98];
        CTimeOfDay* m_pTimeOfDay;
        ICVar* m_pLightQuality;
        CGlobalIlluminationManager* m_pGlobalIlluminationManager;
        char Pad848[0x150];
        CLightEntity* m_pSun;
        char Pad9a0[0x2f0];
        bool m_bIsTessellationAllowedForShadowMap;
        char Padc91[0x1f]; //size 0xcb0

        //I thought this might be necessary for the sun direction, but it wasn't
        static void SetSunDir(C3DEngine* pThis, std::vector<float>* newSunDir) {

            typedef void(__fastcall* tSetSunDir)(C3DEngine* pThis, std::vector<float>* newSunDir);
            tSetSunDir oSetSunDir = (tSetSunDir)((__int64)g_gameHandle + 0x125B00);

            return oSetSunDir(pThis, newSunDir);

        }

    };

    struct SCamModeSettings {

    };

    struct SSpherical {
        float m_fYaw;
        float m_fPitch;
        float m_fDist;
    };

    class CYawPitchDampState {
    public:
        SSpherical spVel;
        SSpherical spNewPrev;
        SSpherical spPrevPrev;
        int nNewWrap;
        int nYawWrap;
        bool bNeedReset;
    };

    struct SCamRetainedState {
        int camIdLast;
        float cutHoldValue;
        float cutHoldDist;
        bool bNeedReset;
        float driverP4HoldValue;
        float driverP6HoldValue;
        class CYawPitchDampState dampStateP3;

    };
   
    class CCameraOverrides {
    public:
        int m_camOverrideType;
        SCamModeSettings m_overrideSettings;
        float m_fFadeOutCoverMode;
        float m_fLookAtHOff;
        float m_fLookAtHOffTarget;
        int m_trackedEntityId;
        float m_fTrackingDistance; //size 0x98
    };

    class CCameraInputHelper;
    class CCameraRayScan;
    class CCameraTracking;

    struct SViewParams {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 rotation;
        DirectX::XMFLOAT4 localRotationLast;
        float nearplane;
        float fov;
        char viewID;
        char groundOnly;
        char Pad036[0x2];
        float shakingRatio;
        DirectX::XMFLOAT4 currentShakeQuat;
        DirectX::XMFLOAT3 currentShakeShift;
        int idTarget;
        DirectX::XMFLOAT3 targetPos;
        float frameTime;
        float angleVel;
        float vel;
        float dist;
        int blend;
        float blendPosSpeed;
        float blendRotSpeed;
        float blendFOVSpeed;
        DirectX::XMFLOAT3 blendPosOffset;
        DirectX::XMFLOAT4 blendRotOffset;
        float blendFOVOffset;
        char justActivated;
        unsigned int viewIDLast;
        //char Pad00aa[0x2];
        DirectX::XMFLOAT3 positionLast;
        DirectX::XMFLOAT4 rotationLast;
        float FOVLast; // size 0xcc

    };

    class CCameraPolar {

    };

    class CCameraView {
    public:
        struct IEntity* m_pTarget;
        DirectX::XMFLOAT3 m_vTargetPosition;
        DirectX::XMFLOAT3 m_vTargetOffset;
        class CCameraInputHelper* m_pCamHelper;
        class CCameraRayScan* m_pCamRayScan;
        class CCameraTracking* m_camTracking;
        SCamModeSettings m_curSettings;
        float m_fFrameTime;
        bool m_bModeTransition;
        float m_fTransitionTimeout;
        CCameraPolar m_curPolar;
        SViewParams m_lastViewParams;

    };


    class CCameraManager {
    public:
        char Pad000[0x18];
        struct SCamRetainedState m_camState;
        int m_idActive;
        int m_idPrev;
        class CCameraView* m_pCameraView;
        class CCameraOverrides* m_pCamOverrides; //size 0x78

    };

    class CView {
    public:
        char Pad000[0x8];
        bool m_active;
        int m_linkedTo;
        SViewParams m_viewParams;
        class CCamera m_camera;
        char Pad318[0x30]; //size 0x348


    };

    class CryStringT {
    public:
        char* m_str;
    };

    class CDXInput {
    public:
        char Pad000[0x80];
        bool m_hasFocus;
        char Pad081[0x37];
        int m_modifiers;
        char Pad0c0[0x20];
        HWND__* m_hwnd;
        IDirectInput8A* m_pDI;
        char Pad0f0[0x8]; //size 0xf8
    };

    struct SInputEvent {

    };

    class CKeyboard {
    public:
        char Pad000[0x8];
        enum EDeviceId m_deviceId;
        bool m_enabled;
        struct IInput* m_input;
        class CryStringT m_deviceName;
        char Pad020[0x40];
        class CDXInput* m_dxInput;
        IDirectInputDevice8A* m_pDevice;
        char Pad070[0x18]; // size 0x88

    };

    class CMouse {
    public:
        char Pad000[0x8];
        enum EDeviceId m_deviceId;
        bool m_enabled;
        struct IInput* m_input;
        class CryStringT m_deviceName;
        char Pad020[0x40];
        class CDXInput* m_dxInput;
        IDirectInputDevice8A* m_pDevice;
        char Pad070[0x30];
        float m_mouseWheel;
        char Pad0a4[0x4]; // size 0xa4
    };

    struct _XINPUT_GAMEPAD {
        unsigned int wButtons;
        bool bLeftTrigger;
        bool bRightTrigger;
        int sThumbLX;
        int sThumbLY;
        int sThumbRX;
        int sThumbRY;
    };

    struct _XINPUT_STATE {
        int dwPacketNumber;
        _XINPUT_GAMEPAD Gamepad;
    };

    class CXInputDevice {
    public:
        char Pad000[0x8];
        enum EDeviceId m_deviceId;
        bool m_enabled;
        struct IInput* m_input;
        class CryStringT m_deviceName;
        char Pad020[0x40];
        int m_deviceNo;
        bool m_connected;
        char Pad065[0x3];
        _XINPUT_STATE m_state;
        float m_basicLeftMotorRumble;
        float m_basicRightMotorRumble;
        float m_frameLeftMotorRumble;
        float m_frameRightMotorRumble;
        float m_fVibrationTimer;
        float m_fDeadZone;
    };

    class CHardwareMouse {
    public:
        virtual void func001();
        virtual void func002();
        virtual void func003();
        virtual void func004();
        virtual void func005();
        virtual void func006();
        virtual void func007();
        virtual void func008();
        virtual void func009();
        virtual void func010();
        virtual void func011();
        virtual void func012();
        virtual void func013();
        virtual void func014();
        virtual void GetHardwareMousePosition(float*, float*) = 0;
        virtual void SetHardwareMousePosition(float, float) = 0;
        virtual void GetHardwareMouseClientPosition(float*, float*) = 0;
        virtual void SetHardwareMouseClientPosition(float, float) = 0;
        virtual void Reset(bool) = 0;
        virtual void ConfineCursor(bool) = 0;
        virtual void Hide(bool) = 0;
        virtual void UseSystemCursor(bool) = 0;
        virtual void Render() = 0;
        
    public:
        char Pad008[0x28];
        int m_iReferenceCounter;
        float m_fCursorX;
        float m_fCursorY;
        float m_fIncX;
        float m_fIncY;
        bool m_bFocus;
        bool m_recapture;
        bool const m_allowConfine;
        bool m_unhidByEscape;
        bool m_useSystemCursor;
        bool m_hide;
        bool m_calledShowHWMouse;
        bool m_unknown;
        int m_debugHardwareMouse; //I was hoping somewhere in here lay the answer to the disappearing mouse cursor, but not so far

    };

    class CGame {
    public:
        char Pad000[0x30];
        IConsole* m_pConsole;
        char Pad038[0xb8];
        unsigned int m_inDevMode; //This also can turn on the Dev Mode, as I recall
        unsigned int m_clientActorId;
        SCVars* m_pCVars;
        char Pad100[0x68];
        CCameraManager* m_pCameraManager;
        char Pad170[0x128]; //size 0x298
    };

    struct SSystemCVars {
        char Pad000[0x80];
        struct ICVar* xav_activateCheats;
        char Pad088[0x78]; //size 0x100
    };

    class CFlashUI {
    public:
        char Pad000[0x1a4];
        bool m_bHudVisible;
        char Pad1a5[0x1b]; //size 0x1c0

        static bool IsHUDVisible(CFlashUI* pThis) {
            typedef bool(__fastcall* tToggleHUD)(CFlashUI* pThis);
            tToggleHUD oToggleHUD = (tToggleHUD)((__int64)g_gameHandle + 0x2302F0);

            return pThis->IsHUDVisible;

        }
    };

    struct INetwork {

    };

    struct IScriptSystem {

    };

    struct SEntitySpawnParams {

    };

    struct IEntitySystem {
        virtual void func00();
        virtual void func01();
        virtual void func02();
        virtual void func03();
        virtual void func04();
        virtual void func05();
        virtual void func06();
        virtual void func07();
        //Maybe these are useful, who knows
        virtual IEntity* *SpawnEntity(IEntitySystem* pThis, SEntitySpawnParams* params, bool spawn);
        virtual bool* InitEntity(IEntitySystem* pThis, IEntity* entity, SEntitySpawnParams* params);
        virtual IEntity* *GetEntity(IEntitySystem* const pThis, unsigned int entity);
        virtual IEntity** FindEntityByName(IEntitySystem* const pThis, char const* sName);
        virtual void func08();
        virtual void func09();
        virtual void func10();
        virtual void func11();
        virtual void func12();
        virtual void func13();
        virtual void func14();
        virtual void func15();
        virtual void func16();
        virtual void func17();
        virtual void func18();
        virtual void PauseTimers(bool, bool) = 0; //This also led to full menu pause

    };

    class CCryActionCVars {

    };

    class CCryAction {
    public:
        char Pad000[0x8];
        bool m_paused;
        bool m_forcedpause;
        bool m_levelPrecachingDone;
        bool m_usingLevelHeap;
        ISystem* m_pSystem;
        INetwork* m_pNetwork;
        I3DEngine* m_p3DEngine;
        IScriptSystem* m_pScriptSystem;
        IEntitySystem* m_pEntitySystem;
        ITimer* m_pTimer;
        ILog* m_pLog;
        char Pad048[0x6a8];
        CCryActionCVars* m_pCryActionCVars;
        char Pad6f8[0x30];
        ICVar* m_pEnableLoadingScreen;
        ICVar* m_pCheats;
        char Pad738[0x90]; //size 0x7c8

        //Another full menu pause
        static void PauseGame(CCryAction* pThis, bool pause, bool force, unsigned int nFadeOutInMS) {
            typedef void(__fastcall* tPauseGame)(CCryAction* pThis, bool pause, bool force, unsigned int nFadeOutInMS);
            tPauseGame oPauseGame = (tPauseGame)((__int64)g_gameHandle + 0x5DDEE0);

            oPauseGame(pThis, pause, force, nFadeOutInMS);
        }
    };

    struct IGame {

    };

    struct IGameFramework {

    };

    struct IInput {

    };

    struct IPhysicalWorld {

    };

    struct IMaterialEffects {

    };

    struct IParticleManager {

    };
  
    struct IScaleformGFx {

    };

    struct IFlashUI {

    };

    class SSystemGlobalEnvironment {
    public:
        ISystem* pSystem;
        IGame* pGame;
        IGameFramework* pGameFramework;
        INetwork* pNetwork;
        IRenderer* pRenderer;
        IInput* pInput;
        ITimer* pTimer;
        IConsole* pConsole;
        char Pad040[0x10];
        I3DEngine* p3DEngine;
        char Pad058[0x10];
        IPhysicalWorld* pPhysicalWorld;
        char Pad070[0x10];
        IEntitySystem* pEntitySystem;
        char Pad088[0x70];
        IMaterialEffects* pMaterialEffects;
        IParticleManager* pParticleManager;
        char Pad108[0x10];
        IScaleformGFx* pScaleformGFx;
        IFlashUI* pFlashUI;
        char Pad128[0x130]; //size 0x258


    };



}