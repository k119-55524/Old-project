
using UnityEngine;

public class CDeletePS : MonoBehaviour
{
	ParticleSystem ps;

	// Use this for initialization
	void Start ()
	{
		ps = GetComponent<ParticleSystem>();
	}
	
	// Update is called once per frame
	void Update ()
	{
		if (!ps.IsAlive())
		{
			Destroy(gameObject);
		}
	}
}
