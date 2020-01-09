
#if UNITY_EDITOR

using UnityEngine;
using UnityEditor;
using System.Collections.Generic;

namespace RedDev.Utils
{
	public class AtlasSpriteSelectorEditor : EditorWindow
	{
		[SerializeField]
		private Texture2D _selectorTexture = null;

		private ImageAtlasEditor _editor;
		private Vector2 _scrollView;

		private int _selectedIndex = -1;
		
		private const int ROW_COUNT = 4;
		private const int OFFSET_SPRITE = 8;
		private const int SIZE_SPRITE = 48;
		private const int BORDERED_SPRITE = SIZE_SPRITE + OFFSET_SPRITE;

		private int _countLines = 0;

		private readonly List<Rect> _rects = new List<Rect>();
		private Rect _selectedRect = new Rect();

		private string _atlasName = "";

		private double _doubleClickTime = 0.3;
		private double _clickTime = 0;

		public void Init(ImageAtlasEditor editor)
		{
			_editor = editor;
			if (_editor.TargetAtlas.Atlas == null)
			{
				Debug.LogWarning("Сначала выберите атлас");
				Close();

				return;
			}

			_atlasName = _editor.TargetAtlas.Atlas.name;

			_countLines = (_editor.Textures.Count / ROW_COUNT);

			_rects.Clear();
			int x = 0;
			int y = 0;
			for (int i = 0; i < _editor.Textures.Count; i++)
			{
				x = i % ROW_COUNT;
				y = i / ROW_COUNT;

				_rects.Add(new Rect(OFFSET_SPRITE + (x * BORDERED_SPRITE), OFFSET_SPRITE + (y * BORDERED_SPRITE), SIZE_SPRITE, SIZE_SPRITE));
			}
		}

		void OnGUI()
		{
			if (Selection.activeTransform.gameObject.GetComponent<ImageAtlas>() == null)
				return;

			if (Selection.activeTransform.gameObject.GetInstanceID() != _editor.TargetAtlas.gameObject.GetInstanceID())
			{
				_editor.TargetAtlas = Selection.activeTransform.gameObject.GetComponent<ImageAtlas>();
				Init(_editor);
			}
			if (_editor.TargetAtlas == null)
			{
				GUILayout.FlexibleSpace();
				GUILayout.BeginHorizontal();
				GUILayout.FlexibleSpace();
				GUILayout.Label("Сначала выберите атлас!");
				GUILayout.FlexibleSpace();
				GUILayout.EndHorizontal();
				GUILayout.FlexibleSpace();
				return;
			}
			if (_atlasName != _editor.TargetAtlas.Atlas.name)
			{
				_atlasName = _editor.TargetAtlas.Atlas.name;
				Init(_editor);
			}

			_scrollView = EditorGUILayout.BeginScrollView(_scrollView, false, true);
			GUILayout.Label("", GUILayout.Width(ROW_COUNT * BORDERED_SPRITE), GUILayout.Height(_countLines * BORDERED_SPRITE));
			int counter = 0;
			
			for (int i = 0; i < _editor.Textures.Count; i++)
			{
				GUI.DrawTexture(_rects[i], _editor.Textures[counter]);

				counter++;
			}

			Event e = Event.current;
			int[] currentPos = new int[2];
			currentPos[0] = (int)((e.mousePosition.x) / BORDERED_SPRITE);
			currentPos[1] = (int)((e.mousePosition.y) / BORDERED_SPRITE);
			if (e.type == EventType.MouseDown && e.button == 0)
			{
				_selectedIndex = currentPos[0] + (currentPos[1] * ROW_COUNT);

				if ((EditorApplication.timeSinceStartup - _clickTime) < _doubleClickTime)
					SelectElement(_selectedIndex);
				_clickTime = EditorApplication.timeSinceStartup;

				int x = _selectedIndex % ROW_COUNT;
				int y = _selectedIndex / ROW_COUNT;
				_selectedRect = new Rect(OFFSET_SPRITE + (x * BORDERED_SPRITE), OFFSET_SPRITE + (y * BORDERED_SPRITE), SIZE_SPRITE, SIZE_SPRITE);
			}
			
			if (_selectedIndex > -1)
				GUI.DrawTexture(_selectedRect, _selectorTexture);

			EditorGUILayout.EndScrollView();

			if (EditorWindow.mouseOverWindow == this)
			{
				Repaint();
			}
		}

		private void SelectElement(int index)
		{
			_editor.SelectSprite(index);
		}
	}
}

#endif