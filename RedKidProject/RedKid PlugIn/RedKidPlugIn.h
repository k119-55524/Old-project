#pragma once

#include "header.h"

class RedMax3DPlugIn : public SceneExport 
{
	public:
		
		virtual int				ExtCount() { return 1; };
		virtual const TCHAR *	Ext(int n)					// Extension #n (i.e. "3DS")
		{
			if ( !n )
				return _T("rm3dt");
			else
				return _T("");
			//switch ( n )
			//{
			//case 0 :
			//	return _T("rm3dt");
			//	break;
			//case 1:
			//	return _T("rm3dtAnim");
			//	break;
			//default:
			//	return _T("");
			//}
		};					
		virtual const TCHAR *	LongDesc() { return _T( "RedMax3D.com PlugIn exports." ); };
		virtual const TCHAR *	ShortDesc() { return _T( "RedMax3D.com RedKid PlugIn *.rm3dt" ); };
		virtual const TCHAR *	AuthorName() { return _T( "Dmitriy Zakharov. RedMax3D.com." ); };
		virtual const TCHAR *	CopyrightMessage() { return _T("Free programm."); };
		virtual const TCHAR *	OtherMessage1() { return _T(""); };
		virtual const TCHAR *	OtherMessage2() { return _T(""); };
		virtual unsigned int	Version() { return 400; };
		virtual void			ShowAbout(HWND hWnd) {};

		virtual BOOL SupportsOptions(int ext, DWORD options) { return TRUE; };
		virtual int	DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

		//Constructor/Destructor
		RedMax3DPlugIn() {};
		virtual ~RedMax3DPlugIn() {};

	private:
		static HWND hParams;
};

class RedMax3DPlugInClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create( BOOL /*loading = FALSE*/ ){ return new RedMax3DPlugIn(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return RM3D_Export_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }
	virtual const TCHAR* InternalName() 			{ return _T("RedMax3D.com export"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
};