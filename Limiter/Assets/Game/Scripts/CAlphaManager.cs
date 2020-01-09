
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class CAlphaManager : MonoBehaviour
{
	private class CColorRef
	{
		public CColorRef(Image im)
		{
			_image = im;
			_startAlpha = _image.color.a;
		}

		public CColorRef(Text t)
		{
			_text = t;
			_startAlpha = _text.color.a;
		}

		private Image _image;
		private Text _text;
		private float _startAlpha;

		public void SetAlpha(float a)
		{
			a = a * _startAlpha;

			Mathf.Clamp(a, 0.0f, 1.0f);

			if (_image != null)
			{
				Color c = _image.color;
				c.a = a;
				_image.color = c;
			}
			else
			{
				Color c = _text.color;
				c.a = a;
				_text.color = c;
			}
		}
	}

	private ArrayList m_Colors;

	// Use this for initialization
	void Awake()
	{
		FindColors();
	}

	public void SetAlpha(float a)
	{
		foreach (CColorRef c in m_Colors)
			c.SetAlpha(a);
	}

	// Перебираем все объекты висящие на окне(вместе с окном)
	// и ищем все компоненты имеющие цвет для последующей регулировки прозрачности
	private void FindColors()
	{
		if (m_Colors != null)
			return;

		m_Colors = new ArrayList();

		FindColors(gameObject);
	}

	private void FindColors(GameObject parent)
	{
		if (parent == null)
			return;

		Image img = parent.GetComponent<Image>();
		if (img != null)
		{
			m_Colors.Add(new CColorRef(img));
		}
		else
		{
			Text txt = parent.GetComponent<Text>();

			if (txt != null)
				m_Colors.Add(new CColorRef(txt));
		}

		for (int i = 0; i < parent.transform.childCount; i++)
			FindColors(parent.transform.GetChild(i).gameObject);
	}
}
