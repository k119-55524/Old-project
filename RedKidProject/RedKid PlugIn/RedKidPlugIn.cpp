#include "Skeleton.h"
#include "WndButton.h"
#include "WndListBox.h"
#include "WndCheckBox.h"
#include "RedKidPlugIn.h"
#include "WndStaticText.h"

static RedMax3DPlugInClassDesc RM3DExportDesc;
ClassDesc2* GetRM3DExportDesc() { return &RM3DExportDesc; }

HWND DlgExp;
HWND DlgExpProcess;
const TCHAR* ExpFileName;
int m_MeshCount = 0;

CWndButton cwPrewStartFrame;
CWndButton cwPrewEndFrame;
CWndButton cwNextStartFrame;
CWndButton cwNextEndFrame;
CWndButton cwSceletClearFrame;

CWndStaticText cwFileExpName;
CWndStaticText cwMeshCount;
CWndStaticText cwExportMeshCount;
CWndStaticText cwSelMeshName;
CWndStaticText cwBoneCount;
CWndStaticText cwYesNoTextCoord;
CWndStaticText cwVertexCount;
CWndStaticText cwStartFrame;
CWndStaticText cwEndFrame;
CWndStaticText cwNameText;
CWndStaticText cwFaceText;

CWndListBox cwMeshList;
CWndListBox cwExportMeshList;

CWndCheckBox cwGlobExpMesh;
CWndCheckBox cwGlobExpUVW;
CWndCheckBox cwGlobExpNormal;
CWndCheckBox cwGlobExpTanBin;
CWndCheckBox cwGlobSCData;
CWndCheckBox cwGlobExpScelet;
CWndCheckBox cwGlobExpSceletAnim;

IGameScene* pIgame = NULL;
Tab<IGameNode*> Meshes;
IGameObject* Obj = NULL;

MeshData* PrintMesh = NULL;
int StartFrame = 0;
int EndFrame = 0;

BOOL fExp;

UINT GetScelet( IGameObject* Obj )
{
	IGameSkin* Skin = Obj->GetIGameSkin();

	if ( !Skin )
		return 0;
	if ( IGameSkin::IGAME_SKIN != Skin->GetSkinType() )
		return 0;

	return Skin->GetNumOfSkinnedVerts();
}

void PrintMeshData( CWndListBox* List, UINT Index )
{
	MeshData* Data;

	Data = (MeshData*)List->GetItemData( Index );
	PrintMesh = Data;

	if ( Data == NULL )
		return;

	TCHAR Name[256];
	if ( LB_ERR == List->GetItemText( Index, Name ) )
		return;

	cwSelMeshName.SetText( Name );
	cwVertexCount.SetText( Data->VertexCount );

	cwStartFrame.SetText( Data->StartSceletFrame );
	cwEndFrame.SetText( Data->EndSceletFrame );

	if ( Data->BoneCount > 0 )
	{
		cwPrewStartFrame.SetEnable( TRUE );
		cwPrewEndFrame.SetEnable( TRUE );
		cwNextStartFrame.SetEnable( TRUE );
		cwNextEndFrame.SetEnable( TRUE );
		cwSceletClearFrame.SetEnable( TRUE );

		cwStartFrame.SetEnable( TRUE );
		cwEndFrame.SetEnable( TRUE );
		cwBoneCount.SetText( "Yes" );
	}
	else
	{
		cwPrewStartFrame.SetEnable( FALSE );
		cwPrewEndFrame.SetEnable( FALSE );
		cwNextStartFrame.SetEnable( FALSE );
		cwNextEndFrame.SetEnable( FALSE );
		cwSceletClearFrame.SetEnable( FALSE );

		cwStartFrame.SetEnable( FALSE );
		cwEndFrame.SetEnable( FALSE );
		cwBoneCount.SetText( "No" );
	}

	if ( Data->fTextCoord )
		cwYesNoTextCoord.SetText( "Yes" );
	else
		cwYesNoTextCoord.SetText( "No" );
}

void CopySelectLine( CWndListBox* InList, CWndListBox* OutList )
{
	UINT CopyCount = InList->GetSelectCount();

	if ( CopyCount == 0 )
		return;

	int* Indexes;
	TCHAR Name[256];
	Indexes = new int[CopyCount];
	InList->GetSelectItems( Indexes, CopyCount );
	MeshData* Data;
	for ( UINT i = 0; i < CopyCount; i++ )
	{
		Data = (MeshData*)InList->GetItemData( Indexes[i] );
		InList->GetItemText( Indexes[i], Name );
		OutList->AddLine( Name, (LPARAM)Data );
	}

	InList->DeleteListLine( Indexes, CopyCount, FALSE );
	delete [] Indexes;

	cwExportMeshCount.SetText( cwExportMeshList.GetCount() );
}

HRESULT InitDlgExp( HWND hdlg )
{
	DlgExp = hdlg;

	cwPrewStartFrame.Init( DlgExp, IDC_PSF );
	cwPrewEndFrame.Init( DlgExp, IDC_PEF );
	cwNextStartFrame.Init( DlgExp, IDC_NSF );
	cwNextEndFrame.Init( DlgExp, IDC_NEF );
	cwSceletClearFrame.Init( DlgExp, IDC_SC_FRAME_CLEAR );

	cwGlobExpMesh.Init( DlgExp, ID_CH_EXPMESH );
	cwGlobExpUVW.Init( DlgExp, ID_CH_EXPUVW );
	cwGlobExpNormal.Init( DlgExp, ID_CH_EXPNORM );
	cwGlobExpTanBin.Init( DlgExp, ID_CH_EXPTB );
	cwGlobSCData.Init( DlgExp, ID_CH_EXPSCDAT );
	cwGlobExpScelet.Init( DlgExp, ID_CH_EXPSCELET );
	cwGlobExpSceletAnim.Init( DlgExp, ID_CH_EXPSCELETMESH );

	cwGlobExpMesh.SetCheck( TRUE );
	cwGlobExpUVW.SetCheck( TRUE );
	cwGlobExpNormal.SetCheck( TRUE );
	cwGlobExpTanBin.SetCheck( TRUE );
	cwGlobSCData.SetCheck( TRUE );
	cwGlobExpScelet.SetCheck( TRUE );
	cwGlobExpSceletAnim.SetCheck( TRUE );

	cwSelMeshName.Init( DlgExp, ID_SEL_MESH_NAME );
	cwBoneCount.Init( DlgExp, ID_BONE_COUNT );
	cwYesNoTextCoord.Init( DlgExp, ID_YN_TEXTCOORD );
	cwVertexCount.Init( DlgExp, ID_VERTEX_COUNT );

	cwMeshList.Init( DlgExp, IDC_LIST_MESH );
	cwExportMeshList.Init( DlgExp, IDC_LIST_EXP_MESH );

	StartFrame = (int)( GetCOREInterface()->GetAnimRange().Start() / GetTicksPerFrame() );
	EndFrame   = (int)( GetCOREInterface()->GetAnimRange().End() / GetTicksPerFrame() );

	Tab<int> mTexMaps;
	int mTC;
	MeshData* Data;
	IGameNode* Node = NULL;
	for ( int i = 0; i < Meshes.Count(); i++ )
	{
		Node = Meshes[i];
		Obj = Node->GetIGameObject();
		Obj->InitializeData();

		if ( Obj->GetIGameType() == IGameMesh::IGAME_MESH )
		{
			m_MeshCount++;

			// Sceleton data
			Data = new MeshData();
			Data->Node = Node;
			Data->BoneCount = GetScelet( Obj );
			if ( Data->BoneCount > 0 )
			{
				Data->StartSceletFrame = StartFrame;
				Data->EndSceletFrame = EndFrame;
			}

			// TextCoord data
			Data->Mesh = static_cast<IGameMesh *>(Obj);
			mTexMaps = Data->Mesh->GetActiveMapChannelNum();
			mTC = mTexMaps.Count();
			if ( mTC > 0 )
				Data->fTextCoord = TRUE;
			else
				Data->fTextCoord = FALSE;

			// Vertex data
			Data->VertexCount = (UINT)Data->Mesh->GetNumberOfVerts();//->GetNumberOfFaces();

			cwMeshList.AddLine( Meshes[i]->GetName(), (LPARAM)Data ); 
		}
		else
			Node->ReleaseIGameObject();
	}

	// If no mesh
	if ( m_MeshCount == 0 )
	{
		MessageBox( NULL , "No compartible mesh.", "ERROR!!!", MB_ICONERROR | MB_OK );
		return S_FALSE;
	}

	cwFileExpName.Init( DlgExp, IDC_FILE_NAME );
	cwFileExpName.SetText( ExpFileName );

	cwMeshCount.Init( DlgExp, ID_TOTAL_MESH );
	cwMeshCount.SetText( m_MeshCount );

	cwExportMeshCount.Init( DlgExp, ID_TOTAL_EXP_MESH );
	cwStartFrame.Init( DlgExp, IDC_START_SC_FRAME );
	cwEndFrame.Init( DlgExp, IDC_END_SC_FRAME );

	return S_OK;
}

HRESULT CaptureVertexWeights( IGameObject* obj, Skeleton* skeleton, MeshVertexList* m_VertexList )
{
	IGameSkin* skin = obj->GetIGameSkin();
	if ( !skin )
		return FALSE;
	if ( IGameSkin::IGAME_SKIN != skin->GetSkinType() )
		return FALSE;

	const int numVerts = m_VertexList->GetCount();
	int i, j, numW, vID;

	for ( i = 0; i < numVerts; ++i )
	{
		vID = (*m_VertexList)[i].GetID();
		numW = skin->GetNumberOfBones( vID );

		if ( IGameSkin::IGAME_RIGID == skin->GetVertexType( vID ) )
			numW = 1;

		for ( j = 0; j < numW; ++j )
		{
			float w = skin->GetWeight( vID, j );
			if ( numW == 1 )
				w = 1.0f;
			int boneId = skeleton->GetBoneIDByMaxID( skin->GetBoneID( vID, j ) );
			(*m_VertexList)[i].AddWeight( VertexWeight( w, boneId ) );
		}
	}

	return TRUE;
}

HRESULT BuildMesh( MeshData* Data, std::ofstream& ExpFile, TCHAR* Name, BOOL fScelet, Skeleton& skeleton )
{
	IGameMesh* Mesh = Data->Mesh;
	UINT faceCount = Mesh->GetNumberOfFaces();
	Tab<int> texMaps = Mesh->GetActiveMapChannelNum();

	TCHAR Text[256];
	MSG msg;
	FaceEx* Face;
	DWORD indices[3];
	Point3 uvw;
	Point3 p, n;
	MeshVertexList m_VertexList( faceCount * 3 );
	std::vector<int> m_IndexList;

	Tab<int> mTexMaps;
	mTexMaps = Data->Mesh->GetActiveMapChannelNum();
	int mTC = mTexMaps.Count();

	ExpFile << "[Start_Mesh_block]\n";
	ExpFile << Name << "\n";

	if ( cwGlobExpNormal.GetCheck() )
	{
		ExpFile << "Normal:TRUE.\n";

		if ( cwGlobExpTanBin.GetCheck() )
			ExpFile << "TangBinor:TRUE.\n";
		else
			ExpFile << "TangBinor:FALSE.\n";
	}
	else
		ExpFile << "Normal:FALSE.\n";

	if ( mTC > 0 && cwGlobExpUVW.GetCheck() )
		ExpFile << "UVW:TRUE.\n";
	else
		ExpFile << "UVW:FALSE.\n";

	if ( fScelet && cwGlobSCData.GetCheck() )
		ExpFile << "SceletData:TRUE.\n";
	else
		ExpFile << "SceletData:FALSE.\n";

	m_IndexList.resize( faceCount * 3 );

	static GMatrix	m_Matrix;
	m_Matrix = Data->Node->GetObjectTM( 0 );

	int IndexTBN;
	Point3 TBN;
	Mesh->InitializeBinormalData();
	int stch = 0;
	int ststep = faceCount / 100;
	for ( UINT i = 0; i < faceCount; ++i )
	{
		//////////////////////////////////////////////////////////////////
		stch++;
		if ( stch > ststep )
		{
			stch = 0;
			sprintf( Text, "Export %d%%.", (UINT)( ( 100.0f / (float)faceCount ) * (float)i ) );
			cwFaceText.SetText( Text );
		}

		if ( PeekMessage( &msg, DlgExpProcess, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( !fExp )
			return S_FALSE;
		//////////////////////////////////////////////////////////////////

		Face = Mesh->GetFace( i );
		for ( UINT j = 0; j < 3; ++j )
		{
			p = Mesh->GetVertex( Face->vert[ j ] );

			MeshVertex v( p.x, p.y, p.z );

			n = Mesh->GetNormal( Face, j );
			v.SetNormal( n.x, n.y, n.z );
			v.SetID( Face->vert[ j ] );

			if ( Mesh->GetMapFaceIndex( texMaps[ 0 ], i, indices ) )
				uvw = Mesh->GetMapVertex( texMaps[ 0 ], indices[ j ] );
			else
				uvw = Mesh->GetMapVertex( texMaps[ 0 ], Face->vert[ j ] );

			v.SetTexCoord( uvw.x, 1.0f - uvw.y );

			IndexTBN = Mesh->GetFaceVertexTangentBinormal( i, j );
			TBN = Mesh->GetTangent( IndexTBN );
			TBN.Normalize();
			v.SetTangent( TBN.x, TBN.y, TBN.z );
			TBN = Mesh->GetBinormal( IndexTBN );
			TBN.Normalize();
			TBN = -TBN;
			v.SetBinormal( TBN.x, TBN.y, TBN.z );

			m_IndexList[ 3 * i + j ] = m_VertexList.Add( v );
		}
	}

	if ( -1 == m_Matrix.Parity() )
	{
		std::vector<int> newInds;
		newInds.resize( m_IndexList.size() );
		for ( UINT iFace = 0; iFace < faceCount / 3; ++iFace )
		{
			newInds[ 3 * iFace + 0 ] = m_IndexList[ 3 * iFace + 2 ];
			newInds[ 3 * iFace + 1 ] = m_IndexList[ 3 * iFace + 1 ];
			newInds[ 3 * iFace + 2 ] = m_IndexList[ 3 * iFace + 0 ];
		}
	}

	// Данные о привязке сетки к скелету
	if ( fScelet && cwGlobSCData.GetCheck() )
		CaptureVertexWeights( Mesh, &skeleton, &m_VertexList );

	ExpFile << "Vertex_count:\n" << m_VertexList.GetCount() << "\n";

	sprintf( Text, "Out mesh %s of file.", Name );
	cwFaceText.SetText( Text );

	for ( UINT i = 0; i < m_VertexList.GetCount(); i++ )
	{
		//////////////////////////////////////////////////////////////////
		if( PeekMessage( &msg, DlgExpProcess, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( !fExp )
			m_IndexList.clear();
		//////////////////////////////////////////////////////////////////

		MeshVertex& v = m_VertexList[i];
		ExpFile << v.GetPosition().x << " " << v.GetPosition().y << " " << v.GetPosition().z << std::endl;

		// Out Normal
		if ( cwGlobExpNormal.GetCheck() )
		{
			ExpFile << v.GetNormal().x << " " << v.GetNormal().y << " " << v.GetNormal().z << std::endl;

			// Out Tangent & Binormal
			if ( cwGlobExpTanBin.GetCheck() )
			{
				ExpFile << v.GetTangent().x << " " << v.GetTangent().y << " " << v.GetTangent().z << std::endl;
				ExpFile << v.GetBinormal().x << " " << v.GetBinormal().y << " " << v.GetBinormal().z << std::endl;
			}
		}

		// Out UVW
		if ( mTC > 0 && cwGlobExpUVW.GetCheck() )
			ExpFile << v.GetTexCoord().x << " " << v.GetTexCoord().y << std::endl;

		// Out Scelet data
		if ( fScelet && cwGlobSCData.GetCheck() )
		{
			ExpFile << v.GetNumWeights() << std::endl;
			for ( int j = 0; j < v.GetNumWeights(); ++j )
				ExpFile << v.GetWeight( j ).GetBoneID() << " " << v.GetWeight( j ).GetWeight() << std::endl;
		}
	}

	ExpFile << "[Start_Index_Block]\n";
	ExpFile << "Index_count:\n" << m_IndexList.size() << "\n";
	for ( UINT i = 0; i < m_IndexList.size(); i += 3 )
	{
		//////////////////////////////////////////////////////////////////
		if( PeekMessage( &msg, DlgExpProcess, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( !fExp )
			return S_FALSE;
		//////////////////////////////////////////////////////////////////

		ExpFile << m_IndexList[ i ] << " " << m_IndexList[ i + 1 ] << " " << m_IndexList [i + 2 ] << " " << std::endl;
	}
	ExpFile << "[End_Index_Block]\n";

	m_IndexList.clear();

	ExpFile << "[End_Mesh_block]\n";

	return S_OK;
}

LRESULT CALLBACK DlgExpProc( HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_CLOSE:
	{
		EndDialog( hdlg, TRUE );
		fExp = FALSE;
		return TRUE;
	}
	case WM_COMMAND:
		switch ( LOWORD( wParam ) )
		{
			case ID_STOP_EXP:
				{
					fExp = FALSE;
					return TRUE;
				}
		}
		break;
	}

	return FALSE;
}

LRESULT CALLBACK DlgHelpProc( HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		case WM_CLOSE:
		{
			EndDialog( hdlg, TRUE );
			return TRUE;
		}
	}

	return FALSE;
}

HRESULT Export( void )
{
	UINT CointMesh = cwExportMeshList.GetCount();
	if ( CointMesh == 0 )
		return S_OK;

	fExp = TRUE;
	HRSRC hrsrc = FindResource( hInstance, MAKEINTRESOURCE( IDD_DLG_EXP_PROC ), RT_DIALOG );
    if( hrsrc != NULL )
	{
		HGLOBAL hglb = LoadResource( hInstance, hrsrc ); 
		if( hglb != NULL )
		{
			DLGTEMPLATE* t1 = (DLGTEMPLATE*)LockResource(hglb);
			DlgExpProcess = CreateDialogIndirect( NULL, t1, DlgExp, (DLGPROC)DlgExpProc );
		}
		else
		{
		    MessageBox( NULL, "LoadResource Error.", "ERROR!!!", MB_ICONERROR | MB_OK );
			return S_FALSE;
		}
	}
	else
	{
        MessageBox( NULL , "FindResource Error.", "ERROR!!!", MB_ICONERROR | MB_OK );
		return S_FALSE;
	}

	TCHAR Name[256];
	TCHAR Text[256];
	std::ofstream ExpFile;
	MeshData* Data;
	BOOL fScelet;
	try
	{
		ExpFile.open( ExpFileName, std::ios::out );
		ExpFile << "[RedMax3D.com._RedKid_PlugIn_V4.0.0.]\n";
		ExpFile << "[Eport_mesh,_sceleton_and_sceleton_animation_of_3Ds_Max,_in_text_format_for_DirectX_dimension.]\n";

		cwNameText.Init( DlgExpProcess, IDC_EXP_NAME );
		cwFaceText.Init( DlgExpProcess, IDC_EXP_FACE );
		for ( UINT i = 0; i < CointMesh; i++ )
		{
			if ( !fExp )
			{
				ExpFile.close();
				EndDialog( DlgExpProcess, FALSE );
				return S_FALSE;
			}

			cwExportMeshList.GetItemText( i, Name );
			Data = (MeshData*)cwExportMeshList.GetItemData( i );

			sprintf( Text, "Export mesh %s(%d of %d).", Name, i + 1, CointMesh );
			cwNameText.SetText( Text );

			Skeleton skeleton;
			fScelet = skeleton.Build( Data->Mesh );
			if ( cwGlobExpScelet.GetCheck() && fScelet )
				skeleton.WriteFile( ExpFile );

			if ( !fExp )
			{
				ExpFile.close();
				EndDialog( DlgExpProcess, FALSE );
				return S_FALSE;
			}

			if ( cwGlobExpMesh.GetCheck() )
				if ( S_OK != BuildMesh( Data, ExpFile, Name, fScelet, skeleton ) )
				{
					ExpFile.close();
					EndDialog( DlgExpProcess, FALSE );
					return S_FALSE;
				}

			if ( !fExp )
			{
				ExpFile.close();
				EndDialog( DlgExpProcess, FALSE );
				return S_FALSE;
			}

			AnimTrack animTrack;
			if ( fScelet && cwGlobExpSceletAnim.GetCheck() )
				if( animTrack.Build( Data->Mesh, &skeleton, Data->StartSceletFrame, Data->EndSceletFrame ) )
					animTrack.WriteFile( ExpFile, &skeleton, DlgExpProcess );
		}

		ExpFile.flush();
		ExpFile.close();
	}
	catch ( ... )
	{
		ExpFile.close();
		EndDialog( DlgExpProcess, FALSE );
		return S_FALSE;
	}

	EndDialog( DlgExpProcess, TRUE );

	return S_OK;
}

LRESULT CALLBACK DlgSetProc( HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	// Inicialise ExportDialog window
	case WM_INITDIALOG:
		{
			if ( S_OK != InitDlgExp( hdlg ) )
				EndDialog( hdlg, FALSE );

			return TRUE;
		}
	case WM_CLOSE:
	{
		EndDialog( hdlg, TRUE );
		return TRUE;
	}
	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			////////////////////////////////////////////////////////////////////////
			// Список с экспортируемыми сетками
			//
			case IDC_LIST_EXP_MESH:
				{
					// Выбор строки
					if ( HIWORD( wParam ) == LBN_SELCHANGE )
					{
						int Index = cwExportMeshList.GetCurSel();
						if ( Index == LB_ERR )
							break;

						PrintMeshData( &cwExportMeshList, Index );

						return TRUE;
					}
					// Двойной щелчёк по строке
					if ( HIWORD( wParam ) == LBN_DBLCLK )
					{
						return TRUE;
					}
					break;
				}
			//
			////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////
			// Список с сетками
			//
			case IDC_LIST_MESH:
				{
					// Выбор строки
					if ( HIWORD( wParam ) == LBN_SELCHANGE )
					{
						int Index = cwMeshList.GetCurSel();
						if ( Index == LB_ERR )
							break;

						PrintMeshData( &cwMeshList, Index );

						return TRUE;
					}
					// Двойной щелчёк по строке
					if ( HIWORD( wParam ) == LBN_DBLCLK )
					{
						return TRUE;
					}
					break;
				}
			//
			////////////////////////////////////////////////////////////////////////

			// CheckBox GlobalExportNormal
			case ID_CH_EXPNORM:
				{
					if ( cwGlobExpNormal.GetCheck() )
						cwGlobExpTanBin.SetEnable( TRUE );
					else
						cwGlobExpTanBin.SetEnable( FALSE );

					return TRUE;
				}

			// CheckBox GlobalExportMesh
			case ID_CH_EXPMESH:
				{
					if ( cwGlobExpMesh.GetCheck() )
					{
						cwGlobExpUVW.SetEnable( TRUE );
						cwGlobExpNormal.SetEnable( TRUE );
						cwGlobExpTanBin.SetEnable( TRUE );
						cwGlobSCData.SetEnable( TRUE );
					}
					else
					{
						cwGlobExpUVW.SetEnable( FALSE );
						cwGlobExpNormal.SetEnable( FALSE );
						cwGlobExpTanBin.SetEnable( FALSE );
						cwGlobSCData.SetEnable( FALSE );
					}

					return TRUE;
				}
			// Кнопка экспорта
			case ID_EXPORT:
				{
					Export();
					return TRUE;
				}
			case IDC_HELP_BUTTON:
				{
					DialogBox( hInstance, MAKEINTRESOURCE( IDD_DLG_HELP ), hdlg, (DLGPROC)DlgHelpProc );
					return TRUE;
				}
			// Выделение всех сеток в списке сеток
			case ID_SEL_ALL_MESH:
				{
					cwMeshList.SelectAll();

					return TRUE;
				}
			// Выделение всех сеток в списке экспортируемых сеток
			case ID_SEL_ALL_EXP_MESH:
				{
					cwExportMeshList.SelectAll();

					return TRUE;
				}
			// Кнопка выхода
			case ID_EXIT:
				{
					EndDialog( hdlg, TRUE );

					return TRUE;
				}

			////////////////////////////////////////////////////////////
			// MOVE MESH BLOCK

			// Add Mesh for Export
			case IDC_ADD_EXP_MESH:
				{
					CopySelectLine( &cwMeshList, &cwExportMeshList );
					return TRUE;
				}
			// Add All Mesh for Export
			case IDC_ADD_ALL_EXP_MESH:
				{
					cwMeshList.SelectAll();
					CopySelectLine( &cwMeshList, &cwExportMeshList );
					return TRUE;
				}
			// Add Export Mesh move to Mesh 
			case IDC_DEL_EXP_MESH:
				{
					CopySelectLine( &cwExportMeshList, &cwMeshList );
					return TRUE;
				}
			// Add All Export Mesh move to Mesh
			case IDC_DEL_ALL_EXP_MESH:
				{
					cwExportMeshList.SelectAll();
					CopySelectLine( &cwExportMeshList, &cwMeshList );
					return TRUE;
				}
			//
			////////////////////////////////////////////////////////////

			// Press Prev Start Frame
			case IDC_PSF:
				{
					if ( PrintMesh == NULL )
						return TRUE;

					if ( PrintMesh->StartSceletFrame > StartFrame )
						PrintMesh->StartSceletFrame--;

					cwStartFrame.SetText( PrintMesh->StartSceletFrame );
					cwEndFrame.SetText( PrintMesh->EndSceletFrame );

					return TRUE;
				}
			// Press Next Start Frame
			case IDC_NSF:
				{
					if ( PrintMesh == NULL )
						return TRUE;

					if ( PrintMesh->StartSceletFrame < EndFrame )
						PrintMesh->StartSceletFrame++;

					if ( PrintMesh->StartSceletFrame > PrintMesh->EndSceletFrame )
						PrintMesh->EndSceletFrame = PrintMesh->StartSceletFrame;

					cwStartFrame.SetText( PrintMesh->StartSceletFrame );
					cwEndFrame.SetText( PrintMesh->EndSceletFrame );

					return TRUE;
				}
			// Press Prev End Frame
			case IDC_PEF:
				{
					if ( PrintMesh == NULL )
						return TRUE;

					if ( PrintMesh->EndSceletFrame > StartFrame )
						PrintMesh->EndSceletFrame--;

					if ( PrintMesh->EndSceletFrame < PrintMesh->StartSceletFrame )
						PrintMesh->StartSceletFrame = PrintMesh->EndSceletFrame;

					cwStartFrame.SetText( PrintMesh->StartSceletFrame );
					cwEndFrame.SetText( PrintMesh->EndSceletFrame );

					return TRUE;
				}
			// Press Next End Frame
			case IDC_NEF:
				{
					if ( PrintMesh == NULL )
						return TRUE;

					if ( PrintMesh->EndSceletFrame < EndFrame )
						PrintMesh->EndSceletFrame++;

					cwStartFrame.SetText( PrintMesh->StartSceletFrame );
					cwEndFrame.SetText( PrintMesh->EndSceletFrame );

					return TRUE;
				}
			// Clear Scelet Frame
			case IDC_SC_FRAME_CLEAR:
				{
					if ( PrintMesh == NULL )
						return TRUE;

					PrintMesh->StartSceletFrame = StartFrame;
					PrintMesh->EndSceletFrame = EndFrame;

					cwStartFrame.SetText( PrintMesh->StartSceletFrame );
					cwEndFrame.SetText( PrintMesh->EndSceletFrame );

					return TRUE;
				}
			break;
			}
		}
	}

	return FALSE;
}

int	RedMax3DPlugIn::DoExport( const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts, DWORD options )
{
	InitCommonControls();

	ExpFileName = name;
	pIgame = NULL;
	m_MeshCount = 0;

	pIgame = GetIGameInterface();
	if ( pIgame == NULL )
	{
		MessageBox( NULL, "Error GetIGameInterface().", "ERROR!!!", MB_ICONERROR | MB_OK );
		return 0;
	}

	IGameConversionManager* cm = GetConversionManager();
	cm->SetCoordSystem( IGameConversionManager::IGAME_D3D );

	pIgame->InitialiseIGame();
	pIgame->SetStaticFrame(0);

	Meshes = pIgame->GetIGameNodeByType( IGameObject::IGAME_MESH );
	if ( Meshes.Count() == 0 )
	{
		MessageBox( NULL, "No mesh.", "ERROR!!!", MB_ICONERROR | MB_OK );
		return 0;
	}

	// Show export dialog
	int res = (int)DialogBox( hInstance, MAKEINTRESOURCE( IDD_DLG_EXPORT ), i->GetMAXHWnd(), (DLGPROC)DlgSetProc );

	// Release Interface
	pIgame->ReleaseIGame();
	pIgame = NULL;

	return res;
}

