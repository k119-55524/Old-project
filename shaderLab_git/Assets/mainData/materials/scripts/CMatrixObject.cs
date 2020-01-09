using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CMatrixObject : MonoBehaviour {
	public GameObject matrixObject;

	Renderer m_Renderer;
	SphereCollider m_Collider;

	private void Start()
	{
		m_Renderer = gameObject.GetComponent<Renderer>();
		m_Renderer.enabled = true;
		m_Collider = matrixObject.GetComponent<SphereCollider>();
	}

	void FixedUpdate()
	{
		//float timeValue = Mathf.Ceil(Time.time % 16);
		//m_Renderer.material.SetFloat("_TimeValue", timeValue);

		Vector3 pos = matrixObject.transform.localPosition;
		m_Renderer.material.SetVector("_PosMatrixObj", pos);
		float rad = m_Collider.radius;
		m_Renderer.material.SetFloat("_RadMatrixObj", rad);
	}
}
