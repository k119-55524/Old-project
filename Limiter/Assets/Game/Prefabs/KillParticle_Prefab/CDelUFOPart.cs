
using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CDelUFOPart : MonoBehaviour
{
	private ArrayList m_PS;

	// Use this for initialization
	void Awake()
	{
		IsPlay = false;
		FindPS();
	}

	public void SetColor(Color color)
	{
		foreach (ParticleSystem ps in m_PS)
		{
			ParticleSystem.MainModule settings = ps.GetComponent<ParticleSystem>().main;
			settings.startColor = color;
		}
	}

	// Update is called once per frame
	void Update()
	{
		foreach (ParticleSystem ps in m_PS)
			if (ps.IsAlive())
				return;

		Destroy(gameObject);
	}

	public bool IsPlay
	{
		get; private set;
	}

	public void Play()
	{
		IsPlay = true;

		foreach (ParticleSystem ps in m_PS)
			ps.Play();
	}

	private void FindPS()
	{
		m_PS = new ArrayList();

		FindPS(gameObject);
	}

	private void FindPS(GameObject parent)
	{
		if (parent == null)
			return;

		ParticleSystem ps = parent.GetComponent<ParticleSystem>();
		if (ps != null)
		{
			m_PS.Add(ps);
			ps.Pause();
		}

		for (int i = 0; i < parent.transform.childCount; i++)
			FindPS(parent.transform.GetChild(i).gameObject);
	}
}
