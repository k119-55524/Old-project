
#if UNITY_EDITOR

using System;
using System.Linq;
using UnityEditor;
using UnityEngine;
using UnityEngine.U2D;
using Image = UnityEngine.UI.Image;
using System.Collections.Generic;

namespace RedDev.Utils
{
	[ExecuteInEditMode]
	[RequireComponent(typeof(Image))]
	public class ImageAtlas : MonoBehaviour
	{
		private Image _image;

		[SerializeField]
		private SpriteAtlas _atlas;

		public SpriteAtlas Atlas
		{
			get { return _atlas; }
			set
			{
				_atlas = value;
				if (_atlas == null)
				{
					NameSprite = "";
				}
			}
		}

		[SerializeField]
		private Sprite _sprite;

		[SerializeField]
		private string _oldNameSprite;

		[SerializeField]
		private string _nameSprite;
		public string NameSprite
		{
			get { return _nameSprite; }
			set
			{
				if (_image == null)
					_image = GetComponent<Image>();

				_nameSprite = value;
				if (String.IsNullOrEmpty(_nameSprite))
					_sprite = null;
				else
				{
					if (!_nameSprite.Equals(_oldNameSprite) && _atlas != null)
					{
						_oldNameSprite = _nameSprite;
						_sprite = _atlas.GetSprite(_nameSprite);
						_image.sprite = _sprite;
					}
				}
			}
		}

		void OnEnable()
		{
			if (_image == null)
				_image = GetComponent<Image>();
			_image.sprite = _sprite;
		}
	}

	[CanEditMultipleObjects]
	[CustomEditor(typeof(ImageAtlas))]
	public class ImageAtlasEditor : Editor
	{
		public ImageAtlas TargetAtlas { get; set; }

		private SpriteAtlas _oldAtlas;

		private Sprite[] _sprites;
		private readonly List<string> _names = new List<string>();

		public List<string> Names
		{
			get { return _names; }
		}

		private readonly List<Texture2D> _textures = new List<Texture2D>();

		public List<Texture2D> Textures
		{
			get { return _textures; }
		}

		private int _index = 0;

		public override void OnInspectorGUI()
		{
			if (TargetAtlas == null)
				TargetAtlas = target as ImageAtlas;
			
			if ((TargetAtlas != null) && (TargetAtlas.Atlas == null))
			{
				_names.Clear();
				_textures.Clear();
			}

			if ((TargetAtlas != null) && (TargetAtlas.Atlas != null) && ((_oldAtlas == null) || (_oldAtlas != TargetAtlas.Atlas)))
			{
				_sprites = new Sprite[TargetAtlas.Atlas.spriteCount];
				TargetAtlas.Atlas.GetSprites(_sprites);
				_oldAtlas = TargetAtlas.Atlas;
				_names.Clear();
				_names.AddRange(_sprites.Select(x => x.name.Replace("(Clone)", "")).ToArray());
				_textures.Clear();
				_textures.AddRange(_sprites.Select(x => x.texture));
			}

			TargetAtlas.Atlas = EditorGUILayout.ObjectField("Atlas", TargetAtlas.Atlas, typeof(SpriteAtlas), false) as SpriteAtlas;

			GUILayout.Label("Выберите спрайт: ");
			var rectPreview = GUILayoutUtility.GetRect(120, 120, GUILayout.ExpandWidth(false), GUILayout.ExpandHeight(false));
			var rectBorderPreview = new Rect(rectPreview);
			rectBorderPreview.x -= 1;
			rectBorderPreview.y -= 1;
			rectBorderPreview.width += 2;
			rectBorderPreview.height += 2;

			var gui = new GUIStyle();
			gui.normal.background = (TargetAtlas.Atlas != null && !String.IsNullOrEmpty(TargetAtlas.NameSprite)) ?
				TargetAtlas.Atlas.GetSprite(TargetAtlas.NameSprite).texture : Texture2D.blackTexture;

			EditorGUI.DrawPreviewTexture(rectBorderPreview, Texture2D.blackTexture);
			if (GUI.Button(rectPreview, "", gui))
			{
				AtlasSpriteSelectorEditor window = EditorWindow.GetWindow<AtlasSpriteSelectorEditor>();
				window.Init(this);
				window.maxSize = new Vector2(224, window.maxSize.y);
				window.minSize = new Vector2(250, 56);
			}

			EditorGUILayout.Space();
		}

		public void SelectSprite(int index)
		{
			_index = index;
			TargetAtlas.NameSprite = _names[_index];
		}
	}
}

#endif