#pragma once

#include "header.h"
#include "CArray.h"
#include "LogFile.h"
#include "AreaTex.h"
#include "SearchTex.h"
#include "BufferUAV.h"
#include "BufferRead.h"
#include "BufferSRV_UAV.h"
#include "BufferConstant.h"
#include "TextureSRV_UAV.h"
#include "TextureRTAndSRV.h"
#include "ArrayBufferStruct.h"
#include "TextureRT_SRV_UAV.h"

#define NUM_DEFFERED_CONTEXT 2

class CSystem
{
	friend class CRedKid;
	friend class CDXInput;
	friend class CRenderPool;

	struct sDefferedRenderData
	{
		D3DXMATRIX mVPInv;
		float Width;
		float Height;
		UINT LightPointCount;
		UINT LightDirectionCount;
		UINT XBlockCount;
	};

	struct sHDRCalcData
	{
		UINT	SummSize;
		float	ElapsedTime;
		float	NumPoint;
	};

	// Класс для хранения настроек программы между работой
	class CAppIni
	{
	public:
		CAppIni(void)
		{
			mWidth		= 800;
			mHeight		= 600;
			fWindowed	= true;
			mNumArapter = 0;
			mNumMonitor	= 0;
		};
		inline void Reset( void )
		{
			mWidth		= 800;
			mHeight		= 600;
			fWindowed	= true;
			mNumArapter = 0;
			mNumMonitor	= 0;
		};
		inline HRESULT Load( void )
		{
			std::ifstream IniFile;
			try
			{
				LogFile.OutputLogFileString( L"Load inicialiseApp data: START." );
				IniFile.open( "ini.ini", ios::binary );
				if ( !IniFile )
				{
					LogFile.OutputLogFileErrorString( L"No INI file.", __FILE__, __FUNCTION__, __LINE__ );
					return S_FALSE;
				}

				IniFile.read( (char*)&mWidth, sizeof(UINT) );
				IniFile.read( (char*)&mHeight, sizeof(UINT) );
				IniFile.read( (char*)&fWindowed, sizeof(bool) );
				IniFile.read( (char*)&mNumArapter, sizeof(UINT) );
				IniFile.read( (char*)&mNumMonitor, sizeof(UINT) );

				IniFile.close();
				LogFile.OutputLogFileString( L"Load inicialiseApp data: OK." );
			}
			catch ( ... )
			{
				LogFile.OutputLogFileErrorString( L"Ошибка при чтении INI файла.", __FILE__, __FUNCTION__, __LINE__ );
				IniFile.close();
				return S_FALSE;
			}

			return S_OK;
		};
		inline HRESULT Save( void )
		{	
			std::ofstream IniFile;
			try
			{
				LogFile.OutputLogFileString( L"Save inicialiseApp data: START." );
				IniFile.open( "ini.ini", std::ios::trunc | std::ios::binary );
				if ( !IniFile )
				{
					LogFile.OutputLogFileErrorString( L"Ошибка создания INI файла.", __FILE__, __FUNCTION__, __LINE__ );
					return S_FALSE;
				}

				IniFile.write( (char*)&mWidth, sizeof( UINT ) );
				IniFile.write( (char*)&mHeight, sizeof( UINT ) );
				IniFile.write( (char*)&fWindowed, sizeof( bool ) );
				IniFile.write( (char*)&mNumArapter, sizeof( UINT ) );
				IniFile.write( (char*)&mNumMonitor, sizeof( UINT ) );

				LogFile.OutputLogFileString( L"Save inicialiseApp data: OK." );
			}
			catch( ... )
			{
				LogFile.OutputLogFileErrorString( L"Ошибка при записи INI файла.", __FILE__, __FUNCTION__, __LINE__ );
				IniFile.close();
				return S_FALSE;
			}

			return S_OK;
		};

		inline UINT GetWidth( void ) { return mWidth; };
		inline void SetWidth( UINT d ) { mWidth = d; };
		inline UINT GetHeight( void ) { return mHeight; };
		inline void SetHeight( UINT d ) { mHeight = d; };
		inline bool GetWindowed( void ) { return fWindowed; };
		inline void SetWindowed( bool f ) { fWindowed = f; };
		inline UINT GetNumAdapter( void ) { return mNumArapter; };
		inline UINT GetNumMonitor( void ) { return mNumMonitor; };

	private:
		UINT mWidth;
		UINT mHeight;
		bool fWindowed;
		UINT mNumArapter;
		UINT mNumMonitor;

		CLogFile LogFile;
	};

	class CMonitorData
	{
	public:
		CMonitorData( IDXGIOutput * Monitor, UINT Index, UINT numModes, DXGI_MODE_DESC* md )
		{
			IndexMonitor = Index;
			pIMonitor = Monitor;
			NumModes = numModes;
			Modes = md;
		}
		~CMonitorData( void )
		{
			Release();
		}
		void Release( void )
		{
			RELEASE( pIMonitor );
			DEL_ARRAY( Modes );
		}
		UINT GetNumModes( void ) { return NumModes; };
		DXGI_MODE_DESC* GetModeDesc( void ) { return Modes; };
	private:
		UINT IndexMonitor;
		IDXGIOutput * pIMonitor;
		UINT NumModes;
		DXGI_MODE_DESC* Modes;
	};

	class CAdapterData
	{
	public:
		CAdapterData( IDXGIAdapter1* Adapter, UINT Index )
		{
			pIAdapter = Adapter;
			IndexAdapter = Index;
		}
		~CAdapterData( void )
		{
			Release();
		}
		inline void Release( void )
		{
			aMonitor.ReleaseLP();
			RELEASE( pIAdapter );
		}
		inline HRESULT FindMonitor( void )
		{
			UINT Index = 0;
			IDXGIOutput * pIOutput;
			CMonitorData* Monitor;
			UINT numMode;
			DXGI_MODE_DESC* md;

			while( S_OK == pIAdapter->EnumOutputs( Index, &pIOutput) )
			{
				numMode = 0;
				if ( S_OK != pIOutput->GetDisplayModeList( FORMAT_RENDER_BUFFER, DXGI_ENUM_MODES_INTERLACED, &numMode, 0 ) )
					continue;

				md = new DXGI_MODE_DESC[ numMode ];
				if ( S_OK != pIOutput->GetDisplayModeList( FORMAT_RENDER_BUFFER, DXGI_ENUM_MODES_INTERLACED, &numMode, md ) )
					continue;

				Monitor = new CMonitorData( pIOutput, Index, numMode, md );

				aMonitor.Add( Monitor );
				Index++;
			}

			if ( aMonitor.GetCount() == 0 )
				return S_FALSE;

			return S_OK;
		}
		inline UINT GetMonitorCount( void ) { return aMonitor.GetCount(); };
		inline CMonitorData* GetMonitor( UINT Index )
		{
			if ( Index > aMonitor.GetCount() )
				return NULL;

			return aMonitor[Index];
		}

		IDXGIAdapter1* GetAdapter( void ) { return pIAdapter; };
		UINT GetIndexAdapter( void ) { return IndexAdapter; };

	private:
		UINT IndexAdapter;
		IDXGIAdapter1* pIAdapter;

		CArray<CMonitorData*> aMonitor;
	};

public:
	CSystem(void);
	~CSystem(void);

	inline HINSTANCE GetHinstance( void ) { return hInst; };
	inline HWND GetHWND( void ) { return hWnd; };
	inline UINT GetWidth( void ) { return AppIni.GetWidth(); };
	inline UINT GetHeight( void ) { return AppIni.GetHeight(); };

	inline D3D_FEATURE_LEVEL GetFeatureLevel( void ) { return FeatureLevel; };
	inline ID3D11Device* GetDevice( void ) { return g_pd3dDevice; };
	inline ID3D11DeviceContext* GetDeviceContext( void ) { return g_pImmediateContext; };
	inline ID3D11DeviceContext* GetDeferredContext( UINT Index )
	{
		if ( Index < NUM_DEFFERED_CONTEXT )
			return g_pDefferedContext[Index];
		else
			return NULL;
	};
	inline IDXGISwapChain* GetSwapChain( void ) { return g_pSwapChain; };

	inline ID3DX11EffectMatrixVariable* GetViewVariable( void ) { return g_pViewVariable; };
	inline ID3DX11EffectMatrixVariable* GetProjectionVariable( void ) { return g_pProjectionVariable; };
	inline ID3DX11EffectMatrixVariable* GetViewProjVariable( void ) { return g_pViewProjVariable; };
	inline ID3DX11EffectMatrixVariable* GetWorldVariable( void ) { return g_pWorldVariable; };

	inline ID3DX11EffectShaderResourceVariable* GetDiffuseTexture( void ) { return g_DiffuseTexture; };
	inline ID3DX11EffectTechnique* GetTechLine( void ) { return	g_pTechLine; };
	inline ID3DX11EffectTechnique* GetTechTexture( void ) { return g_pTechTexture; };
	inline ID3DX11EffectTechnique* GetTechImterface( void ) { return g_pTechInterface; };
	inline ID3DX11EffectTechnique* GetTechTextureBone( void ) { return g_pTechTextureBone; };
	inline ID3D11InputLayout* GetLayoutInterface( void ) { return g_pInpLayoutInterface; };
	inline ID3D11InputLayout* GetLayoutTexture( void ) { return g_pInputLayoutTexture; };
	inline ID3D11InputLayout* GetLayoutTextureBone( void ) { return g_pInputLayoutTextureBone; };
	inline ID3D11InputLayout* GetLayoutLine( void ) { return g_pInputLayoutLine; };

	inline ID3D11ComputeShader* GetCSSceletNormalCopy( void ) { return g_pCSSceletNormalCopy; };
	inline ID3D11ComputeShader* GetCSSceletTBNCopy( void ) { return g_pCSSceletTBNCopy; };
	inline ID3D11ComputeShader* GetCSSceletNormalAnim( void ) { return g_pCSSceletNormalAnim; };
	inline ID3D11ComputeShader* GetCSSceletTBNAnim( void ) { return g_pCSSceletTBNAnim; };
	inline ID3D11ComputeShader* GetCSCopyCalcAABB( void ) { return g_pCSCopyCalcAABB; };
	inline ID3D11ComputeShader* GetCSCopyCalcAABB_Scelet( void ) { return g_pCSCopyCalcAABB_Scelet; };
	inline ID3D11ComputeShader* GetCSCalcAABB( void ) { return g_pCSCalcAABB; };

	inline CBufferConstant* GetBufferAABBData( void ) { return &g_pBufferAABBData; };
	inline CBufferUAV*		GetUAVBufferCalcAABB( void ) {	return &g_pBufferCalcAABB; };
	inline CBufferRead*		GetBufferReadAABB( void ) {	return &g_pBufferReadAABB; };

	inline bool GetFocus( void ) { return fPause; };
	inline UINT GetFps( void ) { return (UINT)mFps; };

	// PhysX
	inline PxPhysics* GetPhysX( void ) { return mPhysX; };
	inline PxCooking* GetPhysXCooking( void ) { return mCooking; };

protected:
	CAppIni AppIni;
	bool fInit;
	bool fPause;
	float mFps;
	inline bool GetfInit( void ) { return fInit; };

	HRESULT CompartibleIni( void );
	CArray<CAdapterData*> aAdapter;

	HRESULT InicialiseForEditor( HWND Wnd, PxDefaultAllocator &PhysXDefAlloc, PxErrorCallback &PhysXDefError, D3D_FEATURE_LEVEL cLev );
	HRESULT Inicialise( HINSTANCE hInstance, PxDefaultAllocator &PhysXDefAlloc, PxErrorCallback &PhysXDefError, D3D_FEATURE_LEVEL cLev );
	HRESULT Inicialise3D( D3D_FEATURE_LEVEL cLev );
	HRESULT InicialiseShader( void );
	HRESULT InicialisePhysX( PxDefaultAllocator &gAlloc, PxErrorCallback &PhysXDefError );
	HRESULT CreateComputeShader( TCHAR* ShaderFileName, LPCSTR FunctionName, ID3D11ComputeShader* &CS );
	HRESULT CreateAreaTexture( void );
	HRESULT CreateSearchTexture( void );
	HRESULT Scan3DSystem( void );
	int Run( void );
	HRESULT FrameRender( void );
	HRESULT Resize( void );
	void Release( void );

	ID3DX11Effect*							g_pEffect;
	ID3DX11EffectTechnique*					g_pTechLine;
	ID3DX11EffectTechnique*					g_pTechTexture;
	ID3DX11EffectTechnique*					g_pTechInterface;
	ID3DX11EffectTechnique*					g_pTechTextureBone;

	ID3DX11EffectTechnique*					g_pTechRenderLight;

	ID3D11InputLayout*						g_pInputLayoutLine;
	ID3D11InputLayout*						g_pInputLayoutQuad;
	ID3D11InputLayout*						g_pInputLayoutTexture;
	ID3D11InputLayout*						g_pInpLayoutInterface;
	ID3D11InputLayout*						g_pInputLayoutTextureBone;

	ID3DX11EffectTechnique*					g_pSMAATechLumaEdgeDet;
	ID3DX11EffectTechnique*					g_pSMAATechBlendWeightCalc;
	ID3DX11EffectTechnique*					g_pSMAANeighborhoodBlending;
	ID3D11Texture2D*						g_pAreaTexture;
	ID3D11Texture2D*						g_pSearchTexture;
	ID3D11ShaderResourceView*				g_pAreaTextureSRV;
	ID3D11ShaderResourceView*				g_pSearchTextureSRV;

	ID3DX11EffectMatrixVariable*			g_pWorldVariable;
	ID3DX11EffectMatrixVariable*			g_pViewVariable;
	ID3DX11EffectMatrixVariable*			g_pProjectionVariable;
	ID3DX11EffectMatrixVariable*			g_pViewProjVariable;
	ID3DX11EffectVectorVariable*			g_pPixelSize;

	ID3DX11EffectShaderResourceVariable*	g_DiffuseTexture;
	ID3DX11EffectShaderResourceVariable*	g_DepthTexture;
	ID3DX11EffectShaderResourceVariable*	g_SearchTexture;

	ID3D11ComputeShader*			        g_pCSSceletNormalCopy;
	ID3D11ComputeShader*			        g_pCSSceletTBNCopy;
	ID3D11ComputeShader*			        g_pCSSceletNormalAnim;
	ID3D11ComputeShader*			        g_pCSSceletTBNAnim;
	ID3D11ComputeShader*			        g_pCSCopyCalcAABB;
	ID3D11ComputeShader*			        g_pCSCopyCalcAABB_Scelet;
	ID3D11ComputeShader*			        g_pCSCalcAABB;
	ID3D11ComputeShader*			        g_pCSRenderLight;
	ID3D11ComputeShader*			        g_pCSCalcSummLum;
	ID3D11ComputeShader*			        g_pCSCalcAdaptedLum;
	ID3D11ComputeShader*			        g_pCSBloomGaus;

	ID3D11Buffer*							g_pVBQuad;
	CBufferConstant							c_CBRenderLight;

	// Установка и отчистка RenderTarget для Deffered Shading
	inline void OMSetDefferedRTAndClear( D3DXVECTOR4* col )
	{
		static D3DXVECTOR4 ClearColor = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f );
		static ID3D11RenderTargetView* rTV[3];

		rTV[0] = cTextDiff.GetTextureRT();
		rTV[1] = cTextGBuffer.GetTextureRT();
		rTV[2] = cTextMaterial.GetTextureRT();

		g_pImmediateContext->ClearRenderTargetView( rTV[0], *col );
		g_pImmediateContext->ClearRenderTargetView( rTV[1], ClearColor );
		g_pImmediateContext->ClearRenderTargetView( rTV[2], ClearColor );
		g_pImmediateContext->OMSetRenderTargets( 3, rTV, g_pDepthStencilView );

		g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

	};
	void DefferedRenderLight( float ElapsedTime, D3DXMATRIX* mVPInv, CArrayBufferStruct<LIGHT_POINT> *aLightPoint, CArrayBufferStruct<LIGHT_DIRECTION> *aLightDirection );
	void RenderFinalSceneQuad( void );
	inline void Present( void ) { g_pSwapChain->Present( 0, 0 ); };

	HINSTANCE hInst;
	HWND hWnd;
	static LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	D3D11_VIEWPORT mViewPort;

	IDXGIFactory1*				g_pFactory;
	ID3D11Device*				g_pd3dDevice;
	ID3D11DeviceContext*		g_pImmediateContext;
	ID3D11DeviceContext*		g_pDefferedContext[NUM_DEFFERED_CONTEXT];
	IDXGISwapChain* 			g_pSwapChain;
	ID3D11RenderTargetView*		g_pRenderScreenTargetView;
	ID3D11DepthStencilView*		g_pDepthStencilView;
	ID3D11ShaderResourceView*	g_pDepthStencilSRV;

	D3D_FEATURE_LEVEL FeatureLevel;

	CTextureRTAndSRV	cTextDiff;
	CTextureRTAndSRV	cTextGBuffer;
	CTextureRTAndSRV	cTextMaterial;
	CTextureSRV_UAV		cTextBloom;
	CTextureSRV_UAV		cTextBloomGaus;
	CTextureRT_SRV_UAV	cTextLightRender;
	CTextureRTAndSRV	cTextEdges;
	CTextureRTAndSRV	cTextWeight;

	CBufferConstant		g_pBufferAABBData;
	CBufferUAV			g_pBufferCalcAABB;
	CBufferRead			g_pBufferReadAABB;

	CBufferSRV_UAV		g_pHDRCalc;
	CBufferSRV_UAV		g_pHDRCalcLum;
	CBufferUAV			g_pHDRPrevLum;
	UINT				XScBl, YScBl;
	CBufferConstant		g_pHDRData;

	// PhysX 3.1 members
	PxPhysics*		mPhysX;
	PxCooking*		mCooking;
	//PxProfileZone*	mProfileZone;
	//SampleProfileEventNameProvider gProfileNameProvider;
};

