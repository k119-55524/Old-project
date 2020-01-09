using System;
using System.Collections.Generic;
using System.Linq;
using UnityEditor;
using UnityEngine;
using UnityEngine.U2D;
using UnityEngine.UI;

namespace RedDev.Utils {
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

	[CustomEditor(typeof(ImageAtlas))]
	[CanEditMultipleObjects]
	public class ImageAtlasEditor : Editor
	{
		private ImageAtlas _target;

		private SpriteAtlas _oldAtlas;

		private Sprite[] _sprites;
		private readonly List<string> _names = new List<string>();

		public override void OnInspectorGUI()
		{
			if (_target == null)
				_target = target as ImageAtlas;

			if ((_target != null) && (_target.Atlas == null))
			{
				_names.Clear();
			}

			if ((_target != null) && (_target.Atlas != null) && ((_oldAtlas == null) || (_oldAtlas != _target.Atlas)))
			{
				_sprites = new Sprite[_target.Atlas.spriteCount];
				_target.Atlas.GetSprites(_sprites);
				_oldAtlas = _target.Atlas;
				_names.Clear();
				_names.AddRange(_sprites.Select(x => x.name.Replace("(Clone)", "")).ToArray());
			}

			_target.Atlas = EditorGUILayout.ObjectField("Atlas", _target.Atlas, typeof(SpriteAtlas), false) as SpriteAtlas;
			int index = EditorGUILayout.Popup("Sprite", _names.IndexOf(_target.NameSprite), _names.ToArray());
			if ((_names.Count == 0) || (index < 0))
				return;
			_target.NameSprite = _names[index];

		}
	}
}