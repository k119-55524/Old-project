using UnityEngine;
using System.Collections.Generic;

public class CRotate : MonoBehaviour {
	public Vector3 RotationSpeed = new Vector3(0.0f, 0.0f, 0.0f);

	// Update is called once per frame
	void Update () {
		transform.Rotate(Time.deltaTime * RotationSpeed.x, Time.deltaTime * RotationSpeed.y, Time.deltaTime * RotationSpeed.z);
	}
}
