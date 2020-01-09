
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class CUnivRot : MonoBehaviour
{
	// Update is called once per frame
	void Update ()
	{
		transform.Rotate(0.0f, 0.0f, 50.0f * Time.deltaTime);
	}
}
