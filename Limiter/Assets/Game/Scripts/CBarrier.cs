
using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CBarrier : MonoBehaviour
{
	//private SpriteRenderer m_SprRend;
	private Material m_Mat;

	// Use this for initialization
	void Awake()
	{
		m_Mat = GetComponent<SpriteRenderer>().material;
	}

	public void SetAlpha(float a)
	{
		Color c = m_Mat.color;
		c.a = a;
		m_Mat.color = c;
	}
}
