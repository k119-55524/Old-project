using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class CBaseWindow : MonoBehaviour
{
	public abstract void LockInput();

	public abstract void UnLockInput();

	public virtual void EndShow()
	{

	}

	public virtual void EndHide()
	{

	}

	public virtual void StartShow()
	{

	}
}
