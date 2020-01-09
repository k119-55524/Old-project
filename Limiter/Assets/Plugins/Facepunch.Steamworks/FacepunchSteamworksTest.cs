using UnityEngine;
using System.Collections;

public class FacepunchSteamworksTest : MonoBehaviour
{
    Facepunch.Steamworks.Client steamClient;

	void OnEnable()
    {
        steamClient = new Facepunch.Steamworks.Client( 220 ); // 220 = half-life 2 appid

        if ( !steamClient.IsValid )
        {
            steamClient.Dispose();
            throw new System.Exception( "Couldn't init Steam - is Steam running? Do you own Half-Life 2? Is steam_appid.txt in your project folder?" );
        }

        Debug.Log( "Hello " + steamClient.Username );
        Debug.Log( "Your SteamID is " + steamClient.SteamId );

    }

    void OnDisable()
    {
        if ( steamClient != null )
        {
            steamClient.Dispose();
            steamClient = null;
        }
    }
}
